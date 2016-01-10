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

#include "qtcachebulkimport.h"
#include "qtcachepluginobserver.h"
#include <stdexcept>
#include <qtcachexmlreader.h>

QTCACHENAMESPACEUSE

BulkImport::BulkImport(QtCache* cache, QObject *parent)
    : QObject(parent),
      m_last_progress(Progress::BULK_IDLE, 0, 0),
      m_cache(cache)
{
    if (NULL == m_cache) throw new std::invalid_argument("QtCache* cache must not be NULL");
}

void BulkImport::load(const QStringList& filepaths, const QString& qspec)
{
    if (compileEarly){
        loadCompileEarly(filepaths, qspec);
    }else{
        loadCompileLate(filepaths, qspec);
    }
}

void BulkImport::loadCompileEarly(const QStringList& filepaths, const QString& qspec)
{
    m_abort_import = false;

    for(int i=0; i<filepaths.length() && !m_abort_import; i++){
        const QString& filepath = filepaths.at(i);
        try{
            reportProgress(Progress(Progress::BULK_UPLOAD, filepath, filepaths.count(), i+1));
            m_cache->importXmlFile(filepath, qspec);
        }catch(std::exception& ex){
            emit error(ex, m_last_progress);
        }catch(...){
            emit error(std::runtime_error("Unknown error!"), m_last_progress);
        }
    }
    reportProgress(Progress(Progress::BULK_IDLE));
    if (m_abort_import) emit aborted();
    else emit finished();
}


void BulkImport::loadCompileLate(const QStringList& filepaths, const QString& qspec)
{
    m_abort_import = false;

    const int ROUTINES=0;
    const int CLASSES=1;
    const int MAXORDER=2;
    XmlObject::List object_list[MAXORDER];

    for(int i=0; i<filepaths.length() && !m_abort_import; i++){
        const QString& filepath = filepaths.at(i);
        try{
            reportProgress(Progress(Progress::BULK_READ, filepath, filepaths.count(), i+1));
            XmlObjectReader r(filepath);
            object_list[ROUTINES] += r.routines();
            object_list[CLASSES] += r.classes();
            reportProgress(Progress(Progress::BULK_UPLOAD, filepath, filepaths.count(), i+1));
            m_cache->importXmlFile(filepath);
        }catch(std::exception& ex){
            emit error(ex, m_last_progress);
        }catch(...){
            emit error(std::runtime_error("Unknown error!"), m_last_progress);
        }
    }

    if (!qspec.isEmpty()){
        int total = object_list[ROUTINES].count() + object_list[CLASSES].count();
        int remain = total;
        for(int i=0; i<MAXORDER; i++)
            for(int j=0; j<object_list[i].count() && !m_abort_import; j++){
                const XmlObject& obj = object_list[i].at(j);
                reportProgress(Progress(Progress::BULK_COMPILE, obj.name() + obj.sourceName(), total, total-remain--));
                try{
                    m_cache->compileObjects(obj.name(), qspec);
                }catch(std::exception& ex){
                    emit error(ex, m_last_progress);
                }catch(...){
                    emit error(std::runtime_error("Unknown error!"), m_last_progress);
                }
            }
    }
    reportProgress(Progress(Progress::BULK_IDLE));
    if (m_abort_import) emit aborted();
    else emit finished();
}

void BulkImport::reportProcessBegin(Progress& p)
{
    m_cache->pluginObserver()->bulkProgressBegin(p);
    if (p.isAborted()) { this->abort(); }
}

void BulkImport::reportProgress(Progress& p)
{
    m_last_progress = p;
    m_cache->pluginObserver()->bulkProgress(p);
    if (p.isAborted()) { this->abort(); }
    emit progress(p);
}

void BulkImport::reportProcessEnd(Progress& p)
{
    m_cache->pluginObserver()->bulkProgressEnd(p);
    if (p.isAborted()) { this->abort(); }
}
