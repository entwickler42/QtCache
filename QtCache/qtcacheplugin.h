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

#ifndef QTCACHEPLUGIN_H
#define QTCACHEPLUGIN_H

#include "qtcache_global.h"
#include <qtcache.h>
#include <QObject>

class QSettings;
class QCommandLineParser;

QTCACHENAMESPACEBEGIN

class QTCACHESHARED_EXPORT Plugin
    : public QObject
{
    Q_OBJECT

public:
    typedef QtC::Plugin*(*FactoryFunction)(QObject*);
    static const char* FactoryFunctionSymbol;

    explicit Plugin(QObject *parent = 0);
    virtual ~Plugin();

    QtCache* cache() const
    {
        return QtCache::instance();
    }

    virtual QString name() const = 0;
    virtual QString description() const { return ""; }
    virtual int version() const { return 0x000000; }

protected:
    friend class PluginDirector;

    virtual void initialize() {}
    virtual void deinitialize() {}

    virtual void progressBegin(Progress&) {}
    virtual void progress(Progress&) {}
    virtual void progressEnd(Progress&) {}

    virtual void parseCommandlineOptionsBegin(QCommandLineParser&) {}
    virtual void parseCommandlineOptionsEnd(QCommandLineParser&) {}
    virtual void loadApplicationSettingsBegin(QSettings&) {}
    virtual void loadApplicationSettingsEnd(QSettings&) {}
    virtual void saveApplicationSettingsBegin(QSettings&) {}
    virtual void saveApplicationSettingsEnd(QSettings&) {}
};

template<class T>
static T* createPluginInstance(QObject* parent)
{
    return new T(parent);
}

QTCACHENAMESPACEEND

#endif // QTCACHEPLUGIN_H
