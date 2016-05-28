#include "qtcacheprogressreporter.h"
#include "qtcacheplugindirector.h"

QTCACHENAMESPACEUSE

ProgressReporter::ProgressReporter(QObject* parent)
    : QObject(parent),
      m_current_progress(Progress::UNKNOWN, 0, 0)
{}

ProgressReporter::~ProgressReporter()
{}

void ProgressReporter::reportError(const std::exception& ex, Progress& p)
{
    m_current_progress = p;
    emitError(ex, p);
}

void ProgressReporter::reportProcessBegin(Progress& p)
{
    m_current_progress = p;
    emitProgressBegin(p);
}

void ProgressReporter::reportProgress(Progress& p)
{
    m_current_progress = p;
    emitProgress(p);
}

void ProgressReporter::reportProcessEnd(Progress& p)
{
    m_current_progress = p;
    emitProgressEnd(p);
}

template <class T> void ProgressReporter::forwardCallToPluginDirector(void (T::*method)(Progress&), Progress& p)
{
    T* plugins = QtCache::instance()->plugins();
    if (plugins){ (plugins->*method)(p); }
}

void ProgressReporter::emitError(const std::exception& ex, Progress& p)
{
    PluginDirector* plugins = QtCache::instance()->plugins();
    if (plugins){ plugins->onError(ex, p); }
    emit error(ex, p);
}

void ProgressReporter::emitProgressBegin(Progress& p)
{
    forwardCallToPluginDirector(&PluginDirector::onProgressBegin, p);
    emit progressBegin(p);
}

void ProgressReporter::emitProgress(Progress& p)
{
    forwardCallToPluginDirector(&PluginDirector::onProgress, p);
    emit progress(p);
}

void ProgressReporter::emitProgressEnd(Progress& p)
{
    forwardCallToPluginDirector(&PluginDirector::onProgressEnd, p);
    emit progressEnd(p);
}
