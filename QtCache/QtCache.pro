QT       -= gui

CONFIG += cppbind exceptions

TARGET = QtCache
TEMPLATE = lib

DESTDIR = ../build

DEFINES += QTCACHE_LIBRARY

SOURCES += qtcache.cpp

HEADERS += qtcache.h \
           qtcache_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

include(../Intersystems/Intersystems.pri)
