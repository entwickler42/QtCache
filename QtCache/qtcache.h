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

#include "qtcacheprogressreporter.h"
#include "poormanslogger.h"
#include <QObject>

class QTranslator;

QTCACHENAMESPACEBEGIN

class QtCachePrivate;
class PluginDirector;

class QTCACHESHARED_EXPORT QtCache :
        public ProgressReporter
{
    Q_OBJECT

    friend class QtCachePrivate;

public:
    enum ObjectFilterType
    {
        REGEXP,
        PATTERN
    };

    static QtCache* instance();
    ~QtCache();

    void connect(const QString& cn, const QString& user, const QString& passwd);
    void disconnect();
    bool isConnected() const;

    const QString& targetUci() const;
    void setTargetUci(const QString& targetUci);

    QString lastStatus() const;
    QString errorLog() const;

    long jobId() const;

    QStringList listObjects(const QString& filter = "", ObjectFilterType filterType = PATTERN) const;
    QStringList listNamespaces(bool excludePercent = false) const;
    void execute(const QString&);

    void importXmlFile(const QString& filepath, const QString& qspec = "");
    void exportXmlFile(const QString& directoryPath, const QString& objectName);
    void compileObjects(const QString& objectNames, const QString& qspec = "cf");

    PluginDirector* plugins() const;

protected:
    QtCache();

private:
    QtCachePrivate* d;
};

QTCACHENAMESPACEEND

#endif // QTCACHE_H
