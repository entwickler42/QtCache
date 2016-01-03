#ifndef QTCACHEPLUGIN_H
#define QTCACHEPLUGIN_H

#include "qtcache_global.h"
#include <qtcache.h>
#include <QObject>

QTCACHENAMESPACEBEGIN

class QTCACHESHARED_EXPORT Plugin
    : public QObject
{
    Q_OBJECT

public:
    typedef QtC::Plugin*(*FactoryFunction)(QObject*);
    static const char* FactoryFunctionSymbol;

    explicit Plugin(QObject *parent = 0);

    QtCache* cache() const
    {
        return QtCache::instance();
    }

    virtual QString name() const = 0;
    virtual QString description() const { return ""; }
    virtual int version() const { return 0x000000; }

    virtual void initialize() {}
    virtual void deinitialize() {}

    virtual void progressBegin(QtCacheProgress& progress) = 0;
    virtual void progress(QtCacheProgress& progress) = 0;
    virtual void progressEnd(QtCacheProgress& progress) = 0;
};

template<class T>
static T* createPluginInstance(QObject* parent)
{
    return new T(parent);
}

QTCACHENAMESPACEEND

#endif // QTCACHEPLUGIN_H
