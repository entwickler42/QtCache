QT       -= gui
TEMPLATE = lib
TARGET = QtCachePluginSample

DESTDIR = ../build/plugins

DEFINES += QTCACHEPLUGINSAMPLE_LIBRARY

SOURCES += qtcachepluginsample.cpp

HEADERS += \
    qtcachepluginsample_global.h \
    qtcachepluginsample.h

include(../QtCache/QtCache.pri)
