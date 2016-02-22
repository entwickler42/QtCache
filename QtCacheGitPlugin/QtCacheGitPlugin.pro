QT       -= gui
TEMPLATE = lib
TARGET = QtCacheGitPlugin

DESTDIR = ../build/plugins

DEFINES += QTCACHEGITPLUGIN_LIBRARY

SOURCES += \
    qtcachegitplugin.cpp \
    gitrepository.cpp

HEADERS += \
    qtcachegitplugin.h \
    qtcachegitplugin_global.h \
    gitrepository.h

include(../QtCache/QtCache.pri)

win32:INCLUDEPATH += $$PWD/deps/libgit2/include/
win32:LIBS += "-L$$PWD/deps/libgit2/msvc2013.x86" -lgit2
