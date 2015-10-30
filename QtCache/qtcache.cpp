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

const QString& QtCache::uci() const
{
    return d->uci;
}

void QtCache::setUci(const QString& uci)
{
    d->uci = uci;
}

QString QtCache::lastStatus()
{
    d_status sc = d->tool()->getLastStatus();
    return QString::fromStdString(sc.get_msg().value());
}

QStringList QtCache::listNamespaces()
{
    QStringList q_ls;
    d_list c_ls = d->tool()->ListNamespaces();
    while (!c_ls.at_end()){
        d_string s;
        c_ls.get_elem(s);
        q_ls << QString::fromStdString(s.value());
        c_ls.next();
    }
    return q_ls;
}

void QtCache::execute(const QString& code)
{
    d->execute(code);
}
