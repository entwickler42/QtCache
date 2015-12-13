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

#include "qtcache_config.h"
#include "qtcache_global.h"
#include "qtcache.h"

QTCACHENAMESPACEBEGIN

class BulkImportProgress
{
public:
    enum ImportStep
    {
        IDLE,
        READING,
        UPLOADING,
        COMPILING
    };

    BulkImportProgress(ImportStep step, const QString& filename, int pos, int max)
        : step(step),
          filename(filename),
          pos(pos),
          max(max)
    {}

    ImportStep step = IDLE;
    QString filename;
    int pos;
    int max;
};

class QTCACHESHARED_EXPORT BulkImport
        : public QObject
{
    Q_OBJECT

public:
    explicit BulkImport(QtCache* cache, QObject *parent = 0);

    const BulkImportProgress& currentProgress() const
    {
        return m_last_progress;
    }

signals:
    void finished();
    void error(std::exception& ex, const QtC::BulkImportProgress& progress);
    void reportProgress(const QtC::BulkImportProgress& progress);

public slots:
    void load(const QStringList& filepaths, const QString& qspec = "");
    void abort()
    {
        m_abort_import = true;
    }

private:
    BulkImportProgress m_last_progress;
    QtCache* m_cache = NULL;
    bool m_abort_import = false;

    void setCurrentProgress(const BulkImportProgress& progress)
    {
        m_last_progress = progress;
        emit reportProgress(progress);
    }
};

QTCACHENAMESPACEEND

#endif // BULKIMPORT_H
