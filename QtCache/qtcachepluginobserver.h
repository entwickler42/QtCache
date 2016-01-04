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

    void progressBegin(QtCacheProgress& progress);
    void progress(QtCacheProgress& progress);
    void progressEnd(QtCacheProgress& progress);

protected:
    QString name() const { return "QtCachePluginObserver"; }
    QString description() const { return "QtCachePluginObserver"; }

private:
    QList<Plugin*> m_plugins;

    void QtCachePluginObserver::foreachPlugin(QtCacheProgress& progress, void (Plugin::*fn)(QtCacheProgress&));
    template<class T> QList<T*> loadDirectory(const QString& path, QObject* parent);
};

QTCACHENAMESPACEEND

#endif // QTCACHEPLUGINOBSERVER_H
