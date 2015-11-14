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
#include <QTranslator>


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
    return QString::fromStdString(sc.get_msg().value());
}

QString QtCache::errorLog() const
{
    d_string err = d->tool()->getErrorLog();
    return QString::fromStdString(err.value());
}

long QtCache::jobId() const
{
    return d->jobId();
}

QStringList QtCache::listObjects(const QString& filter = "") const
{
    return d->listObjects(filter);
}

QStringList QtCache::listNamespaces(bool excludePercent) const
{
    return d->listNamespaces(excludePercent);
}

void QtCache::execute(const QString& code)
{
    d->execute(code);
}

void QtCache::importObject(const QString& filepath, const QString& qspec)
{
    d->importObject(filepath, qspec);
}

void QtCache::exportObject(const QString& directoryPath, const QString& objectName)
{
    d->exportObject(directoryPath, objectName);
}
