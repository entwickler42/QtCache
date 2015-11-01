#include "qtcacheui.h"

#include <QFile>
#include <QTranslator>


QtCacheUi::QtCacheUi()
{}

QStringList QtCacheUi::defaultNameFilters()
{
    return QStringList()
            << QObject::tr("Caché (*.xml *.mac *.int *.cls *.inc *.gbl)", "QtCacheUi")
            << QObject::tr("Caché Sources (*.mac *.int *.cls *.inc)", "QtCacheUi")
            << QObject::tr("Caché Globals (*.gbl)", "QtCacheUi")
            << QObject::tr("Any Files (*)", "QtCacheUi");
}

QTranslator* QtCacheUi::createTranslator(const QString& lang)
{
    QString filepath = QString(":/i18n/qtcacheui_%1.qm").arg(lang);
    if (QFile::exists(filepath)){
        QTranslator* t = new QTranslator();
        if (t->load(filepath)){
            return t;
        }else{
            delete t;
        }
    }
    return NULL;
}
