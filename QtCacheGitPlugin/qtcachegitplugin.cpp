#include "qtcachegitplugin.h"

QtC::Plugin* createInstance(QObject* parent)
{
    return QtC::createPluginInstance<QtCacheGitPlugin>(parent);
}

QtCacheGitPlugin::QtCacheGitPlugin(QObject* parent)
    : QtC::Plugin(parent)
{
}

QtCacheGitPlugin::~QtCacheGitPlugin()
{
}

