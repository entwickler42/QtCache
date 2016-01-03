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
