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

#include "qtcachexmlreader.h"
#include "qtcacheexception.h"
#include "poormanslogger.h"
#include <QFile>
#include <QXmlStreamReader>

using namespace QtCacheXml;

Reader::Reader(QObject* parent)
    : QObject(parent)
{}

Reader::Reader(const QString& filepath, QObject* parent)
    : Reader(parent)
{
    read(filepath);
}

Reader::Reader(QIODevice* io, QObject* parent)
    : Reader(parent)
{
    read(io);
}

Reader::~Reader()
{}

void Reader::readRoutineHeaders(QXmlStreamReader* stream, Reader* document)
{
    Object xobj;
    foreach(const QXmlStreamAttribute &i, stream->attributes()){
        if (i.name().compare("name", Qt::CaseInsensitive) == 0) xobj.setName(i.value().toString());
        else if (i.name().compare("type", Qt::CaseInsensitive) == 0) xobj.setType(Object::fromString(i.value()));
        else if (i.name().compare("timestamp", Qt::CaseInsensitive) == 0){
            xobj.setTimeChanged(i.value().toString());
            xobj.setTimeCreated(document->timeStamp());
        }
    }
    document->m_objects.append(xobj);
}

void Reader::readClassHeaders(QXmlStreamReader* stream, Reader* document)
{
    Object xobj;
    foreach(const QXmlStreamAttribute &i, stream->attributes()){
        if (i.name().compare("name", Qt::CaseInsensitive) == 0) xobj.setName(i.value().toString());
    }
    xobj.setTimeChanged(document->timeStamp());
    xobj.setTimeCreated(document->timeStamp());
    xobj.setType(Object::CLS);
    while(!(stream->isEndElement() && stream->name().compare("class", Qt::CaseInsensitive) == 0)){
        stream->readNext();
        if (stream->isStartElement() && stream->name().compare("timechanged", Qt::CaseInsensitive) == 0){
            xobj.setTimeChanged(stream->readElementText());
        }else if (stream->isStartElement() && stream->name().compare("timecreated", Qt::CaseInsensitive) == 0){
            xobj.setTimeCreated(stream->readElementText());
        }
    }
    document->m_objects.append(xobj);
}

void Reader::loadGlobalHeaders(QXmlStreamReader* stream, Reader* document)
{
    Object xobj;
    xobj.setTimeChanged(document->timeStamp());
    xobj.setTimeCreated(document->timeStamp());
    xobj.setType(Object::GBL);
    while(!stream->atEnd()){
        stream->readNext();
        if (stream->name().compare("sub", Qt::CaseInsensitive) == 0){
            xobj.setName(stream->readElementText());
            document->m_objects.append(xobj);
            break;
        }
    }
}

void Reader::readExportHeaders(QXmlStreamReader* stream, Reader* document)
{
    document->m_time_stamp.clear();
    document->m_generator.clear();
    document->m_zv.clear();
    document->m_version.clear();
    document->m_objects.clear();

    while (!stream->atEnd()){
        stream->readNext();
        if (stream->name().compare("export",Qt::CaseInsensitive) == 0){
            if (stream->isEndElement()) {
                /* end of document */
                return;
            }
            foreach(const QXmlStreamAttribute &i, stream->attributes()){
                if (i.name().compare("generator", Qt::CaseInsensitive) == 0) document->m_generator = i.value().toString();
                else if (i.name().compare("version", Qt::CaseInsensitive) == 0) document->m_version = i.value().toString();
                else if (i.name().compare("zv", Qt::CaseInsensitive) == 0) document->m_zv = i.value().toString();
                else if (i.name().compare("ts", Qt::CaseInsensitive) == 0) document->m_time_stamp = i.value().toString();
            }
        }else if (stream->isStartElement()){
            if(stream->name().compare("routine", Qt::CaseInsensitive) == 0) readRoutineHeaders(stream, document);
            if(stream->name().compare("class", Qt::CaseInsensitive) == 0) readClassHeaders(stream, document);
            if(stream->name().compare("global", Qt::CaseInsensitive) == 0) loadGlobalHeaders(stream, document);
        }
    }
}

void Reader::read(QIODevice* io)
{
    QXmlStreamReader r(io);
    readExportHeaders(&r, this);
}

void Reader::read(const QString& filepath)
{
    QFile f(filepath);
    if (!f.exists()){
        throw QtCacheException(tr("Input file does not exists:\n%1").arg(filepath));
    }
    if (!f.open(QFile::ReadOnly)){
        throw QtCacheException(tr("Can not open input file:\n%1").arg(filepath));
    }
    read(&f);
}
