QT       -= gui
TEMPLATE = lib
TARGET = QtCachePluginSample

DESTDIR = ../build/plugins

DEFINES += QTCACHEPLUGINSAMPLE_LIBRARY

SOURCES += qtcachepluginsample.cpp

HEADERS += \
    qtcachepluginsample_global.h \
    qtcachepluginsample.h

INCLUDEPATH += "../QtCache"

win32:LIBS += "-L../build" -lQtCache
