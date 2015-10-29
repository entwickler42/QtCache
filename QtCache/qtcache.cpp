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

#include "qtcache.h"
#include <exception>

#ifdef CACHECPPBIND
#include <Qt_CacheTool.h>
#endif

#ifdef CACHEVISM
#include <vismocx.h>
#endif

/* QtCachePrivate Implementation */
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
    {}

    QtCacheToolType tool()
    {
        if (connect(cn, uci, user, passwd)){
            if (m_QtCacheTool.is_null()) {
                Database db(m_Conn);
                m_QtCacheTool = Qt_CacheTool::create_new(&db);
            }
        }
        return m_QtCacheTool;
    }

    void installCacheBackend()
    {
        #ifdef CACHEVISM
        VISM::_DVisM vism;
        vism.setControl("{88f75483-0574-11d0-8085-0000c0bd354b}");
        #endif
    }

    bool connect(const QString& cn, const QString& uci, const QString& user, const QString& passwd, bool forceNew = false)
    {
        forceNew = (
            cn != this->cn ||
            uci != this->uci ||
            user != this->user ||
            passwd != this->passwd
        );
        if (forceNew || !m_Conn->is_connected()) {
            this->cn = cn;
            this->uci = uci;
            this->user = user;
            this->passwd = passwd;

            Db_err conn_err;

            try{
                m_Conn = tcp_conn::connect(
                            "localhost[1972]:Samples",
                            "_SYSTEM", "SYS", 0, &conn_err
                            );
            }catch(...){
                return false;
            }
        }

        return m_Conn->is_connected();
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

    d_connection m_Conn;
    QtCacheToolType m_QtCacheTool;
};

/* QtCache Implementation */

QtCache::QtCache()
    : d(new QtCachePrivate)
{}

QtCache::~QtCache()
{
    delete d;
}

QtCache* QtCache::instance()
{
    static QtCache* i = NULL;

    if (NULL == i){
        i = new QtCache();
    }

    return i;
}

bool QtCache::connect(const QString& cn, const QString& user, const QString& passwd)
{
    return d->connect(cn, this->uci(), user, passwd);
}

void QtCache::disconnect()
{
    d->disconnect();
}

bool QtCache::isConnected() const
{
    return d->isConnected();
}

const QString& QtCache::uci() const
{
    return d->uci;
}

void QtCache::setUci(const QString& uci)
{
    d->uci = uci;
}

int QtCache::execute(const QString& code)
{
    QtCacheToolType qct = d->tool();

    d_status sc = qct->Execute(
                d_string(d->uci.toStdString()),
                d_string(code.toStdString())
                );

    return 0;
}
