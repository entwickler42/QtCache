#include "gitrepository.h"
#include <qtcacheexception.h>
#include <git2.h>

inline void __gitcall(int err){
    if (err) {
        const git_error* err = giterr_last();
        throw QtC::QtCacheException(err->message);
    }
}

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
    __gitcall(git_repository_open(&repo, m_local_directory.absolutePath().toLocal8Bit()));
    setRepository(repo);
}

void GitRepository::init()
{
    git_repository* repo = NULL;

    git_repository_init_options initopts = GIT_REPOSITORY_INIT_OPTIONS_INIT;
    initopts.flags = GIT_REPOSITORY_INIT_MKPATH;
    __gitcall(git_repository_init_ext(&repo, m_local_directory.absolutePath().toLocal8Bit(), &initopts));

    git_signature *sig = NULL;
    __gitcall(git_signature_now(&sig, "QtCacheTool", "qtcachetool@bobmail.info"));

    git_index *index = NULL;
    __gitcall(git_repository_index(&index, repo));

    git_oid tree_id;
    __gitcall(git_index_write_tree(&tree_id, index));

    git_tree *tree = NULL;
    __gitcall(git_tree_lookup(&tree, repo, &tree_id));

    git_oid commit_id;
    __gitcall(git_commit_create_v(&commit_id, repo, "HEAD", sig, sig, NULL, "Initial commit", tree, 0));

    git_tree_free(tree);
    git_index_free(index);
    git_signature_free(sig);

    setRepository(repo);
}

void GitRepository::clone(const QUrl& remote, const QString& branch)
{
}

void GitRepository::branch(const QString& name)
{
    git_repository* repo = repository();
    git_reference *branch = NULL;

    int err = git_branch_lookup(&branch, repo, name.toLocal8Bit(), GIT_BRANCH_LOCAL);
    if (err == GIT_ENOTFOUND){
        git_oid parent_id;
        git_commit *parent = NULL;
        __gitcall(git_reference_name_to_id(&parent_id, repo, "HEAD"));
        __gitcall(git_commit_lookup(&parent, repo, &parent_id));

        __gitcall(git_branch_create(&branch, repo, name.toLocal8Bit(), parent, 1));
        git_commit_free(parent);
    }else{
        __gitcall(err);
    }

    __gitcall(git_repository_set_head(repo, git_reference_name(branch)));

    git_checkout_options opts = GIT_CHECKOUT_OPTIONS_INIT;
    opts.checkout_strategy = GIT_CHECKOUT_FORCE;
    __gitcall(git_checkout_head(repo, &opts));

    git_reference_free(branch);
}

void GitRepository::add(const QString& filepath)
{
    git_repository* repo = repository();
    git_index* index;
    __gitcall(git_repository_index(&index, repo));
    __gitcall(git_index_add_bypath(index, filepath.toLatin1()));
    git_index_write(index);
    git_index_free(index);
}

void GitRepository::commit(const QString& commitMessage)
{
    git_repository* repo = repository();

    git_status_list* changes = NULL;
    git_status_list_new(&changes, repo, NULL);
    size_t nchanges = git_status_list_entrycount(changes);
    git_status_list_free(changes);
    if (nchanges == 0){
        return;
    }

    git_index* index = NULL;
    __gitcall(git_repository_index(&index, repo));

    git_oid tree_id;
    __gitcall(git_index_write_tree(&tree_id, index));

    git_tree *tree = NULL;
    __gitcall(git_tree_lookup(&tree, repo, &tree_id));

    git_oid parent_id;
    __gitcall(git_reference_name_to_id(&parent_id, repo, "HEAD"));

    git_commit *parent;
    __gitcall(git_commit_lookup(&parent, repo, &parent_id));

    git_signature *sig = NULL;
    __gitcall(git_signature_now(&sig, "QtCacheTool", "qtcachetool@bobmail.info"));

    git_oid commit_id;
    __gitcall(git_commit_create_v(&commit_id, repo, "HEAD", sig, sig, NULL, commitMessage.toLocal8Bit(), tree, 1, parent));

    git_signature_free(sig);
    git_tree_free(tree);
    git_index_free(index);
}

void GitRepository::push(const QUrl& remote, const QString& branch)
{}
