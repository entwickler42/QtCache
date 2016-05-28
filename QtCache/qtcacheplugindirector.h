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

public slots:
    void onError(const std::exception&, Progress&);
    void onProgressBegin(Progress&);
    void onProgress(Progress&);
    void onProgressEnd(Progress&);
    void onParseCommandlineOptionsBegin(QCommandLineParser&);
    void onParseCommandlineOptionsEnd(QCommandLineParser&);
    void onLoadApplicationSettingsBegin(QSettings&);
    void onLoadApplicationSettingsEnd(QSettings&);
    void onSaveApplicationSettingsBegin(QSettings&);
    void onSaveApplicationSettingsEnd(QSettings&);

protected:
    QString name() const { return "QtCachePluginObserver"; }
    QString description() const { return "QtCachePluginObserver"; }

private:
    QList<Plugin*> m_loaded_plugins;

    void registerPlugin(Plugin* plugin);

    void onInitialize();
    void onDeinitialize();

    void foreachPlugin(void (Plugin::*fn)());
    template<class T> void foreachPlugin(T& args, void (Plugin::*fn)(T&));

    template<class T> QList<T*> loadDirectory(const QString& path);
};

QTCACHENAMESPACEEND

#endif // QTCACHEPLUGINDIRECTOR_H
