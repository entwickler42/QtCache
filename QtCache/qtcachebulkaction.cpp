#include "qtcachebulkaction.h"
#include "qtcachepluginobserver.h"

QTCACHENAMESPACEUSE

BulkAction::BulkAction(QtCache* cache, QObject *parent)
    : QObject(parent),
      m_cache(cache),
      m_current_progress(Progress::BULK_IDLE, 0, 0)
{
    if (NULL == m_cache) throw new std::invalid_argument("QtCache* cache must not be NULL");
}

void BulkAction::reportProcessBegin(Progress& p)
{
    m_cache->plugins()->progressBegin(p);
    setAbort(p.isAborted());
}

void BulkAction::reportProgress(Progress& p)
{
    m_current_progress = p;
    m_cache->plugins()->progress(p);
    setAbort(p.isAborted());
    emit progress(p);
}

void BulkAction::reportProcessEnd(Progress& p)
{
    m_cache->plugins()->progressEnd(p);
    setAbort(p.isAborted());
}
