#ifndef QTCACHEUI_GLOBAL_H
#define QTCACHEUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTCACHEUI_LIBRARY)
#  define QTCACHEUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTCACHEUISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QTCACHEUI_GLOBAL_H
