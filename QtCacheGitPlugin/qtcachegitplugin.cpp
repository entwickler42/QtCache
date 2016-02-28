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
    : QtC::Plugin(parent),
      m_origin_url(""),
      m_branchname_local(""),
      m_branchname_remote("master"),
      m_commit_message("QtCache export entry"),
      m_repo(NULL)
{}

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
            if (!m_branchname_local.isEmpty()) {
                m_repo->branch(m_branchname_local);
            }
        }catch(...){
            try{
                if (m_origin_url.length() > 0) {
                    m_repo->clone(m_origin_url);
                }else{
                    m_repo->init();
                }
                if (!m_branchname_local.isEmpty()) {
                    m_repo->branch(m_branchname_local);
                }
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
            m_repo->commit(m_commit_message);
            if (!m_origin_url.isEmpty()){
                m_repo->push();
                m_repo->fetch();
            }
        }
    }
}

void QtCacheGitPlugin::parseCommandlineOptionsBegin(QCommandLineParser& p)
{
    QCommandLineOption origin_url("originurl", tr("clone, use and push remote repositry"), "url", "");
    p.addOption(origin_url);

    QCommandLineOption local_branch("localbranch", tr("target branch name"), "name", "");
    p.addOption(local_branch);

    QCommandLineOption remote_branch("remotebranch", tr("remote branch name"), "name", "");
    p.addOption(remote_branch);

    QCommandLineOption commit_message("commitmessage", tr("use this message for commits"), "message", "");
    p.addOption(commit_message);
}

void QtCacheGitPlugin::parseCommandlineOptionsEnd(QCommandLineParser& p)
{
    if(p.isSet("originurl")){
        m_origin_url = p.value("originurl");
    }
    if(p.isSet("localbranch")){
        m_branchname_local = p.value("localbranch");
    }
    if(p.isSet("remotebranch")){
        m_branchname_remote = p.value("remotebranch");
    }
    if(p.isSet("commitmessage")){
        m_commit_message = p.value("commitmessage");
    }
}

void QtCacheGitPlugin::loadApplicationSettingsEnd(QSettings& conf)
{
    conf.beginGroup("GitPlugin");
    m_origin_url = conf.value("OriginUrl", "").toString();
    m_branchname_remote = conf.value("RemoteBranch", "").toString();
    m_branchname_local = conf.value("LocalBranch", "").toString();
    m_commit_message = conf.value("CommitMessage", "QtCacheTool export commit").toString();
    conf.endGroup();
}

void QtCacheGitPlugin::saveApplicationSettingsEnd(QSettings& conf)
{
    conf.beginGroup("GitPlugin");
    conf.setValue("OriginUrl", m_origin_url);
    conf.setValue("RemoteBranch", m_branchname_remote);
    conf.setValue("LocalBranch", m_branchname_local);
    conf.setValue("CommitMessage", m_commit_message);
    conf.endGroup();
}
