#include "qtcachepluginsample.h"

QtC::Plugin* createInstance(QObject* parent)
{
    return QtC::createPluginInstance<QtCachePluginSample>(parent);
}

QtCachePluginSample::QtCachePluginSample(QObject* parent)
    : QtC::Plugin(parent)
{
}

QtCachePluginSample::~QtCachePluginSample()
{
}

