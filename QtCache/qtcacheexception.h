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

#ifndef QTCACHEEXCEPTION
#define QTCACHEEXCEPTION

#include <Qt_CacheTool.h>
#include <QString>
#include <exception>


class QtCacheException
        : public std::exception
{
public:
    QtCacheException(const QString& msg)
        : std::exception(msg.toStdString().c_str())
    {}

    QtCacheException(const std::string& msg)
        : std::exception(msg.c_str())
    {}

    QtCacheException(const d_string& msg)
        : QtCacheException(msg.value())
    {}

    QtCacheException(const d_status& sc)
        : QtCacheException(sc.get_msg())
    {}

    QtCacheException(const Db_err& err)
        : QtCacheException(err.get_msg())
    {}
};

#endif // QTCACHEEXCEPTION

