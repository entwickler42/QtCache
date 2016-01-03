#include "qtcacheplugin.h"

QTCACHENAMESPACEUSE

const char* Plugin::FactoryFunctionSymbol = "createInstance";

Plugin::Plugin(QObject *parent) : QObject(parent)
{
}

