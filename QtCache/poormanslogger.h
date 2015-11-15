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

#ifndef POORMANSLOGGER_H
#define POORMANSLOGGER_H

#include "qtcache_global.h"
#include <QString>


class QTCACHESHARED_EXPORT PoorMansLogger
{
public:
    PoorMansLogger(const QString& filepath = "logfile.log");
    ~PoorMansLogger();
};

#endif // POORMANSLOGGER_H
