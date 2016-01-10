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

#ifndef QTCACHETOOLSETTINGS_H
#define QTCACHETOOLSETTINGS_H

#include <QDir>
#include <QString>
#include <QSettings>

#define DECL_PROPERTY(TYPE,NAME,DEFAULT) \
    TYPE NAME() { return get(#NAME, DEFAULT); } \
    void set##NAME(const TYPE& value) { set(#NAME, value); } \

class QtCacheToolSettings
        : public QObject
{
    Q_OBJECT

public:
    QtCacheToolSettings(QObject* parent = 0, const QString& filename = "QtCacheTool.ini");
    ~QtCacheToolSettings();

    QSettings* config() const { return conf; }

    DECL_PROPERTY(QString, PreferedUCI, QString(""))
    DECL_PROPERTY(QString, QSPEC, QString("cfkb"))
    DECL_PROPERTY(QString, PostImportHook, QString(""))
    DECL_PROPERTY(QString, PreImportHook, QString(""))
    DECL_PROPERTY(QString, Server, QString("127.0.0.1"))
    DECL_PROPERTY(QString, Port, QString("1972"))
    DECL_PROPERTY(QString, User, QString("_system"))
    DECL_PROPERTY(QString, Passwd, QString(""))
    DECL_PROPERTY(QString, DefaultExportDirectory, QDir::currentPath())

    DECL_PROPERTY(bool, AutoConnect, false)
    DECL_PROPERTY(bool, Compile, true)
    DECL_PROPERTY(bool, CompileEarly, true)

private:
    QSettings* conf;

    template<class T> T get(const QString& key, T defaultValue = T())
    {
        if(!conf->allKeys().contains(key)){
            set(key, defaultValue);
        }
        return conf->value(key, defaultValue).value<T>();
    }

    template<class T> void set(const QString& key, const T& value)
    {
        conf->setValue(key, value);
        conf->sync();
    }
};

#endif // QTCACHETOOLSETTINGS_H
