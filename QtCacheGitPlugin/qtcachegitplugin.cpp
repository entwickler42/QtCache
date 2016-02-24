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
    m_branchname_local = "master";
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

        if (repo.isValid()){
            repo.resetHard();
        }else{
            if (m_origin_url.isEmpty()){
                repo.init();
                repo.branch(m_branchname_local);
            }else{
                repo.clone(m_origin_url, m_branchname_remote);
            }
        }
    }
}

void QtCacheGitPlugin::progress(QtC::Progress& p)
{
    if (p.type() == QtC::Progress::BULK_SAVE){
        QStringList tags = p.tag().toStringList();
        QString filepath = QDir(tags.at(0)).absoluteFilePath(tags.at(1));
        GitRepository repo(filepath);

        if (repo.isValid()){
            repo.add(filepath);
        }
    }
}

void QtCacheGitPlugin::progressEnd(QtC::Progress& p)
{
    if (p.type() == QtC::Progress::BULK_SAVE){
        QString dirpath = p.tag().toString();
        GitRepository repo(dirpath);

        if (repo.isValid()){
            repo.add(dirpath);
            repo.commit("Auto commit");
            if (!m_origin_url.isEmpty()){
                repo.push(m_origin_url);
            }
        }
    }
}

void QtCacheGitPlugin::parseCommandlineOptionsBegin(QCommandLineParser&)
{}

void QtCacheGitPlugin::parseCommandlineOptionsEnd(QCommandLineParser&)
{}

void QtCacheGitPlugin::loadApplicationSettingsEnd(QSettings&)
{}
