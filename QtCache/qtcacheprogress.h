#ifndef QTCACHEPROGRESS_H
#define QTCACHEPROGRESS_H

#include "qtcache_global.h"
#include <QString>
#include <QVariant>

QTCACHENAMESPACEBEGIN

class QTCACHESHARED_EXPORT QtCacheProgress
{
public:
    enum Type
    {
        UNKNOWN,
        KEEP_CURRENT_TYPE,
        CONNECT,
        DISCONNECT,
        QUERY_NS,
        QUERY_OBJECTS,
        XMLFILE_IMPORT,
        XMLFILE_EXPORT,
        OBJECT_COMPILE
    };

    QtCacheProgress(Type type, QVariant tag, int max = 0, int cur = 0)
        : m_type(type),
          m_tag(tag),
          m_max(max),
          m_cur(cur)
    {}

    QtCacheProgress(Type type, int max = 0, int cur = 0)
        : QtCacheProgress(type, 0, max, cur)
    {}

    QtCacheProgress& operator ()(int max, int cur, Type type = KEEP_CURRENT_TYPE)
    {
        if (type != KEEP_CURRENT_TYPE){
            m_type = type;
        }
        setMaximum(max);
        setPosition(cur);
        return *this;
    }

    Type type() const { return m_type; }

    void abort() { m_abort = true; }
    bool isAborted() const { return m_abort; }
    float percent() const
    {
        if (m_max == 0) return 0;
        return 100.0f * m_cur / m_max;
    }
    void setTag(const QVariant& tag) { m_tag = tag; }
    const QVariant& tag() const { return m_tag; }
    void setPosition(int value) { m_cur = value; }
    void setMaximum(int value) { m_max = value; }

private:
    bool m_abort = false;
    Type m_type = UNKNOWN;
    QVariant m_tag;

    int m_max = 0;
    int m_cur = 0;
};

QTCACHENAMESPACEEND

#endif // QTCACHEPROGRESS

