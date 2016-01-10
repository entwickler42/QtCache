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

#ifndef QTCACHE_GLOBAL_H
#define QTCACHE_GLOBAL_H

#include <QtCore/qglobal.h>
#include "qtcache_config.h"

#if defined(QTCACHE_LIBRARY)
#  define QTCACHESHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTCACHESHARED_EXPORT Q_DECL_IMPORT
#endif

#define LOG_EXCEPTION(EX) PML::LOG << __FILE__ << " " << __FUNCTION__ << " Line " << __LINE__ << " " << EX.what();
#define LOG_UNKNOWN_EXCEPTION PML::LOG << __FILE__ << " " << __FUNCTION__ << " Line " << __LINE__ << " unknown exception";

#endif // QTCACHE_GLOBAL_H
