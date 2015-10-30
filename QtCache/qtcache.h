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

#ifndef QTCACHE_H
#define QTCACHE_H

#include "qtcache_global.h"
#include <QObject>

class QtCachePrivate;

class QTCACHESHARED_EXPORT QtCache
{
public:
    static QtCache* instance();
    virtual ~QtCache();

    void connect(const QString& cn, const QString& user, const QString& passwd);
    void disconnect();
    bool isConnected() const;

    const QString& uci() const;
    void setUci(const QString& uci);

    QString lastStatus();

    QStringList listNamespaces();
    void execute(const QString&);

protected:
    QtCache();

private:
    QtCachePrivate* d;
};

#endif // QTCACHE_H
