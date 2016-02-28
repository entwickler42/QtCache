#include "qtcachebulkaction.h"
#include "qtcacheplugindirector.h"

QTCACHENAMESPACEUSE

BulkAction::BulkAction(QtCache* cache, QObject *parent)
    : ProgressReporter(parent),
      m_cache(cache)
{
    if (NULL == m_cache) throw new std::invalid_argument("QtCache* cache must not be NULL");
}
