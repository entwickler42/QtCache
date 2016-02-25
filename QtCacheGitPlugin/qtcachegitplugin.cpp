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
    m_repo = NULL;
    m_branchname_local = "test";
    m_branchname_remote = "master";
    m_origin_url = "d:\\src\\QtCache\\build\\remote\\";
}

QtCacheGitPlugin::~QtCacheGitPlugin()
{
    delete m_repo;
}

void QtCacheGitPlugin::progressBegin(QtC::Progress& p)
{
    if (p.type() == QtC::Progress::BULK_SAVE){
        QString dirpath = p.tag().toString();
        delete m_repo;
        m_repo = new GitRepository(dirpath);

        try{
            m_repo->open();
            m_repo->branch(m_branchname_local);
        }catch(...){
            try{
                if (m_origin_url.length() > 0) {
                    m_repo->clone(m_origin_url);
                }else{
                    m_repo->init();
                }
                m_repo->branch(m_branchname_local);
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
        if (m_repo){
            m_repo->add(filepath);
        }
    }
}

void QtCacheGitPlugin::progressEnd(QtC::Progress& p)
{
    if (p.type() == QtC::Progress::BULK_SAVE){
        if (m_repo){
            m_repo->commit("Auto commit");
            if (!m_origin_url.isEmpty()){
                m_repo->push();
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
