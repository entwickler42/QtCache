#ifndef GITREPOSITORY_H
#define GITREPOSITORY_H

#include <QString>
#include <QDir>

struct git_repository;

class GitRepository
{
public:
    GitRepository(const QString& directoryPath);
    ~GitRepository();

    bool isValid();

    void open();
    void init();

    void branch(const QString& name);
    void add(const QString& filepath);
    void commit(const QString& commitMessage);

    void clone(const QString& url);
    void push(const QString& remote, const QString& branch = QString());

private:
    QDir m_local_directory;
    git_repository* m_repo;

    void setRepository(git_repository* repo);
    git_repository* repository();
};

#endif // GITREPOSITORY_H
