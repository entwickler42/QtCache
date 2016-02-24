#include "qtcachegitplugin.h"
#include "gitrepository.h"
#include <QCommandLineParser>
#include <QSettings>
#include <qtcacheprogress.h>

QtC::Plugin* createInstance(QObject* parent)
{
    return QtC::createPluginInstance<QtCacheGitPlugin>(parent);
}

QtCacheGitPlugin::QtCacheGitPlugin(QObject* parent)
    : QtC::Plugin(parent)
{
    m_branchname_local = "test";
    m_branchname_remote = "master";
    m_origin_url = "";
}

QtCacheGitPlugin::~QtCacheGitPlugin()
{}

void QtCacheGitPlugin::progressBegin(QtC::Progress& p)
{
    if (p.type() == QtC::Progress::BULK_SAVE){
        QString dirpath = p.tag().toString();
        GitRepository repo(dirpath);

        try{
            repo.open();
            repo.branch(m_branchname_local);
        }catch(...){
            try{
                if (m_origin_url.length() > 0) {
                    repo.clone(m_origin_url);
                }else{
                    repo.init();
                }
                repo.branch(m_branchname_local);
            }catch(...){
                throw;
            }
        }
    }
}

void QtCacheGitPlugin::progress(QtC::Progress& p)
{
    if (p.type() == QtC::Progress::BULK_SAVE){
        QStringList tags = p.tag().toStringList();
        QString dirpath = tags.at(0);
        QString filepath = QDir(dirpath).relativeFilePath(tags.at(1));
        GitRepository repo(dirpath);

        repo.add(filepath);
    }
}

void QtCacheGitPlugin::progressEnd(QtC::Progress& p)
{
    if (p.type() == QtC::Progress::BULK_SAVE){
        QString dirpath = p.tag().toString();
        GitRepository repo(dirpath);

        repo.commit("Auto commit");
        if (!m_origin_url.isEmpty()){
            repo.push(m_origin_url);
        }
    }
}

void QtCacheGitPlugin::parseCommandlineOptionsBegin(QCommandLineParser&)
{}

void QtCacheGitPlugin::parseCommandlineOptionsEnd(QCommandLineParser&)
{}

void QtCacheGitPlugin::loadApplicationSettingsEnd(QSettings&)
{}
