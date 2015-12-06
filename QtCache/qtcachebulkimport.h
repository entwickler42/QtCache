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
    BulkImportProgress(const QString& filename, int pos, int max)
        : filename(filename),
          pos(pos),
          max(max)
    {
    }
    const QString& filename;
    const int pos;
    const int max;
};

class QTCACHESHARED_EXPORT BulkImport
        : public QObject
{
    Q_OBJECT

public:
    enum ImportStep
    {
        IDLE,
        LOADING,
        UPLOADING,
        COMPILING
    };

    explicit BulkImport(QtCache* cache, QObject *parent = 0);

    const ImportStep& CurrentStep() const
    {
        return m_current_step;
    }

signals:
    void finished();
    void error(std::exception& ex, const QString& filename);
    void reportProgress(const BulkImportProgress& progress);

public slots:
    void load(const QStringList& filepaths, const QString& qspec = "");
    void abort()
    {
        m_abort_import = true;
    }

private:
    ImportStep m_current_step;
    QtCache* m_cache = NULL;
    bool m_abort_import = false;

    void setCurrentStep(ImportStep step)
    {
        m_current_step = step;
    }
};

QTCACHENAMESPACEEND

#endif // BULKIMPORT_H
