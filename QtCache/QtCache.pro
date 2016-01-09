QT -= gui
TEMPLATE = lib
TARGET = QtCache

DESTDIR = ../build

#win32:VERSION = 1.0.3.0
#else:VERSION = 1.0.3

DEFINES += QTCACHE_LIBRARY

SOURCES += \
    poormanslogger.cpp \
    qtcacheplugin.cpp \
    qtcachepluginobserver.cpp \
    qtcache.cpp \
    qtcacheexception.cpp \
    qtcachexmlobject.cpp \
    qtcachexmlreader.cpp \
    qtcachebulkimport.cpp

HEADERS += \
    qtcache_global.h \
    qtcache_config.h \
    poormanslogger.h \
    qtcacheplugin.h \
    qtcachepluginobserver.h \
    qtcache_p.h \
    qtcache.h \
    qtcacheexception.h \
    qtcachexmlobject.h \
    qtcachexmlreader.h \
    qtcachebulkimport.h \
    qtcachebulkimportprogress.h \
    qtcacheprogress.h

TRANSLATIONS = qtcache_de_DE.ts

RESOURCES += \
    QtCache.qrc

OTHER_FILES += qtcache_de_DE.ts QtCache.pri

CONFIG(debug, debug|release){
}else{
    #system(lupdate -no-obsolete QtCache.pro)
    #system(lrelease QtCache.pro)
}

include(../Intersystems/Intersystems.pri)
