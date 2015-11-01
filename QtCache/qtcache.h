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
class QTranslator;


class QTCACHESHARED_EXPORT QtCache
{
public:
    static QtCache* instance();
    virtual ~QtCache();

    QTranslator* createTranslator(const QString& lang);

    void connect(const QString& cn, const QString& user, const QString& passwd);
    void disconnect();
    bool isConnected() const;

    const QString& uci() const;
    void setUci(const QString& uci);

    QString lastStatus() const;
    QString errorLog() const;

    QStringList listNamespaces(bool excludePercent = false) const;
    void execute(const QString&);

    void importFile(const QString& uci, const QString& filepath, const QString& qspec);

protected:
    QtCache();

private:
    QtCachePrivate* d;
};

#endif // QTCACHE_H
