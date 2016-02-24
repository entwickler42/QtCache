#include "qtcachebulkexport.h"

QTCACHENAMESPACEUSE

BulkExport::BulkExport(QtCache* cache, QObject *parent)
    : BulkAction(cache, parent)
{}

void BulkExport::run()
{
    save(this->outputDirectory, this->filter, this->filterType);
}

void BulkExport::save(const QDir& outputDirectory, const QString& filter, QtCache::ObjectFilterType filterType)
{
    Progress prog(Progress::BULK_SAVE);
    reportProcessBegin(prog);

    QStringList ls = cache()->listObjects(filter, filterType);

    for(int i=0; !isAborted() && i<ls.count(); i++){
        const QString& s = ls.at(i);
        reportProgress(prog(ls.count(), i+1, QStringList() << outputDirectory.absolutePath() << s));
        if (isAborted()){
            continue;
        }
        try{
            cache()->exportXmlFile(outputDirectory.absolutePath(), s);
        }catch(std::exception& ex){
            prog.setTag(s);
            emit error(ex, prog);
        }catch(...){
            prog.setTag(s);
            emit error(std::runtime_error("Unknown error!"), prog);
        }
    }

    reportProcessEnd(prog);
    if (isAborted()){
        emit aborted();
    }else{
        emit finished();
    }
}
