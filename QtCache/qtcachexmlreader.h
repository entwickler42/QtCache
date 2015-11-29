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

#include "qtcache_global.h"
#include "qtcachexmlobject.h"
#include <QObject>
#include <QList>

class QIODevice;
class QXmlStreamReader;

namespace QtCacheXml {

class QTCACHESHARED_EXPORT Reader
        : public QObject
{
    Q_OBJECT

public:
    Reader(QObject* parent = 0);
    explicit Reader(const QString& filepath, QObject* parent = 0);
    explicit Reader(QIODevice* io, QObject* parent = 0);
    virtual ~Reader();

    Reader(const Reader& other)
    {
        *this = other;
    }

    Reader& operator = (const Reader& other)
    {
        setParent(other.parent());
        m_time_stamp = other.m_time_stamp;
        m_generator = other.m_generator;
        m_zv = other.m_zv;
        m_version = other.m_version;
        m_objects = other.m_objects;
        return *this;
    }

    Object::List routines(Object::Types types = Object::Types(Object::INT|Object::MAC) ) const
    { return Object::select(m_objects, types); }

    Object::List includes() const
    { return Object::select(m_objects, Object::INC); }

    Object::List classes() const
    { return Object::select(m_objects, Object::CLS); }

    Object::List globals() const
    { return Object::select(m_objects, Object::GBL); }

    void read(QIODevice* io);
    void read(const QString& filepath);

    const QString& timeStamp() const { return m_time_stamp; }
    const QString& generator() const { return m_generator; }
    const QString& zv() const { return m_zv; }
    const QString& version() const { return m_version; }
    const Object::List& objects() const { return m_objects; }

    void setTimeSpamp(const QString& ts) { m_time_stamp = ts;}
    void setGenerator(const QString& generator) { m_time_stamp = generator;}
    void setZv(const QString& zv) { m_time_stamp = zv;}
    void setVersion(const QString& version) { m_time_stamp = version;}

private:
    QString m_time_stamp;
    QString m_generator;
    QString m_zv;
    QString m_version;
    Object::List m_objects;

    static void readRoutineHeaders(QXmlStreamReader* stream, Reader* document);
    static void readClassHeaders(QXmlStreamReader* stream, Reader* document);
    static void loadGlobalHeaders(QXmlStreamReader* stream, Reader* document);
    static void readExportHeaders(QXmlStreamReader* stream, Reader* document);
};

}

#endif // CACHEXMLHEADERS_H
