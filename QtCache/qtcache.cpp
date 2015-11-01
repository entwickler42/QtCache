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


QTranslator* QtCache::createTranslator(const QString& lang)
{
    QString filepath = QString(":/i18n/qtcacheui_%1.qm").arg(lang);
    if (QFile::exists(filepath)){
        QTranslator* t = new QTranslator();
        if (t->load(filepath)){
            return t;
        }else{
            delete t;
        }
    }
    return NULL;
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

QStringList QtCache::listNamespaces() const
{
    QStringList q_ls;
    if (d->isConnected()){
        d_wstring s;
        d_list c_ls = d->tool()->ListNamespaces();
        while (!c_ls.at_end()){
            c_ls.get_elem(s);
            q_ls << QString::fromStdWString(s.value());
            c_ls.next();
        }
    }
    return q_ls;
}

void QtCache::execute(const QString& code)
{
    d->execute(code);
}

void QtCache::importFile(const QString& uci, const QString& filepath, const QString& qspec)
{
    d->importFile(uci, filepath, qspec);
}
