QT -= gui
TEMPLATE = lib
TARGET = QtCache

DESTDIR = ../build

#win32:VERSION = 1.0.3.0
#else:VERSION = 1.0.3

DEFINES += QTCACHE_LIBRARY

SOURCES += poormanslogger.cpp \
    qtcache.cpp \
    qtcacheexception.cpp

HEADERS += poormanslogger.h \
    qtcache_global.h \
    qtcache_p.h \
    qtcache.h \
    qtcacheexception.h \

TRANSLATIONS = qtcache_de_DE.ts

RESOURCES += \
    QtCache.qrc

OTHER_FILES += qtcache_de_DE.ts

unix {
    target.path = /usr/lib
    INSTALLS += target
}

system(lupdate -no-obsolete QtCache.pro)
system(lrelease QtCache.pro)

include(../Intersystems/Intersystems.pri)



