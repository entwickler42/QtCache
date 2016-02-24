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

#ifndef QTCACHE_P_H
#define QTCACHE_P_H

#include "qtcache.h"
#include "qtcacheexception.h"
#include "qtcachepluginobserver.h"
#include <ios>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <Qt_CacheTool.h>
#include "poormanslogger.h"

QTCACHENAMESPACEBEGIN

#define TRY_LOG_THROW try
#define CATCH_LOG_THROW \
    catch(std::exception& ex){ \
    LOG_EXCEPTION(ex) \
    throw QtCacheException(ex.what()); \
    }catch(...){ \
    LOG_UNKNOWN_EXCEPTION \
    throw QtCacheException(QString("File %2\nLine %3").arg(__FILE__,__LINE__)); \
    }

typedef d_ref<Qt_CacheTool> QtCacheToolType;

class QtCachePrivate
{
public:
    mutable QtCachePluginObserver* plugin_observer;
    QString cn = "";
    QString uci = "%SYS";
    QString user = "";
    QString passwd = "";
    QtCache *i_ptr;

    QtCachePrivate(QtCache* qtcache)
        : i_ptr(qtcache),
          plugin_observer(NULL)
    {
        TRY_LOG_THROW{
            plugin_observer = new QtCachePluginObserver();
        }CATCH_LOG_THROW;
    }

    virtual ~QtCachePrivate()
    {
        delete plugin_observer;
        disconnect();
    }

    QtCacheToolType tool()
    {
        TRY_LOG_THROW{
            if (!isConnected()){
                throw QtCacheException(QObject::tr("Cachè connection has not been established yet!", "QtCachePrivate"));
            }
            if (Qt_CacheTool.is_null()) {
                Db_err err;
                Qt_CacheTool = Qt_CacheTool::create_new(db,0,&err);
                if (err.get_code()){
                    throw QtCacheException(err);
                }
            }
        }CATCH_LOG_THROW;
        return Qt_CacheTool;
    }

    long jobId() const
    {
        TRY_LOG_THROW{
            if (connected && NULL != db){
                return db->get_job_id();
            }
        }CATCH_LOG_THROW;
        return 0;
    }

    void connect(const QString& cn, const QString& user, const QString& passwd, bool forceNew = false)
    {
        TRY_LOG_THROW{
            Db_err conn_err;

            forceNew = (
                        cn != this->cn ||
                    user != this->user ||
                    passwd != this->passwd
                    );
            if (forceNew && connected){
                disconnect();
            }
            if (forceNew || !connected) {
                d_connection conn = tcp_conn::connect(
                            cn.toStdString(),
                            user.toStdString(),
                            passwd.toStdString(),
                            0,
                            &conn_err
                            );
                if (conn_err.get_code()){
                    throw QtCacheException(conn_err);
                }else if (conn->is_connected()){
                    this->cn = cn;
                    this->user = user;
                    this->passwd = passwd;
                    this->connected = true;
                    db = new Database(conn);
                    installCacheBackend();
                }
            }
        }CATCH_LOG_THROW;
    }

    void disconnect()
    {
        TRY_LOG_THROW{
            delete db; db = NULL;
            connected = false;
        }CATCH_LOG_THROW;
    }

    bool isConnected() const
    {
        TRY_LOG_THROW{
            return connected;
        }CATCH_LOG_THROW;
    }

    void execute(const QString& code)
    {
        TRY_LOG_THROW{
            QtCacheToolType qct = tool();
            d_status sc = qct->Execute(
                        d_string(uci.toStdString()),
                        d_string(code.toStdString()));
            if (sc.get_code()){
                sc.get_msg(db);
                throw QtCacheException(sc);
            }
        }CATCH_LOG_THROW;
    }

    QStringList listNamespaces(bool excludePercent)
    {
        QStringList q_ls;
        TRY_LOG_THROW{
            Progress progress(Progress::QUERY_NS, 0, 0);
            progress.setTag(QStringList() << QString::number(excludePercent));
            i_ptr->reportProcessBegin(progress);
            if (progress.isAborted()){
                return q_ls;
            }
            if (isConnected()){
                d_wstring s;
                d_list c_ls = tool()->ListNamespaces();
                while (!c_ls.at_end()){
                    c_ls.get_elem(s);
                    QString uci = QString::fromStdWString(s.value());
                    progress.setTag(uci);
                    i_ptr->reportProgress(progress);
                    if (progress.isAborted()) { break; }
                    if (uci.startsWith('%') && excludePercent){
                        c_ls.next();
                        continue;
                    }
                    q_ls << uci;
                    c_ls.next();
                }
            }
            i_ptr->reportProcessEnd(progress);
        }CATCH_LOG_THROW;
        return q_ls;
    }

    QStringList listObjects(const QString& filter, QtCache::ObjectFilterType filterType)
    {
        QStringList objects;
        TRY_LOG_THROW{
            Progress progress(Progress::QUERY_OBJECTS, 0, 0);
            progress.setTag(QStringList() << filter << QString::number(filterType));
            i_ptr->reportProcessBegin(progress);
            if (progress.isAborted()){ return objects; }

            d_string _uci(uci.toStdString());
            d_string _filter = filter.toStdString();
            d_ref<d_char_stream> bstream = tool()->ListObjects(_uci, _filter, filterType);
            d_status sc = tool()->getLastStatus();
            if (sc.get_code()){
                sc.get_msg(db);
                throw QtCacheException(sc);
            }
            d_iostream io(bstream);
            std::string line;
            io.rewind();

            i_ptr->reportProgress(progress(0,0));
            while (std::getline(io, line) && !progress.isAborted()){
                progress.setTag(QString::fromStdString(line).remove('\r'));
                objects << progress.tag().toString();
                i_ptr->reportProgress(progress(100,-1));
            }
            i_ptr->reportProcessEnd(progress);
        }CATCH_LOG_THROW;
        return objects;
    }

