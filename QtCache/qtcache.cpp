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
#include "qtcache_p.h"
#include "poormanslogger.h"

QTCACHENAMESPACEUSE

QtCache::QtCache()
    : d(new QtCachePrivate(this))
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

void QtCache::connect(const QString& cn, const QString& user, const QString& passwd)
{
    return d->connect(cn, user, passwd);
}

void QtCache::disconnect()
{
    d->disconnect();
}

bool QtCache::isConnected() const
{
    return d->isConnected();
}

const QString& QtCache::targetUci() const
{
    return d->uci;
}

void QtCache::setTargetUci(const QString& uci)
{
    d->uci = uci;
}

QString QtCache::lastStatus() const
{
    d_status sc = d->tool()->getLastStatus();
    return QString::fromStdString((std::string)sc.get_msg());
}

QString QtCache::errorLog() const
{
    d_string err = d->tool()->getErrorLog();
    return QString::fromStdString((std::string)err);
}

long QtCache::jobId() const
{
    return d->jobId();
}

QStringList QtCache::listObjects(const QString& filter, ObjectFilterType filterType) const
{
    return d->listObjects(filter, filterType);
}

QStringList QtCache::listNamespaces(bool excludePercent) const
{
    return d->listNamespaces(excludePercent);
}

void QtCache::execute(const QString& code)
{
    d->execute(code);
}

void QtCache::importXmlFile(const QString& filepath, const QString& qspec)
{
    d->importXmlFile(filepath, qspec);
}

void QtCache::exportXmlFile(const QString& directoryPath, const QString& objectName, const QString& fileName)
{
    d->exportXmlFile(directoryPath, objectName, fileName);
}

void QtCache::compileObjects(const QString& objectNames, const QString& qspec)
{
    d->compileObjects(objectNames, qspec);
}

PluginDirector* QtCache::plugins() const
{
    return d->plugin_observer;
}

