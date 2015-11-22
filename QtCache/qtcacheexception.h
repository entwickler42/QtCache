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

#include <QString>
#include <stdexcept>

namespace InterSystems {
    class d_string;
    class d_status;
    class Db_err;
}

class QtCacheException
        : public std::runtime_error
{
public:
    QtCacheException(const QString& msg)
        : QtCacheException(msg.toStdString().c_str())
    {}

    QtCacheException(const std::string& msg)
        : QtCacheException(msg.c_str())
    {}

    QtCacheException(const char* msg)
        : std::runtime_error(msg)
    {}

    QtCacheException(const InterSystems::d_string& msg);

    QtCacheException(const InterSystems::d_status& sc);

    QtCacheException(const InterSystems::Db_err& err);
};

#endif // QTCACHEEXCEPTION

