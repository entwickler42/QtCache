#include "gitrepository.h"
#include <qtcacheexception.h>
#include <git2.h>

inline void git_eval(int err){
    if (err) {
        const git_error* err = giterr_last();
        throw QtC::QtCacheException(err->message);
    }
}

template<typename T> class git_auto
{
public:
    git_auto(T* object = NULL)
        : ref(object)
    {}
    ~git_auto();

    operator T*() const { return ref; }
    T** operator &() {return &ref; }

private:
    T* ref;
};

git_auto<git_repository>::~git_auto() { git_repository_free(this->ref); }
git_auto<git_signature>::~git_auto() { git_signature_free(this->ref); }
git_auto<git_index>::~git_auto() { git_index_free(this->ref); }
git_auto<git_tree>::~git_auto() { git_tree_free(this->ref); }
git_auto<git_reference>::~git_auto() { git_reference_free(this->ref); }
git_auto<git_commit>::~git_auto() { git_commit_free(this->ref); }
git_auto<git_status_list>::~git_auto() { git_status_list_free(this->ref); }

GitRepository::GitRepository(const QString &directoryPath)
    : m_local_directory(directoryPath),
      m_repo(NULL)
{
    git_libgit2_init();
}

GitRepository::~GitRepository()
{
    git_libgit2_shutdown();
    if (NULL != m_repo) {
        git_repository_free(m_repo);
    }
}

bool GitRepository::isValid()
{
    return NULL != m_repo;
}

void GitRepository::setRepository(git_repository* repo)
{
    if (NULL != m_repo){
        git_repository_free(m_repo);
    }
    m_repo = repo;
}

git_repository* GitRepository::repository()
{
    if (NULL == m_repo){
        try{
            open();
        }catch(...){
            try{
                init();
            }catch(...){
                throw;
            }
        }
    }
    return m_repo;
}

void GitRepository::open()
{
    git_repository* repo = NULL;
    git_eval(git_repository_open(&repo, m_local_directory.absolutePath().toLocal8Bit()));
    setRepository(repo);
}

void GitRepository::init()
{
    git_repository* repo = NULL;

    git_repository_init_options initopts = GIT_REPOSITORY_INIT_OPTIONS_INIT;
    initopts.flags = GIT_REPOSITORY_INIT_MKPATH;
    git_eval(git_repository_init_ext(&repo, m_local_directory.absolutePath().toLocal8Bit(), &initopts));

    git_auto<git_signature> sig;
    git_eval(git_signature_now(&sig, "QtCacheTool", "qtcachetool@bobmail.info"));

    git_auto<git_index> index;
    git_eval(git_repository_index(&index, repo));

    git_oid tree_id;
    git_eval(git_index_write_tree(&tree_id, index));

    git_auto<git_tree> tree;
    git_eval(git_tree_lookup(&tree, repo, &tree_id));

    git_oid commit_id;
    git_eval(git_commit_create_v(&commit_id, repo, "HEAD", sig, sig, NULL, "Initial commit", tree, 0));

    setRepository(repo);
}

void GitRepository::branch(const QString& name)
{
    git_repository* repo = repository();
    git_auto<git_reference> branch;

    int err = git_branch_lookup(&branch, repo, name.toLocal8Bit(), GIT_BRANCH_LOCAL);
    if (err == GIT_ENOTFOUND){
        git_oid parent_id;
        git_auto<git_commit> parent;

        git_eval(git_reference_name_to_id(&parent_id, repo, "HEAD"));
        git_eval(git_commit_lookup(&parent, repo, &parent_id));
        git_eval(git_branch_create(&branch, repo, name.toLocal8Bit(), parent, 1));
    }else{
        git_eval(err);
    }

    git_eval(git_repository_set_head(repo, git_reference_name(branch)));

    git_checkout_options opts = GIT_CHECKOUT_OPTIONS_INIT;
    opts.checkout_strategy = GIT_CHECKOUT_FORCE;

    git_eval(git_checkout_head(repo, &opts));
}

void GitRepository::add(const QString& filepath)
{
    git_repository* repo = repository();
    git_auto<git_index> index;
    git_eval(git_repository_index(&index, repo));
    git_eval(git_index_add_bypath(index, filepath.toLatin1()));
    git_index_write(index);
    git_index_free(index);
}

void GitRepository::commit(const QString& commitMessage)
{
    git_repository* repo = repository();

    {
        git_auto<git_status_list> changes;
        git_eval(git_status_list_new(&changes, repo, NULL));
        if (git_status_list_entrycount(changes) == 0) { return; }
    }

    git_auto<git_index> index;
    git_eval(git_repository_index(&index, repo));

    git_oid tree_id;
    git_eval(git_index_write_tree(&tree_id, index));

    git_auto<git_tree> tree;
    git_eval(git_tree_lookup(&tree, repo, &tree_id));

    git_oid parent_id;
    git_eval(git_reference_name_to_id(&parent_id, repo, "HEAD"));

    git_auto<git_commit> parent;
    git_eval(git_commit_lookup(&parent, repo, &parent_id));

    git_auto<git_signature> sig;
    git_eval(git_signature_now(&sig, "QtCacheTool", "qtcachetool@bobmail.info"));

    git_oid commit_id;
    git_eval(git_commit_create_v(&commit_id, repo, "HEAD", sig, sig, NULL, commitMessage.toLocal8Bit(), tree, 1, parent));
}

void GitRepository::clone(const QUrl& remote, const QString& branch)
{
}

void GitRepository::push(const QUrl& remote, const QString& branch)
{}
