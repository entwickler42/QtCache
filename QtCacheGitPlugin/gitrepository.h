#ifndef GITREPOSITORY_H
#define GITREPOSITORY_H

#include <QString>
#include <QDir>
#include <QUrl>

class GitRepository
{
public:
    GitRepository(const QString& directoryPath);
    ~GitRepository();

    bool isValid();

    void init();
    void branch(const QString& name);
    void resetHard();
    void clone(const QUrl& remote, const QString& branch = QString());
    void add(const QString& filepath);
    void commit(const QString& commitMessage);
    void push(const QUrl& remote, const QString& branch = QString());

private:
    QDir m_local_directory;

    void* m_repository = NULL;
};

#endif // GITREPOSITORY_H
