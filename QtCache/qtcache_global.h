#ifndef QTCACHE_GLOBAL_H
#define QTCACHE_GLOBAL_H

#include <QtCore/qglobal.h>
#include "qtcache_config.h"

#if defined(QTCACHE_LIBRARY)
#  define QTCACHESHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTCACHESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QTCACHE_GLOBAL_H
