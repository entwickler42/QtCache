QT -= gui
TEMPLATE = lib
TARGET = QtCache

DESTDIR = ../build

#win32:VERSION = 1.0.7.0
#else:VERSION = 1.0.7

DEFINES += QTCACHE_LIBRARY

SOURCES += \
    poormanslogger.cpp \
    qtcacheplugin.cpp \
    qtcache.cpp \
    qtcacheexception.cpp \
    qtcachexmlobject.cpp \
    qtcachexmlreader.cpp \
    qtcachebulkaction.cpp \
    qtcachebulkimport.cpp \
    qtcachebulkexport.cpp \
    qtcacheplugindirector.cpp \
    qtcacheprogressreporter.cpp

HEADERS += \
    qtcache_global.h \
    qtcache_config.h \
    poormanslogger.h \
    qtcacheplugin.h \
    qtcache_p.h \
    qtcache.h \
    qtcacheexception.h \
    qtcachexmlobject.h \
    qtcachexmlreader.h \
    qtcacheprogress.h \
    qtcachebulkaction.h \
    qtcachebulkimport.h \
    qtcachebulkexport.h \
    qtcacheplugindirector.h \
    qtcacheprogressreporter.h

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
