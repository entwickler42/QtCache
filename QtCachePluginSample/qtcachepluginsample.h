#ifndef QTCACHEPLUGINSAMPLE_H
#define QTCACHEPLUGINSAMPLE_H

#include "qtcachepluginsample_global.h"
#include <qtcacheplugin.h>

class QTCACHEPLUGINSAMPLESHARED_EXPORT QtCachePluginSample
        : public QtC::Plugin
{
    Q_OBJECT

public:
    QtCachePluginSample(QObject* parent);
    ~QtCachePluginSample();

    QString name() const { return "QtCachePluginSample"; }
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
};

extern "C"
QTCACHEPLUGINSAMPLESHARED_EXPORT
QtC::Plugin* createInstance(QObject* parent);

#endif // QTCACHEPLUGINSAMPLE_H
