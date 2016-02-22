#ifndef QTCACHEGITPLUGIN_H
#define QTCACHEGITPLUGIN_H

#include "qtcachegitplugin_global.h"
#include <qtcacheplugin.h>

class QCommandLineParser;
class QSettings;
class QtC::Progress;

class QTCACHEGITPLUGINSHARED_EXPORT QtCacheGitPlugin
        : public QtC::Plugin
{
    Q_OBJECT

public:
    QtCacheGitPlugin(QObject* parent);
    ~QtCacheGitPlugin();

    QString name() const { return "QtCacheGitPlugin"; }
    QString description() const { return "Revision control for exported Caché objects using Git"; }

    void progressBegin(QtC::Progress&);
    void progress(QtC::Progress&);
    void progressEnd(QtC::Progress&);

    void parseCommandlineOptionsBegin(QCommandLineParser&);
    void parseCommandlineOptionsEnd(QCommandLineParser&);

    void loadApplicationSettingsEnd(QSettings&);

private:
    QString m_branchname_local;
    QString m_branchname_remote;
    QString m_origin_url;
};

extern "C"
QTCACHEGITPLUGINSHARED_EXPORT
QtC::Plugin* createInstance(QObject* parent);

#endif // QTCACHEGITPLUGIN_H
