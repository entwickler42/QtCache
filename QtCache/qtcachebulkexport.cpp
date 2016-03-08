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
    Progress prog(Progress::BULK_SAVE, outputDirectory.absolutePath());
    reportProcessBegin(prog);

    QStringList ls = cache()->listObjects(filter, filterType);
    QStringList ls_exported;

    for(int i=0; !isAborted() && i<ls.count(); i++){
        const QString& objectname = ls.at(i);
        QString final_filename = objectname;

        int filename_count = 0;
        while(ls_exported.contains(final_filename.toUpper())){
            final_filename = QString("%1.%2").arg(objectname).arg(++filename_count);
        }

        if (isAborted()){
            continue;
        }
        try{
            cache()->exportXmlFile(outputDirectory.absolutePath(), objectname, final_filename);
            ls_exported.append(final_filename.toUpper());
            reportProgress(prog(ls.count(), i+1, QStringList() << outputDirectory.absolutePath() << final_filename));
        }catch(std::exception& ex){
            prog.setTag(objectname);
            emit error(ex, prog);
        }catch(...){
            prog.setTag(objectname);
            emit error(std::runtime_error("Unknown error!"), prog);
        }
    }

    prog.setTag(outputDirectory.absolutePath());
    reportProcessEnd(prog);
    if (isAborted()){
        emit aborted();
    }else{
        emit finished();
    }
}
