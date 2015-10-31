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

#include <QFile>
#include <Qt_CacheTool.h>
#include "qtcacheexception.h"


#ifdef CACHEVISM
#include <vismocx.h>
#endif

typedef d_ref<Qt_CacheTool> QtCacheToolType;

class QtCachePrivate
        : QObject
{
public:
    QString cn = "";
    QString uci = "%SYS";
    QString user = "";
    QString passwd = "";

    QtCachePrivate()
    {}

    virtual ~QtCachePrivate()
    {
        delete db;
    }

    QtCacheToolType tool()
    {
        if (conn->is_connected() && Qt_CacheTool.is_null()) {
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
                        &conn_err);

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

    void importFile(const QString& uci, const QString& filepath, const QString& qspec)
    {
        QFile f(filepath);

        if (!f.exists()){
            throw QtCacheException(tr("Input file does not exists:\n%1").arg(filepath));
        }
        if(!f.open(QFile::ReadOnly)){
            throw QtCacheException(tr("Could not open file for reading:\n%1").arg(filepath));
        }

        QByteArray data = f.readAll();
        f.close();

        d_string _data(data.constData());
        d_string _uci(uci.toStdString());
        d_string _qspec(qspec.toStdString());
        d_ref<d_char_stream> fstream = d_char_stream::create_new(db);
        fstream->write(_data);
        d_status sc = tool()->ImportXML(_uci, fstream, _qspec);
        if (sc.get_code()){
            throw QtCacheException(sc);
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

