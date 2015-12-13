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

#include "qtcachexmlobject.h"

QTCACHENAMESPACEUSE

XmlObject::XmlObject()
{
}

QString XmlObject::fromType(Type t)
{
    switch (t)
    {
    case XmlObject::GBL: return "GBL";
    case XmlObject::CLS: return "CLS";
    case XmlObject::MAC: return "MAC";
    case XmlObject::INT: return "INT";
    case XmlObject::INC: return "INC";
    default: return "UNKNOWN";
    }
}

XmlObject::Type XmlObject::fromString(QStringRef typeString)
{
    if (typeString.compare("GBL", Qt::CaseInsensitive) == 0) return XmlObject::GBL;
    if (typeString.compare("CLS", Qt::CaseInsensitive) == 0) return XmlObject::CLS;
    if (typeString.compare("MAC", Qt::CaseInsensitive) == 0) return XmlObject::MAC;
    if (typeString.compare("INT", Qt::CaseInsensitive) == 0) return XmlObject::INT;
    if (typeString.compare("INC", Qt::CaseInsensitive) == 0) return XmlObject::INC;
    return XmlObject::UNKNOWN;
}

XmlObject::List XmlObject::select(const List& source, Types types)
{
    List selected_objects;
    foreach(const XmlObject& i, source){
        if ((i.m_type & types) == i.m_type){ selected_objects.append(i); }
    }
    return selected_objects;
}
