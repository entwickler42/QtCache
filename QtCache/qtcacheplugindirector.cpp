#include "qtcacheplugindirector.h"
#include <QCoreApplication>
#include <QLibrary>
#include <QDir>

QTCACHENAMESPACEUSE

PluginDirector::PluginDirector(QObject *parent)
    : Plugin(parent)
{
    QString plugin_directory_path = QDir::cleanPath(
                QCoreApplication::applicationDirPath()
                + QDir::separator()
                + "plugins");

    m_loaded_plugins = loadDirectory<Plugin>(plugin_directory_path);
    foreachPlugin(&Plugin::initialize);
}

PluginDirector::~PluginDirector()
{
    foreachPlugin(&Plugin::deinitialize);
    foreach(Plugin* i, m_loaded_plugins){
        m_loaded_plugins.removeAll(i);
        delete i;
    }
}

void PluginDirector::initialize()
{
    foreachPlugin(&Plugin::initialize);
}

void PluginDirector::deinitialize()
{
    foreachPlugin(&Plugin::deinitialize);
}

void PluginDirector::progressBegin(Progress& progress)
{
    foreachPlugin(progress, &Plugin::progressBegin);
}

void PluginDirector::progress(Progress& progress)
{
    foreachPlugin(progress, &Plugin::progress);
}

void PluginDirector::progressEnd(Progress& progress)
{
    foreachPlugin(progress, &Plugin::progressEnd);
}

void PluginDirector::parseCommandlineOptionsBegin(QCommandLineParser& commandLineParser)
{
    foreachPlugin(commandLineParser, &Plugin::parseCommandlineOptionsBegin);
}

void PluginDirector::parseCommandlineOptionsEnd(QCommandLineParser& commandLineParser)
{
    foreachPlugin(commandLineParser, &Plugin::parseCommandlineOptionsEnd);
}

void PluginDirector::loadApplicationSettingsBegin(QSettings& settings)
{
    foreachPlugin(settings, &Plugin::loadApplicationSettingsBegin);
}

void PluginDirector::loadApplicationSettingsEnd(QSettings& settings)
{
    foreachPlugin(settings, &Plugin::loadApplicationSettingsEnd);
}

void PluginDirector::saveApplicationSettingsBegin(QSettings& settings)
{
    foreachPlugin(settings, &Plugin::saveApplicationSettingsBegin);
}

void PluginDirector::saveApplicationSettingsEnd(QSettings& settings)
{
    foreachPlugin(settings, &Plugin::saveApplicationSettingsEnd);
}

bool PluginDirector::emitException(std::exception& ex, Plugin* plugin)
{
    bool abort = false;
    emit exception(ex, plugin, abort);
    return abort;
}

void PluginDirector::foreachPlugin(void (Plugin::*fn)(), ExceptionHandler exceptionHandler)
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

template<class T> void PluginDirector::foreachPlugin(T& args, void (Plugin::*fn)(T&), ExceptionHandler exceptionHandler)
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

template<class T> QList<T*> PluginDirector::loadDirectory(const QString& path, ExceptionHandler exceptionHandler)
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
