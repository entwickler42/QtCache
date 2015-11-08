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

#include <QDir>
#include <QFile>
#include <QObject>
#include <QTextStream>
#include <Qt_CacheTool.h>
#include "qtcache.h"
#include "qtcacheexception.h"

#include <ios>

#ifdef CACHEVISM
#include <vismocx.h>
#endif

typedef d_ref<Qt_CacheTool> QtCacheToolType;

class QtCachePrivate
{
public:
    QString cn = "";
    QString uci = "%SYS";
    QString user = "";
    QString passwd = "";
    QtCache *i_ptr;

    QtCachePrivate(QtCache* qtcache)
        : i_ptr(qtcache)
    {}

    virtual ~QtCachePrivate()
    {
        delete db;
    }

    QtCacheToolType tool()
    {
        if (!isConnected()){
            throw QtCacheException(QObject::tr("Cachè connection has not been established yet!", "QtCachePrivate"));
        }
        if (Qt_CacheTool.is_null()) {
            Qt_CacheTool = Qt_CacheTool::create_new(db);
        }
        return Qt_CacheTool;
    }

    void connect(const QString& cn, const QString& user, const QString& passwd, bool forceNew = false)
    {
        Db_err conn_err;

        forceNew = (
                    cn != this->cn ||
                    user != this->user ||
                    passwd != this->passwd
                );
        if (forceNew || !conn->is_connected()) {
            conn = tcp_conn::connect(
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
                delete db;
                db = new Database(conn);
                installCacheBackend();
            }
        }
    }

    void disconnect()
    {
        Qt_CacheTool = QtCacheToolType();
        conn = d_connection();
        connected = false;
    }

    bool isConnected() const
    {
        return connected;
    }

    void execute(const QString& code)
    {
        QtCacheToolType qct = tool();
        d_status sc = qct->Execute(
                    d_string(uci.toStdString()),
                    d_string(code.toStdString()));
        if (sc.get_code()){
            throw QtCacheException(sc);
        }
    }

    QStringList listNamespaces(bool excludePercent)
    {
        QStringList q_ls;
        if (isConnected()){
            d_wstring s;
            d_list c_ls = tool()->ListNamespaces();
            while (!c_ls.at_end()){
                c_ls.get_elem(s);
                QString uci = QString::fromStdWString(s.value());
                if (uci.startsWith('%') && excludePercent){
                    c_ls.next();
                    continue;
                }
                q_ls << uci;
                c_ls.next();
            }
        }
        return q_ls;
    }

    void importFile(const QString& filepath, const QString& qspec)
    {
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
        while (!f.atEnd()){
            qint64 s = f.read(buf, chunk_size);
            io.write(buf, s);
            file_pos += s;
            emit i_ptr->reportProgress(filepath, file_pos, file_size);
        }
        io.rewind();

        emit i_ptr->reportProgress(QObject::tr("Cachè is processing the input now. Please be patient...", "QtCachePrivate"), 0, 100);
        d_string _uci(uci.toStdString());
        d_string _qspec(qspec.toStdString());
        d_status sc = tool()->ImportXML(_uci, bstream, _qspec);
        if (sc.get_code()){
            throw QtCacheException(sc);
        }
    }

    QStringList listObjects(const QString& filter)
    {
        d_string _uci(uci.toStdString());
        d_string _filter = filter.toStdString();
        d_ref<d_char_stream> bstream = tool()->ListObjects(_uci, _filter);
        d_status sc = tool()->GetLastStatus();
        if (sc.get_code()){
            throw QtCacheException(sc);
        }
        d_iostream io(bstream);
        QStringList objects;
        std::string line;
        io.rewind();
        while (std::getline(io, line)){
            objects << QString::fromStdString(line).remove('\r');
        }
        return objects;
    }

    void exportFiles(const QString& directoryPath, const QString& objectName)
    {
        d_string _objectName = objectName.toStdString();
        d_string _uci = uci.toStdString();
        d_ref<d_bin_stream> bstream = tool()->ExportXML(_uci, _objectName);
        d_status sc = tool()->GetLastStatus();
        if (sc.get_code()){
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
        while (std::getline(io,buf)){
            ofstream << QString::fromStdString(buf) << "\n";
        }
    }

private:
    bool connected = false;
    Database* db = NULL;
    d_connection conn;
    QtCacheToolType Qt_CacheTool;

    void installCacheBackend()
    {
#ifdef CACHEVISM
        VISM::_DVisM vism;
        vism.setControl("{88f75483-0574-11d0-8085-0000c0bd354b}");
#endif
    }
};



#endif // QTCACHE_P_H

