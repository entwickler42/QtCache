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

    void QtCachePluginObserver::foreachPlugin(QtCacheProgress& progress, void (*fn)(Plugin*, QtCacheProgress&));
    template<class T> QList<T*> loadDirectory(const QString& path, QObject* parent);
};

QTCACHENAMESPACEEND

#endif // QTCACHEPLUGINOBSERVER_H
