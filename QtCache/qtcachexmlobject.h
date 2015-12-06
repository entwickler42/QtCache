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

#ifndef QTCACHEXMLOBJECT_H
#define QTCACHEXMLOBJECT_H

#include "qtcache_config.h"
#include "qtcache_global.h"
#include <QObject>

QTCACHENAMESPACEBEGIN

class QTCACHESHARED_EXPORT XmlObject
{
public:
    typedef QList<XmlObject> List;

    enum Type
    {
        UNKNOWN = 0x00,
        GBL = 0x01,
        CLS = 0x02,
        MAC = 0x04,
        INT = 0x06,
        INC = 0x08
    };

    Q_DECLARE_FLAGS(Types, Type)

    XmlObject();

    const Type& type() const { return m_type; }
    const QString& name() const { return m_name; }
    const QString& timeCreated() const { return m_timeCreated; }
    const QString& timeChanged() const { return m_timeChanged; }

    void setType(Type t) { m_type = t;}
    void setName(const QString& name) { m_name = name;}
    void setTimeCreated(const QString& timeCreated) { m_timeCreated = timeCreated;}
    void setTimeChanged(const QString& name) { m_timeChanged = name;}

    static QString fromType(Type t);
    static Type fromString(QStringRef typeString);
    static List select(const List& source, Types types);

private:
    Type m_type;
    QString m_name;
    QString m_timeCreated;
    QString m_timeChanged;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(XmlObject::Types)

}

#endif // QTCACHEXMLOBJECT_H
