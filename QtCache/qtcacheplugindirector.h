#ifndef QTCACHEPLUGINDIRECTOR_H
#define QTCACHEPLUGINDIRECTOR_H

#include "qtcache_global.h"
#include "qtcacheexception.h"
#include <qtcacheplugin.h>

QTCACHENAMESPACEBEGIN

class QTCACHESHARED_EXPORT PluginDirector
        : public Plugin
{
    Q_OBJECT

public:
    explicit PluginDirector(QObject *parent = 0);
    ~PluginDirector();

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
    typedef bool (PluginDirector::*ExceptionHandler)(std::exception&, Plugin* plugin);

    QList<Plugin*> m_loaded_plugins;

    void initialize();
    void deinitialize();

    bool emitException(std::exception& ex, Plugin* plugin);

    void foreachPlugin(void (Plugin::*fn)(), ExceptionHandler exceptionHandler = &PluginDirector::emitException);
    template<class T> void foreachPlugin(T& args, void (Plugin::*fn)(T&), ExceptionHandler exceptionHandler = &PluginDirector::emitException);

    template<class T> QList<T*> loadDirectory(const QString& path, ExceptionHandler exceptionHandler = &PluginDirector::emitException);
};

QTCACHENAMESPACEEND

#endif // QTCACHEPLUGINDIRECTOR_H
