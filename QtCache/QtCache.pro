QT       -= gui

TARGET = QtCache
TEMPLATE = lib

DESTDIR = ../build

DEFINES += QTCACHE_LIBRARY

SOURCES += qtcache.cpp \
    poormanslogger.cpp

HEADERS += qtcache.h \
           qtcache_global.h \
    qtcache_p.h \
    qtcacheexception.h \
    poormanslogger.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

include(../Intersystems/Intersystems.pri)
