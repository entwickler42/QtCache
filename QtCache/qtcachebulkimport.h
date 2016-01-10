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

#ifndef BULKIMPORT_H
#define BULKIMPORT_H

#include "qtcache.h"

QTCACHENAMESPACEBEGIN

class QTCACHESHARED_EXPORT BulkImport
        : public QObject
{
    Q_OBJECT

public:
    bool compileEarly = false;

    explicit BulkImport(QtCache* cache, QObject *parent = 0);

    const Progress& currentProgress() const
    {
        return m_last_progress;
    }

signals:
    void aborted();
    void finished();
    void error(std::exception& ex, QtC::Progress&);
    void progress(QtC::Progress&);

public slots:
    void load(const QStringList& filepaths, const QString& qspec = "");
    void abort()
    {
        m_abort_import = true;
    }

private:
    Progress m_last_progress;
    QtCache* m_cache = NULL;
    bool m_abort_import = false;

    void loadCompileEarly(const QStringList& filepaths, const QString& qspec = "");
    void loadCompileLate(const QStringList& filepaths, const QString& qspec = "");

    void reportProcessBegin(Progress& p);
    void reportProgress(Progress& p);
    void reportProcessEnd(Progress& p);
};

QTCACHENAMESPACEEND

#endif // BULKIMPORT_H
