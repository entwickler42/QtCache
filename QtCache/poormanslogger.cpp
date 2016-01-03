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
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace PML;

PoorMansLogger PML::LOG("error.log");

PoorMansLogger::PoorMansLogger(const QString& filepath)
{
    QString _filepath(filepath);
    int FH = -1;
    int retry = 3;
    do{
        FH = open(qPrintable(_filepath), O_WRONLY | O_APPEND | O_CREAT);
        _filepath.replace(".","_.");
    }while(retry-- && FH < 0);

    if (FH > -1){
        dup2(FH, fileno(stderr));
        dup2(FH, fileno(stdout));
        close(FH);
    }
}

PoorMansLogger::~PoorMansLogger()
{}

