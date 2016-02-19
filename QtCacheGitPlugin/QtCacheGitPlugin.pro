QT       -= gui
TEMPLATE = lib
TARGET = QtCacheGitPlugin

DESTDIR = ../build/plugins

DEFINES += QTCACHEGITPLUGIN_LIBRARY

SOURCES += \
    qtcachegitplugin.cpp

HEADERS += \
    qtcachegitplugin.h \
    qtcachegitplugin_global.h

include(../QtCache/QtCache.pri)

WIN32:INCLUDEPATH += $$PWD/deps/libgit2//include/
WIN32:LIBS += "-L$$PWD/deps/libgit2/msvc2013.x86" -lgit2
