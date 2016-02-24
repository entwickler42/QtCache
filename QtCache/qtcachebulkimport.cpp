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
    : BulkAction(cache, parent)
{
}

void BulkImport::run()
{
    load(this->filepaths, this->qspec);
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
    Progress prog(Progress::BULK_COMPILE_EARLY);
    reportProcessBegin(prog);

    for(int i=0; i<filepaths.length() && !isAborted(); i++){
        const QString& filepath = filepaths.at(i);
        try{
            cache()->importXmlFile(filepath, qspec);
            reportProgress(prog(filepaths.count(), i+1, QStringList() << filepath << filepath));
        }catch(std::exception& ex){
            prog.setTag(filepath);
            emit error(ex, prog);
        }catch(...){
            prog.setTag(filepath);
            emit error(std::runtime_error("Unknown error!"), prog);
        }
    }

    reportProcessEnd(prog(Progress::BULK_COMPILE_EARLY));
    if (isAborted()){
        emit aborted();
    }else{
        emit finished();
    }
}


void BulkImport::loadCompileLate(const QStringList& filepaths, const QString& qspec)
{
    setAbort(false);

    const int ROUTINES=0;
    const int CLASSES=1;
    const int MAXORDER=2;
    XmlObject::List object_list[MAXORDER];

    Progress prog(Progress::BULK_READ);
    reportProcessBegin(prog);

    for(int i=0; i<filepaths.length() && !isAborted(); i++){
        const QString& filepath = filepaths.at(i);
        try{
            XmlObjectReader r(filepath);
            object_list[ROUTINES] += r.routines();
            object_list[CLASSES] += r.classes();
            reportProgress(prog(filepaths.count(), i+1, filepath));
        }catch(std::exception& ex){
            prog.setTag(filepath);
            emit error(ex, prog);
        }catch(...){
            prog.setTag(filepath);
            emit error(std::runtime_error("Unknown error!"), prog);
        }
    }

    if (!isAborted()){
        reportProcessEnd(prog(Progress::BULK_READ));
    }
    if (!isAborted()){
        reportProcessBegin(prog(Progress::BULK_UPLOAD));
    }

    for(int i=0; i<filepaths.length() && !isAborted(); i++){
        const QString& filepath = filepaths.at(i);
        try{
            cache()->importXmlFile(filepath);
            reportProgress(prog(filepaths.count(), i+1, filepath));
        }catch(std::exception& ex){
            prog.setTag(filepath);
            emit error(ex, prog);
        }catch(...){
            prog.setTag(filepath);
            emit error(std::runtime_error("Unknown error!"), prog);
        }
    }

    if (!isAborted()){
        reportProcessEnd(prog(Progress::BULK_UPLOAD));
    }
    if (!isAborted()){
        reportProcessBegin(prog(Progress::BULK_COMPILE));
    }

    if (!qspec.isEmpty() && !isAborted()){
        int total = object_list[ROUTINES].count() + object_list[CLASSES].count();
        int remain = total;
        for(int i=0; i<MAXORDER && !isAborted(); i++)
            for(int j=0; j<object_list[i].count() && !isAborted(); j++){
                const XmlObject& obj = object_list[i].at(j);
                reportProgress(prog(total, total-remain--, QStringList() << obj.name() << obj.sourceName()));
                if (isAborted()) { continue; }
                try{
                    cache()->compileObjects(obj.name(), qspec);
                }catch(std::exception& ex){
                    prog.setTag(obj.sourceName());
                    emit error(ex, prog);
                }catch(...){
                    prog.setTag(obj.sourceName());
                    emit error(std::runtime_error("Unknown error!"), prog);
                }
            }
    }

    if (!isAborted()){
        reportProcessEnd(prog(Progress::BULK_COMPILE));
    }
    if (isAborted()){
        emit aborted();
    }else{
        reportProgress(prog(Progress::BULK_IDLE));
        emit finished();
    }
}

