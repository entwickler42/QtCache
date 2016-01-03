#include "qtcachepluginobserver.h"
#include <QCoreApplication>
#include <QLibrary>
#include <QDir>

QTCACHENAMESPACEUSE

QtCachePluginObserver::QtCachePluginObserver(QObject *parent)
    : Plugin(parent)
{
    QString plugin_directory_path = QDir::cleanPath(
                QCoreApplication::applicationDirPath()
                + QDir::separator()
                + "plugins");

    m_plugins = loadDirectory<Plugin>(plugin_directory_path, this);
    foreach(Plugin* i, m_plugins){
        try {
            i->initialize();
        }catch(std::exception& ex){
            LOG_EXCEPTION(ex)
        }catch(...){
            LOG_UNKNOWN_EXCEPTION
        }
        m_plugins.removeAll(i);
        delete i;
    }
}

QtCachePluginObserver::~QtCachePluginObserver()
{
    foreach(Plugin* i, m_plugins){
        try {
            i->deinitialize();
        }catch(std::exception& ex){
            LOG_EXCEPTION(ex)
        }catch(...){
            LOG_UNKNOWN_EXCEPTION
        }
        m_plugins.removeAll(i);
        delete i;
    }
}

void QtCachePluginObserver::initialize()
{}

void QtCachePluginObserver::deinitialize()
{}

void QtCachePluginObserver::progressBegin(QtCacheProgress& progress)
{
    foreachPlugin(progress, [](Plugin* p, QtCacheProgress& pp) { p->progressBegin(pp); });
}

void QtCachePluginObserver::progress(QtCacheProgress& progress)
{
    foreachPlugin(progress, [](Plugin* p, QtCacheProgress& pp) { p->progress(pp); });
}

void QtCachePluginObserver::progressEnd(QtCacheProgress& progress)
{
    foreachPlugin(progress, [](Plugin* p, QtCacheProgress& pp) { p->progressEnd(pp); });
}

void QtCachePluginObserver::foreachPlugin(QtCacheProgress& progress, void (*fn)(Plugin*, QtCacheProgress&))
{
    foreach(Plugin* i, m_plugins){
        fn(i, progress);
        if (progress.isAborted()) { break; }
    }
}

template<class T> QList<T*> QtCachePluginObserver::loadDirectory(const QString& path, QObject* parent)
{
    QList<T*> ls;

    QDir plugin_dir(path);
    if (plugin_dir.exists()){
        foreach(const QString& plugin_file, plugin_dir.entryList(QDir::Files|QDir::NoDotAndDotDot)){
            T* plugin = NULL;
            try{
                Plugin::FactoryFunction fn = (Plugin::FactoryFunction)QLibrary::resolve(
                            plugin_dir.absoluteFilePath(plugin_file),
                            Plugin::FactoryFunctionSymbol);
                if(NULL != fn){
                    plugin = (fn)(parent);
                    if (NULL != plugin){ ls.push_back(plugin); }
                }
            }catch(std::exception& ex){
                delete plugin;
                LOG_EXCEPTION(ex)
            }catch(...){
                delete plugin;
                LOG_UNKNOWN_EXCEPTION
            }
        }
    }

    return ls;
}
