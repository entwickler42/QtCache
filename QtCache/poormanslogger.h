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
#include <iostream>


namespace PML {

class QTCACHESHARED_EXPORT PoorMansLogger
{
public:
    PoorMansLogger(const QString& filepath = "error.log");
    ~PoorMansLogger();

    const PoorMansLogger& operator << (const QString& s) const
    {
        std::cerr << qPrintable(s) << std::endl;
        return *this;
    }

    const PoorMansLogger& operator << (int i) const
    {
        return *this << QString::number(i);
    }
};

extern QTCACHESHARED_EXPORT PoorMansLogger LOG;

}


#endif // POORMANSLOGGER_H
