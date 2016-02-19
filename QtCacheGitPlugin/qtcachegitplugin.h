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

    QString name() const { return "QtCacheGITPLUGIN"; }
    QString description() const { return "Just the basic QtCache plugin example"; }

    void initialize()
    {
        PML::LOG << __FUNCTION__;
    }

    void deinitialize()
    {
        PML::LOG << __FUNCTION__;
    }

    void progressBegin(QtC::Progress& progress)
    {
        Q_UNUSED(progress);
        PML::LOG << __FUNCTION__;
    }

    void progress(QtC::Progress& progress)
    {
        Q_UNUSED(progress);
        PML::LOG << __FUNCTION__;
    }

    void progressEnd(QtC::Progress& progress)
    {
        Q_UNUSED(progress);
        PML::LOG << __FUNCTION__;
    }

    void bulkProgressBegin(QtC::Progress& progress)
    {
        Q_UNUSED(progress);
        PML::LOG << __FUNCTION__;
    }

    void bulkProgress(QtC::Progress& progress)
    {
        Q_UNUSED(progress);
        PML::LOG << __FUNCTION__;
    }

    void bulkProgressEnd(QtC::Progress& progress)
    {
        Q_UNUSED(progress);
        PML::LOG << __FUNCTION__;
    }
};

extern "C"
QTCACHEGITPLUGINSHARED_EXPORT
QtC::Plugin* createInstance(QObject* parent);

#endif // QTCACHEGITPLUGIN_H
