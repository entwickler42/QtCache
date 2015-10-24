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

#include "qtcache.h"

#ifdef CACHECPPBIND
#include <Qt_CacheTool.h>
#endif

#ifdef CACHEVISM
#include <vismocx.h>
#endif

/* QtCachePrivate Implementation */

class QtCachePrivate
{
public:
    QtCachePrivate()
    {}

    virtual ~QtCachePrivate()
    {        
        delete qt_cachetool;
    }

    Qt_CacheTool* connect(const QString& cn, const QString& uci, bool forceNew = false)
    {
        if (forceNew && NULL != qt_cachetool){
            delete qt_cachetool;
        }
        if (NULL == qt_cachetool){
            installCacheBackend();
        }
        return qt_cachetool;
    }

private:
    void installCacheBackend()
    {
        #ifdef CACHEVISM
        VISM::_DVisM vism;
        vism.setControl("{88f75483-0574-11d0-8085-0000c0bd354b}");
        #endif
    }

    Qt_CacheTool* qt_cachetool = NULL;
};

/* QtCache Implementation */

QtCache::QtCache()
    : d(new QtCachePrivate)
{}

QtCache::~QtCache()
{
    delete d;
}

QtCache* QtCache::instance()
{
    static QtCache* i = NULL;

    if (NULL == i){
        i = new QtCache();
    }

    return i;
}

int QtCache::connect(const QString& connectionString, const QString& uci, bool forceNew)
{    
    Qt_CacheTool* qct = d->connect(connectionString, uci, forceNew);
    return 0;
}
