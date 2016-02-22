#include "gitrepository.h"
#include <git2.h>

GitRepository::GitRepository(const QString &directoryPath)
    : m_local_directory(directoryPath)
{
}

GitRepository::~GitRepository()
{
}

bool GitRepository::isValid()
{
    return false;
}

void GitRepository::init()
{}

void GitRepository::branch(const QString& name)
{
}

void GitRepository::resetHard()
{}

void GitRepository::clone(const QUrl& remote, const QString& branch)
{
}

void GitRepository::add(const QString& filepath)
{}

void GitRepository::commit(const QString& commitMessage)
{}

void GitRepository::push(const QUrl& remote, const QString& branch)
{}
