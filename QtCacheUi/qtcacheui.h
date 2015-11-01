#ifndef QTCACHEUI_H
#define QTCACHEUI_H

#include "qtcacheui_global.h"
#include <QStringList>


class QTCACHEUISHARED_EXPORT QtCacheUi
{
public:
    static QStringList defaultNameFilters();

private:
    QtCacheUi();
};

#endif // QTCACHEUI_H
