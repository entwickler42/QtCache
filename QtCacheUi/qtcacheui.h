#ifndef QTCACHEUI_H
#define QTCACHEUI_H

#include "qtcacheui_global.h"
#include <QStringList>

class QTranslator;

class QTCACHEUISHARED_EXPORT QtCacheUi
{
public:
    static QStringList defaultNameFilters();
    static QTranslator* createTranslator(const QString& lang);

private:
    QtCacheUi();
};

#endif // QTCACHEUI_H
