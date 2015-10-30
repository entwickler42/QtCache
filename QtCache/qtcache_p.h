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

#include <exception>

#ifdef CACHECPPBIND
#include <Qt_CacheTool.h>
#endif

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

    QtCachePrivate()
    {}

    virtual ~QtCachePrivate()
    {
        delete m_db;
    }

    QtCacheToolType tool()
    {
        connect(cn, user, passwd);
        if (m_QtCacheTool.is_null()) {
            m_QtCacheTool = Qt_CacheTool::create_new(m_db);
        }
        return m_QtCacheTool;
    }

    void connect(const QString& cn, const QString& user, const QString& passwd, bool forceNew = false)
    {
        Db_err conn_err;

        forceNew = (
                cn != this->cn ||
                user != this->user ||
                passwd != this->passwd
                );
        if (forceNew || !m_Conn->is_connected()) {
            m_Conn = tcp_conn::connect(
                        cn.toStdString(),
                        user.toStdString(),
                        passwd.toStdString(),
                        0,
                        &conn_err);

            if (conn_err.get_code()){
                d_string msg = conn_err.get_msg();
                throw std::exception(msg.value().c_str());
            }else if (m_Conn->is_connected()){
                this->cn = cn;
                this->user = user;
                this->passwd = passwd;
                delete m_db;
                m_db = new Database(m_Conn);
                installCacheBackend();
            }
        }
    }

    void disconnect()
    {
        m_QtCacheTool = QtCacheToolType();
        m_Conn = d_connection();
    }

    bool isConnected() const
    {
        return m_Conn->is_connected() && !m_QtCacheTool.is_null();
    }

    void execute(const QString& code)
    {
        QtCacheToolType qct = tool();
        d_status sc = qct->Execute(
                    d_string(uci.toStdString()),
                    d_string(code.toStdString()));
        if (sc.get_code()){
            d_string msg = sc.get_msg();
            throw std::exception(msg.value().c_str());
        }
    }

private:
    Database* m_db = NULL;
    d_connection m_Conn;
    QtCacheToolType m_QtCacheTool;

    void installCacheBackend()
    {
#ifdef CACHEVISM
        VISM::_DVisM vism;
        vism.setControl("{88f75483-0574-11d0-8085-0000c0bd354b}");
#endif
    }
};

#endif // QTCACHE_P_H

