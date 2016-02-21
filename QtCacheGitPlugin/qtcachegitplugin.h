#ifndef QTCACHEGITPLUGIN_H
#define QTCACHEGITPLUGIN_H

#include "qtcachegitplugin_global.h"
#include <qtcacheplugin.h>
#include <git2.h>

class QTCACHEGITPLUGINSHARED_EXPORT QtCacheGitPlugin
        : public QtC::Plugin
{
    Q_OBJECT

public:
    QtCacheGitPlugin(QObject* parent);
    ~QtCacheGitPlugin();

    QString name() const { return "QtCacheGitPlugin"; }
    QString description() const { return "Revision control for exported Caché objects using Git"; }
};

extern "C"
QTCACHEGITPLUGINSHARED_EXPORT
QtC::Plugin* createInstance(QObject* parent);

#endif // QTCACHEGITPLUGIN_H
