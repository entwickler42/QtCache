#ifndef QTCACHEGITPLUGIN_H
#define QTCACHEGITPLUGIN_H

#include "qtcachegitplugin_global.h"
#include <qtcacheplugin.h>

class QCommandLineParser;
class QSettings;
class QtC::Progress;
class GitRepository;

class QTCACHEGITPLUGINSHARED_EXPORT QtCacheGitPlugin
        : public QtC::Plugin
{
    Q_OBJECT

public:
    QtCacheGitPlugin(QObject* parent);
    ~QtCacheGitPlugin();

    QString name() const { return "QtCacheGitPlugin"; }
    QString description() const { return "Revision control for exported Caché objects using Git"; }

    void onProgressBegin(QtC::Progress&);
    void onProgress(QtC::Progress&);
    void onProgressEnd(QtC::Progress&);

    void onParseCommandlineOptionsBegin(QCommandLineParser&);
    void onParseCommandlineOptionsEnd(QCommandLineParser&);

    void onSaveApplicationSettingsEnd(QSettings&);
    void onLoadApplicationSettingsEnd(QSettings&);

private:
    QString m_origin_url;
    QString m_branchname_local;
    QString m_branchname_remote;
    QString m_commit_message;
    QString m_commit_author;
    QString m_commit_email;
    GitRepository* m_repo;
};

extern "C"
QTCACHEGITPLUGINSHARED_EXPORT
QtC::Plugin* createInstance(QObject* parent);

#endif // QTCACHEGITPLUGIN_H
