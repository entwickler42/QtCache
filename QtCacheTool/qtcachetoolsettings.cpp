#include "qtcachetoolsettings.h"

QtCacheToolSettings::QtCacheToolSettings(QObject* parent, const QString& filename)
    : QObject(parent),
      conf(new QSettings(filename, QSettings::IniFormat, this))
{
}

QtCacheToolSettings::~QtCacheToolSettings()
{
    conf->sync();
}

