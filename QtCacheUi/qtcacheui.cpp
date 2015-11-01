#include "qtcacheui.h"
#include <QObject>

QtCacheUi::QtCacheUi()
{}

QStringList QtCacheUi::defaultNameFilters()
{
    return QStringList()
            << QObject::tr("Caché (*.xml *.mac *.int *.cls *.gbl)", "QtCacheUi")
            << QObject::tr("Caché Sources (*.mac *.int *.cls )", "QtCacheUi")
            << QObject::tr("Caché Globals (*.gbl)", "QtCacheUi")
            << QObject::tr("Any Files (*)", "QtCacheUi");
}

