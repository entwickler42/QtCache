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

#include "qtcachetoolsettings.h"

QtCacheToolSettings::QtCacheToolSettings(QObject* parent, const QString& filename)
    : QObject(parent),
      conf(new QSettings(filename, QSettings::IniFormat, this))
{
    QMutableListIterator<init_property*> i(g_init_property);
    while (i.hasNext()){
        init_property* p = i.next();
        (*p)(this);
        i.remove();
    }
}

QtCacheToolSettings::~QtCacheToolSettings()
{
    conf->sync();
}

QList<QtCacheToolSettings::init_property*> QtCacheToolSettings::g_init_property;
