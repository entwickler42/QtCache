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

QTCACHENAMESPACEBEGIN

class QTCACHESHARED_EXPORT Plugin
    : public QObject
{
    Q_OBJECT

public:
    typedef QtC::Plugin*(*FactoryFunction)(QObject*);
    static const char* FactoryFunctionSymbol;

    explicit Plugin(QObject *parent = 0);

    QtCache* cache() const
    {
        return QtCache::instance();
    }

    virtual QString name() const = 0;
    virtual QString description() const { return ""; }
    virtual int version() const { return 0x000000; }

    virtual void initialize() {}
    virtual void deinitialize() {}

    virtual void progressBegin(Progress& progress) = 0;
    virtual void progress(Progress& progress) = 0;
    virtual void progressEnd(Progress& progress) = 0;

    virtual void bulkProgressBegin(Progress& progress) = 0;
    virtual void bulkProgress(Progress& progress) = 0;
    virtual void bulkProgressEnd(Progress& progress) = 0;
};

template<class T>
static T* createPluginInstance(QObject* parent)
{
    return new T(parent);
}

QTCACHENAMESPACEEND

#endif // QTCACHEPLUGIN_H
