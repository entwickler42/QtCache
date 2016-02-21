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

    m_loaded_plugins = loadDirectory<Plugin>(plugin_directory_path);
    foreachPlugin(&Plugin::initialize);
}

QtCachePluginObserver::~QtCachePluginObserver()
{
    foreachPlugin(&Plugin::deinitialize);
    foreach(Plugin* i, m_loaded_plugins){
        m_loaded_plugins.removeAll(i);
        delete i;
    }
}

void QtCachePluginObserver::initialize()
{
    foreachPlugin(&Plugin::initialize);
}

void QtCachePluginObserver::deinitialize()
{
    foreachPlugin(&Plugin::deinitialize);
}

void QtCachePluginObserver::progressBegin(Progress& progress)
{
    foreachPlugin(progress, &Plugin::progress);
}

void QtCachePluginObserver::progress(Progress& progress)
{
    foreachPlugin(progress, &Plugin::progress);
}

void QtCachePluginObserver::progressEnd(Progress& progress)
{
    foreachPlugin(progress, &Plugin::progress);
}

void QtCachePluginObserver::parseCommandlineOptionsBegin(QCommandLineParser& commandLineParser)
{
    foreachPlugin(commandLineParser, &Plugin::parseCommandlineOptionsBegin);
}

void QtCachePluginObserver::parseCommandlineOptionsEnd(QCommandLineParser& commandLineParser)
{
    foreachPlugin(commandLineParser, &Plugin::parseCommandlineOptionsEnd);
}

void QtCachePluginObserver::loadApplicationSettingsBegin(QSettings& settings)
{
    foreachPlugin(settings, &Plugin::loadApplicationSettingsBegin);
}

void QtCachePluginObserver::loadApplicationSettingsEnd(QSettings& settings)
{
    foreachPlugin(settings, &Plugin::loadApplicationSettingsEnd);
}

void QtCachePluginObserver::saveApplicationSettingsBegin(QSettings& settings)
{
    foreachPlugin(settings, &Plugin::saveApplicationSettingsBegin);
}

void QtCachePluginObserver::saveApplicationSettingsEnd(QSettings& settings)
{
    foreachPlugin(settings, &Plugin::saveApplicationSettingsEnd);
}

bool QtCachePluginObserver::emitException(std::exception& ex, Plugin* plugin)
{
    bool abort = false;
    emit exception(ex, plugin, abort);
    return abort;
}

void QtCachePluginObserver::foreachPlugin(void (Plugin::*fn)(), ExceptionHandler exceptionHandler)
{
    foreach(Plugin* i, m_loaded_plugins){
        try{
            (i->*fn)();
        }catch(std::exception& ex){
            LOG_EXCEPTION(ex);
            if ((this->*exceptionHandler)(ex, i)){ break; }
        }catch(...){
            LOG_UNKNOWN_EXCEPTION;
            QtCacheException ex("Unknown error");
            if ((this->*exceptionHandler)(ex, i)){ break; }
        }
    }
}

template<class T> void QtCachePluginObserver::foreachPlugin(T& args, void (Plugin::*fn)(T&), ExceptionHandler exceptionHandler)
{
    foreach(Plugin* i, m_loaded_plugins){
        try{
            (i->*fn)(args);
        }catch(std::exception& ex){
            LOG_EXCEPTION(ex);
            if ((this->*exceptionHandler)(ex, i)){ break; }
        }catch(...){
            LOG_UNKNOWN_EXCEPTION;
            QtCacheException ex("Unknown error");
            if ((this->*exceptionHandler)(ex, i)){ break; }
        }
    }
}

template<class T> QList<T*> QtCachePluginObserver::loadDirectory(const QString& path, ExceptionHandler exceptionHandler)
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
                    plugin = (fn)(this->parent());
                    if (NULL != plugin){ ls.push_back(plugin); }
                }
            }catch(std::exception& ex){
                LOG_EXCEPTION(ex);
                if ((this->*exceptionHandler)(ex, plugin)){ break; }
                delete plugin;
            }catch(...){
                LOG_UNKNOWN_EXCEPTION;
                QtCacheException ex("Unknown error");
                if ((this->*exceptionHandler)(ex, plugin)){ break; }
                delete plugin;
            }
        }
    }

    return ls;
}
