#include "bulkimport.h"
#include <stdexcept>


BulkImport::BulkImport(QtCache* cache, QObject *parent)
    : QObject(parent),
      m_cache(cache)
{}

void BulkImport::load(const QStringList& filepaths, const QString& qspec)
{
    m_abort_import = false;
    for(int i=0; i<filepaths.length() && !m_abort_import; i++){
        const QString& filepath = filepaths.at(i);
        emit loadingFile(filepath, i+1, filepaths.count());
        try{
            load(filepath, qspec);
        }catch(std::exception& ex){
            emit error(ex);
        }
    }
    emit finished();
}

void BulkImport::load(const QString& filepath, const QString& qspec)
{

}
