#include "bulkimport.h"
#include <stdexcept>
#include <qtcachexmlreader.h>

using namespace LibQtCache;

BulkImport::BulkImport(QtCache* cache, QObject *parent)
    : QObject(parent),
      m_cache(cache)
{
    if (NULL == m_cache) throw new std::invalid_argument("QtCache* cache must not be NULL");
}

void BulkImport::load(const QStringList& filepaths, const QString& qspec)
{
    m_abort_import = false;
    QtCacheXml::Object::List object_list;

    for(int i=0; i<filepaths.length() && !m_abort_import; i++){
        const QString& filepath = filepaths.at(i);
        emit loading(filepath, i+1, filepaths.count());
        try{
            QtCacheXml::Reader r(filepath);
            object_list += r.routines();
            object_list += r.classes();
            m_cache->importFile(filepath);
        }catch(std::exception& ex){
            emit error(ex);
        }
    }

    if (!qspec.isEmpty()){
        try{
            for(int i=0; i<object_list.count() && !m_abort_import; i++){
                const QtCacheXml::Object& obj = object_list.at(i);
                emit compiling(obj.name(), i+1, object_list.count());
                m_cache->compile(obj.name(), qspec);
            }
        }catch(std::exception& ex){
            emit error(ex);
        }
    }

    emit finished();
}

