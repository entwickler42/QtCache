#ifndef QTCACHEPLUGINOBSERVER_H
#define QTCACHEPLUGINOBSERVER_H

#include <qtcacheplugin.h>
#include "qtcache_global.h"

QTCACHENAMESPACEBEGIN

class QTCACHESHARED_EXPORT QtCachePluginObserver
        : public Plugin
{
    Q_OBJECT

public:
    explicit QtCachePluginObserver(QObject *parent = 0);
    ~QtCachePluginObserver();

    QList<Plugin*> plugins() const { return m_plugins; }

public slots:
    void initialize();
    void deinitialize();

    void progressBegin(Progress& progress) { foreachPlugin(progress, &Plugin::progress); }
    void progress(Progress& progress) { foreachPlugin(progress, &Plugin::progress); }
    void progressEnd(Progress& progress) { foreachPlugin(progress, &Plugin::progress); }

    void bulkProgressBegin(Progress& progress) { foreachPlugin(progress, &Plugin::progress); }
    void bulkProgress(Progress& progress) { foreachPlugin(progress, &Plugin::progress); }
    void bulkProgressEnd(Progress& progress) { foreachPlugin(progress, &Plugin::progress); }

protected:
    QString name() const { return "QtCachePluginObserver"; }
    QString description() const { return "QtCachePluginObserver"; }

private:
    QList<Plugin*> m_plugins;

    void foreachPlugin(Progress& progress, void (Plugin::*fn)(Progress&));
    template<class T> QList<T*> loadDirectory(const QString& path, QObject* parent);
};

QTCACHENAMESPACEEND

#endif // QTCACHEPLUGINOBSERVER_H