    void importXmlFile(const QString& filepath, const QString& qspec = "")
    {
        TRY_LOG_THROW{
            Progress progress(Progress::XMLFILE_IMPORT);
            i_ptr->reportProcessBegin(progress(0, 0, QStringList() << filepath << qspec));
            if (progress.isAborted()){ return; }

            QFile f(filepath);

            if (!f.exists()){
                throw QtCacheException(QObject::tr("Input file does not exists:\n%1", "QtCachePrivate").arg(filepath));
            }
            if(!f.open(QFile::ReadOnly)){
                throw QtCacheException(QObject::tr("Could not open file for reading:\n%1", "QtCachePrivate").arg(filepath));
            }

            d_ref<d_bin_stream> bstream = d_bin_stream::create_new(db);
            d_iostream io(bstream);

            qint64 file_pos = 0;
            qint64 file_size = f.size();
            const qint64 chunk_size = 512;
            char buf[chunk_size];
            i_ptr->reportProgress(progress(file_size, file_pos));
            while (!f.atEnd()){
                qint64 s = f.read(buf, chunk_size);
                io.write(buf, s);
                file_pos += s;
                i_ptr->reportProgress(progress(file_size, file_pos));
                if (progress.isAborted()){ break; }
            }
            io.rewind();
            if (!progress.isAborted()){
                i_ptr->reportProgress(progress(100, 100));
                d_string _uci(uci.toStdString());
                d_string _qspec(qspec.toStdString());
                d_status sc = tool()->ImportXML(_uci, bstream, _qspec);
                if (sc.get_code()){
                    sc.get_msg(db);
                    throw QtCacheException(sc);
                }
            }
            i_ptr->reportProcessEnd(progress);
        }CATCH_LOG_THROW;
    }

    void exportXmlFile(const QString& directoryPath, const QString& objectName)
    {
        TRY_LOG_THROW{
            Progress prog(Progress::XMLFILE_EXPORT);
            i_ptr->reportProcessBegin(prog(0, 0, QStringList() << directoryPath << objectName));
            if (prog.isAborted()){ return; }

            d_string _objectName = objectName.toStdString();
            d_string _uci = uci.toStdString();
            d_ref<d_bin_stream> bstream = tool()->ExportXML(_uci, _objectName);
            d_status sc = tool()->getLastStatus();
            if (sc.get_code()){
                sc.get_msg(db);
                throw QtCacheException(sc);
            }
            QDir out_dir(directoryPath);
            if(!out_dir.mkpath(".")){
                throw QtCacheException(QObject::tr("Output directory does not exists!"));
            }
            QFile f(out_dir.absoluteFilePath(objectName));
            if (!f.open(QFile::WriteOnly|QFile::Truncate)){
                throw QtCacheException(QObject::tr("Can't open output file:\n%1", "QtCachePrivate").arg(objectName));
            }
            std::string buf;
            QTextStream ofstream(&f);
            ofstream.setCodec("UTF-8");
            ofstream.setGenerateByteOrderMark(true);
            d_iostream io(bstream);
            io.rewind();

            long bytes_written = 0;
            while (std::getline(io,buf) && !prog.isAborted()){
                prog.setTag(QString::fromStdString(buf));
                i_ptr->reportProgress(prog(io.size(), bytes_written));
                if (prog.isAborted()) { continue; }
                ofstream << prog.tag().toString() << "\n";
                bytes_written += buf.length();
            }
            i_ptr->reportProcessEnd(prog(100,100));
        }CATCH_LOG_THROW;
    }

    void compileObjects(const QString& objectNames, const QString& qspec)
    {
        TRY_LOG_THROW{
            Progress progress(Progress::OBJECT_COMPILE);
            i_ptr->reportProcessBegin(progress(0, 0, QStringList() << objectNames << qspec));
            if (progress.isAborted()){ return; }

            d_string _objectNames(objectNames.toStdString());
            d_string _qspec(qspec.toStdString());
            d_string _uci = uci.toStdString();
            i_ptr->reportProgress(progress(100, 100));
            d_status sc = tool()->CompileList(_uci, _objectNames, _qspec);
            if (sc.get_code()){
                sc.get_msg(db);
                throw QtCacheException(tool()->getErrorLog());
            }

            i_ptr->reportProcessEnd(progress);
        }CATCH_LOG_THROW;
    }

private:
    bool connected = false;
    Database* db = NULL;
    QtCacheToolType Qt_CacheTool;

    void installCacheBackend()
    {
#if 1
        TRY_LOG_THROW{
            QFile xml(":/src/QtCache.xml");
            if (!xml.open(QFile::ReadOnly)){
                throw QtCacheException("can not open QtCache.xml from resources");
            }
            QByteArray data = xml.readAll();
            d_ref<d_bin_stream> bstream = d_bin_stream::create_new(db);
            bstream->write(d_binary(data.toStdString()));
            bstream->rewind();
            d_string qspec = "cf";
            D_type* args[2] = { &bstream, &qspec };
            Dyn_obj::run_class_method(db, L"%SYSTEM.OBJ", L"LoadStream", args, 2);
        }CATCH_LOG_THROW;;
#endif
    }

};

QTCACHENAMESPACEEND

#endif // QTCACHE_P_H

