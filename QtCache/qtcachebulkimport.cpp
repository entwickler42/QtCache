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
#include <stdexcept>
#include <qtcachexmlreader.h>

QTCACHENAMESPACEUSE

BulkImport::BulkImport(QtCache* cache, QObject *parent)
    : QObject(parent),
      m_current_step(BulkImport::IDLE),
      m_cache(cache)
{
    if (NULL == m_cache) throw new std::invalid_argument("QtCache* cache must not be NULL");
}

void BulkImport::load(const QStringList& filepaths, const QString& qspec)
{
    m_abort_import = false;
    XmlObject::List object_list;

    for(int i=0; i<filepaths.length() && !m_abort_import; i++){
        const QString& filepath = filepaths.at(i);
        try{
            reflectStepAndProgress(LOADING, BulkImportProgress(filepath, i+1, filepaths.count()));
            XmlObjectReader r(filepath);
            object_list += r.routines();
            object_list += r.classes();
            reflectStepAndProgress(UPLOADING, BulkImportProgress(filepath, i+1, filepaths.count()));
            m_cache->importXmlFile(filepath);
        }catch(std::exception& ex){
            emit error(ex, filepath);
        }
    }
    if (!qspec.isEmpty()){

        for(int i=0; i<object_list.count() && !m_abort_import; i++){
            const XmlObject& obj = object_list.at(i);
            reflectStepAndProgress(COMPILING, BulkImportProgress(obj.name(), i+1, object_list.count()));
            try{
                m_cache->compileObjects(obj.name(), qspec);
            }catch(std::exception& ex){
                emit error(ex, obj.name());
            }
        }
    }
    reflectStepAndProgress(IDLE, BulkImportProgress("", 0, 0));
    emit finished();
}

