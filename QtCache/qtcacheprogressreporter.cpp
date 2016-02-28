#include "qtcacheprogressreporter.h"
#include "qtcacheplugindirector.h"

QTCACHENAMESPACEUSE

ProgressReporter::ProgressReporter(QObject* parent)
    : QObject(parent),
    m_current_progress(Progress::UNKNOWN, 0, 0)
{}

ProgressReporter::~ProgressReporter()
{}

void ProgressReporter::reportError(std::exception& ex, Progress& p)
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

void ProgressReporter::emitError(std::exception& ex, Progress& p)
{
    emit error(ex, p);
}

static void delegateToPluginDirector(void (PluginDirector::*method)(Progress&), Progress& p)
{
    PluginDirector* plugins = QtCache::instance()->plugins();
    if (plugins){ (plugins->*method)(p); }
}

void ProgressReporter::emitProgressBegin(Progress& p)
{
    delegateToPluginDirector(&PluginDirector::progressBegin, p);
    emit progressBegin(p);
}

void ProgressReporter::emitProgress(Progress& p)
{
    delegateToPluginDirector(&PluginDirector::progress, p);
    emit progress(p);
}

void ProgressReporter::emitProgressEnd(Progress& p)
{
    delegateToPluginDirector(&PluginDirector::progressEnd, p);
    emit progressEnd(p);
}
