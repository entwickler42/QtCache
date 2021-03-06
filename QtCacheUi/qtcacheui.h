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

#ifndef QTCACHEUI_H
#define QTCACHEUI_H

#include "qtcacheui_global.h"
#include <QStringList>

class QTranslator;

QTCACHENAMESPACEBEGIN

class QTCACHEUISHARED_EXPORT QtCacheUi
{
public:
    static QStringList defaultNameFilters();

private:
    QtCacheUi();
};

QTCACHENAMESPACEEND

#endif // QTCACHEUI_H
