#ifndef QTCACHEPLUGINOBSERVER_H
#define QTCACHEPLUGINOBSERVER_H

#include "qtcache_global.h"
#include "qtcacheexception.h"
#include <qtcacheplugin.h>

QTCACHENAMESPACEBEGIN

class QTCACHESHARED_EXPORT QtCachePluginObserver
        : public Plugin
{
    Q_OBJECT

public:
    explicit QtCachePluginObserver(QObject *parent = 0);
    ~QtCachePluginObserver();

    const QList<Plugin*>& loaded() const { return m_loaded_plugins; }
    QList<Plugin*>& loaded() { return m_loaded_plugins; }

signals:
    void exception(std::exception& ex, Plugin* Plugin, bool& abort);

public slots:
    void progressBegin(Progress& progress);
    void progress(Progress& progress);
    void progressEnd(Progress& progress);
    void parseCommandlineOptionsBegin(QCommandLineParser& commandLineParser);
    void parseCommandlineOptionsEnd(QCommandLineParser& commandLineParser);
    void loadApplicationSettingsBegin(QSettings& settings);
    void loadApplicationSettingsEnd(QSettings& settings);
    void saveApplicationSettingsBegin(QSettings& settings);
    void saveApplicationSettingsEnd(QSettings& settings);

protected:
    QString name() const { return "QtCachePluginObserver"; }
    QString description() const { return "QtCachePluginObserver"; }

private:
    typedef bool (QtCachePluginObserver::*ExceptionHandler)(std::exception&, Plugin* plugin);

    QList<Plugin*> m_loaded_plugins;

    void initialize();
    void deinitialize();

    bool emitException(std::exception& ex, Plugin* plugin);

    void foreachPlugin(void (Plugin::*fn)(), ExceptionHandler exceptionHandler = &QtCachePluginObserver::emitException);
    template<class T> void foreachPlugin(T& args, void (Plugin::*fn)(T&), ExceptionHandler exceptionHandler = &QtCachePluginObserver::emitException);

    template<class T> QList<T*> loadDirectory(const QString& path, ExceptionHandler exceptionHandler = &QtCachePluginObserver::emitException);
};

QTCACHENAMESPACEEND

#endif // QTCACHEPLUGINOBSERVER_H
