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
#include "poormanslogger.h"
#include <QObject>

class QTranslator;

QTCACHENAMESPACEBEGIN

class QtCachePrivate;

class QTCACHESHARED_EXPORT QtCache
        : public QObject
{
    Q_OBJECT

signals:
    void reportProgress(const QString& message, qint64 pos, qint64 end);

public:
    static QtCache* instance();
    virtual ~QtCache();

    void connect(const QString& cn, const QString& user, const QString& passwd);
    void disconnect();
    bool isConnected() const;

    const QString& targetUci() const;
    void setTargetUci(const QString& targetUci);

    QString lastStatus() const;
    QString errorLog() const;

    long jobId() const;

    QStringList listObjects(const QString& filter) const;
    QStringList listNamespaces(bool excludePercent = false) const;
    void execute(const QString&);

    void importFile(const QString& filepath, const QString& qspec = "");
    void exportFile(const QString& directoryPath, const QString& objectName);
    void compileObject(const QString& name, const QString& qspec = "cf");

protected:
    QtCache();

private:
    QtCachePrivate* d;
};

QTCACHENAMESPACEEND

#endif // QTCACHE_H
