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

#include "poormanslogger.h"
#include <io.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <QDate>

PoorMansLogger::PoorMansLogger(const QString& filepath)
{
    // unlink(qPrintable(QString(filepath).prepend("previous_")));
    // rename(qPrintable(filepath), qPrintable(QString(filepath).prepend("previous_")));
    QString _filepath(filepath);
    _filepath.prepend('_').prepend(QDate::currentDate().toString(Qt::ISODate));

    FH = open(qPrintable(_filepath), O_WRONLY | O_APPEND | O_CREAT);
    dup2(FH, fileno(stderr));
    dup2(FH, fileno(stdout));
}

PoorMansLogger::~PoorMansLogger()
{
    close(FH);
}

