#ifndef QTCACHEPROGRESS_H
#define QTCACHEPROGRESS_H

#include "qtcache_global.h"
#include <QString>

QTCACHENAMESPACEBEGIN

class QtCacheProgress
{
public:
    enum Type
    {
        UNKNOWN,
        CONNECT,
        DISCONNECT,
        QUERY_NS,
        QUERY_OBJECTS,
        XMLFILE_IMPORT,
        XMLFILE_EXPORT,
        OBJECT_COMPILE
    };

    QtCacheProgress(Type type, int max = 0, int cur = 0)
        : m_type(type),
          m_max(max),
          m_cur(cur)
    {}

    Type type() const { return m_type; }

    void abort() { m_abort = true; }
    bool isAborted() const { return m_abort; }

    float percent() const
    {
        if (m_max == 0) return 0;
        return 100.0f * m_cur / m_max;
    }

private:
    bool m_abort = false;
    Type m_type = UNKNOWN;

    int m_max = 0;
    int m_cur = 0;
};

QTCACHENAMESPACEEND

#endif // QTCACHEPROGRESS

