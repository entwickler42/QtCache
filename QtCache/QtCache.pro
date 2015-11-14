QT -= gui
TEMPLATE = lib
TARGET = QtCache

DESTDIR = ../build

win32:VERSION = 1.0.3.0
else:VERSION = 1.0.3

DEFINES += QTCACHE_LIBRARY

SOURCES += qtcache.cpp \
    poormanslogger.cpp \
    qtcacheexception.cpp

HEADERS += qtcache.h \
           qtcache_global.h \
    qtcache_p.h \
    qtcacheexception.h \
    poormanslogger.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

system(lupdate -no-obsolete QtCache.pro)
system(lrelease QtCache.pro)

TRANSLATIONS = qtcache_de_DE.ts

OTHER_FILES += qtcache_de_DE.ts

include(../Intersystems/Intersystems.pri)
