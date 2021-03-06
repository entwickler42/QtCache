/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef QTCACHEPROGRESS_H
#define QTCACHEPROGRESS_H

#include "qtcache_global.h"
#include <QString>
#include <QVariant>

QTCACHENAMESPACEBEGIN

class QTCACHESHARED_EXPORT Progress
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
        OBJECT_COMPILE,
        BULK_IDLE,
        BULK_READ,
        BULK_UPLOAD,
        BULK_COMPILE,
        BULK_COMPILE_EARLY,
        BULK_SAVE,
        PLUGIN_LOAD,
        USER_DEFINED
    };

    Progress(Type type, QVariant tag, int max = 0, int cur = 0)
        : m_type(type),
          m_tag(tag),
          m_max(max),
          m_cur(cur)
    {}

    Progress(Type type, int max = 0, int cur = 0)
        : Progress(type, QVariant(), max, cur)
    {}

    Progress& operator ()(Type type)
    {
        return this->operator ()(m_max, m_cur, type, m_tag);
    }

    Progress& operator ()(int max, int cur)
    {
        return this->operator ()(max, cur, m_type, m_tag);
    }

    Progress& operator ()(int max, int cur, QVariant tag)
    {
        return this->operator ()(max, cur, m_type, tag);
    }

    Progress& operator ()(int max, int cur, Type type, QVariant tag)
    {
        m_type = type;
        setTag(tag);
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

