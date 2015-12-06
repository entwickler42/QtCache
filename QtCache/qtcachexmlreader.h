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

#ifndef CACHEXMLHEADERS_H
#define CACHEXMLHEADERS_H

#include "qtcacheconfig.h"
#include "qtcache_global.h"
#include "qtcachexmlobject.h"
#include <QObject>
#include <QList>

class QIODevice;
class QXmlStreamReader;

QTCACHENAMESPACEBEGIN

class QTCACHESHARED_EXPORT XmlObjectReader
        : public QObject
{
    Q_OBJECT

public:
    XmlObjectReader(QObject* parent = 0);
    explicit XmlObjectReader(const QString& filepath, QObject* parent = 0);
    explicit XmlObjectReader(QIODevice* io, QObject* parent = 0);
    virtual ~XmlObjectReader();

    XmlObjectReader(const XmlObjectReader& other)
    {
        *this = other;
    }

    XmlObjectReader& operator = (const XmlObjectReader& other)
    {
        setParent(other.parent());
        m_time_stamp = other.m_time_stamp;
        m_generator = other.m_generator;
        m_zv = other.m_zv;
        m_version = other.m_version;
        m_objects = other.m_objects;
        return *this;
    }

    XmlObject::List routines(XmlObject::Types types = XmlObject::Types(XmlObject::INT|XmlObject::MAC) ) const
    { return XmlObject::select(m_objects, types); }

    XmlObject::List includes() const
    { return XmlObject::select(m_objects, XmlObject::INC); }

    XmlObject::List classes() const
    { return XmlObject::select(m_objects, XmlObject::CLS); }

    XmlObject::List globals() const
    { return XmlObject::select(m_objects, XmlObject::GBL); }

    void read(QIODevice* io);
    void read(const QString& filepath);

    const QString& timeStamp() const { return m_time_stamp; }
    const QString& generator() const { return m_generator; }
    const QString& zv() const { return m_zv; }
    const QString& version() const { return m_version; }
    const XmlObject::List& objects() const { return m_objects; }

    void setTimeSpamp(const QString& ts) { m_time_stamp = ts;}
    void setGenerator(const QString& generator) { m_time_stamp = generator;}
    void setZv(const QString& zv) { m_time_stamp = zv;}
    void setVersion(const QString& version) { m_time_stamp = version;}

private:
    QString m_time_stamp;
    QString m_generator;
    QString m_zv;
    QString m_version;
    XmlObject::List m_objects;

    static void readRoutineHeaders(QXmlStreamReader* stream, XmlObjectReader* document);
    static void readClassHeaders(QXmlStreamReader* stream, XmlObjectReader* document);
    static void loadGlobalHeaders(QXmlStreamReader* stream, XmlObjectReader* document);
    static void readExportHeaders(QXmlStreamReader* stream, XmlObjectReader* document);
};

QTCACHENAMESPACEEND

#endif // CACHEXMLHEADERS_H
