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

    QList<Plugin*> ls = loadDirectory<Plugin>(plugin_directory_path);
    foreach(Plugin* i, ls){ this->registerPlugin(i); }
    foreachPlugin(&Plugin::onInitialize);
}

PluginDirector::~PluginDirector()
{
    foreachPlugin(&Plugin::onDeinitialize);
    foreach(Plugin* i, m_loaded_plugins){
        m_loaded_plugins.removeAll(i);
        delete i;
    }
}

void PluginDirector::onInitialize()
{
    foreachPlugin(&Plugin::onInitialize);
}

void PluginDirector::onDeinitialize()
{
    foreachPlugin(&Plugin::onDeinitialize);
}

void PluginDirector::onError(const std::exception& ex, Progress& progress)
{
    foreach(Plugin* i, m_loaded_plugins){
        try{
            i->onError(ex, progress);
        }catch(std::exception& ex){
            LOG_EXCEPTION(ex);
            this->reportError(ex, currentProgress());
            if (currentProgress().isAborted()) { break; }
        }catch(...){
            LOG_UNKNOWN_EXCEPTION;
            QtCacheException ex("Unknown error");
            this->reportError(ex, currentProgress());
            if (currentProgress().isAborted()) { break; }
        }
    }
}

void PluginDirector::onProgressBegin(Progress& progress)
{
    foreachPlugin(progress, &Plugin::onProgressBegin);
}

void PluginDirector::onProgress(Progress& progress)
{
    foreachPlugin(progress, &Plugin::onProgress);
}

void PluginDirector::onProgressEnd(Progress& progress)
{
    foreachPlugin(progress, &Plugin::onProgressEnd);
}

void PluginDirector::onParseCommandlineOptionsBegin(QCommandLineParser& commandLineParser)
{
    foreachPlugin(commandLineParser, &Plugin::onParseCommandlineOptionsBegin);
}

void PluginDirector::onParseCommandlineOptionsEnd(QCommandLineParser& commandLineParser)
{
    foreachPlugin(commandLineParser, &Plugin::onParseCommandlineOptionsEnd);
}

void PluginDirector::onLoadApplicationSettingsBegin(QSettings& settings)
{
    foreachPlugin(settings, &Plugin::onLoadApplicationSettingsBegin);
}

void PluginDirector::onLoadApplicationSettingsEnd(QSettings& settings)
{
    foreachPlugin(settings, &Plugin::onLoadApplicationSettingsEnd);
}

void PluginDirector::onSaveApplicationSettingsBegin(QSettings& settings)
{
    foreachPlugin(settings, &Plugin::onSaveApplicationSettingsBegin);
}

void PluginDirector::onSaveApplicationSettingsEnd(QSettings& settings)
{
    foreachPlugin(settings, &Plugin::onSaveApplicationSettingsEnd);
}

void PluginDirector::foreachPlugin(void (Plugin::*fn)())
{
    foreach(Plugin* i, m_loaded_plugins){
        try{
            (i->*fn)();
        }catch(std::exception& ex){
            LOG_EXCEPTION(ex);
            this->reportError(ex, currentProgress());
            if (currentProgress().isAborted()) { break; }
        }catch(...){
            LOG_UNKNOWN_EXCEPTION;
            QtCacheException ex("Unknown error");
            this->reportError(ex, currentProgress());
            if (currentProgress().isAborted()) { break; }
        }
    }
}

template<class T> void PluginDirector::foreachPlugin(T& args, void (Plugin::*fn)(T&))
{
    foreach(Plugin* i, m_loaded_plugins){
        try{
            (i->*fn)(args);
        }catch(std::exception& ex){
            LOG_EXCEPTION(ex);
            this->reportError(ex, currentProgress());
            if (currentProgress().isAborted()) { break; }
        }catch(...){
            LOG_UNKNOWN_EXCEPTION;
            QtCacheException ex("Unknown error");
            this->reportError(ex, currentProgress());
            if (currentProgress().isAborted()) { break; }
        }
    }
}

void PluginDirector::registerPlugin(Plugin* plugin)
{
    link(plugin);
    plugin->setParent(this);
    m_loaded_plugins.append(plugin);
}

template<class T> QList<T*> PluginDirector::loadDirectory(const QString& path)
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
                    plugin = (fn)(this);
                    if (NULL != plugin){ ls.push_back(plugin); }
                }
            }catch(std::exception& ex){
                LOG_EXCEPTION(ex);
                this->reportError(ex, currentProgress());
                if (currentProgress().isAborted()) { break; }
                delete plugin;
            }catch(...){
                LOG_UNKNOWN_EXCEPTION;
                QtCacheException ex("Unknown error");
                this->reportError(ex, currentProgress());
                if (currentProgress().isAborted()) { break; }
                delete plugin;
            }
        }
    }

    return ls;
}
