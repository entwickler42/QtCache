QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtCacheUi
TEMPLATE = lib

DESTDIR = ../build

DEFINES += QTCACHEUI_LIBRARY

SOURCES += \
    cacheconnectiondialog.cpp

HEADERS +=\
        qtcacheui_global.h \
    cacheconnectiondialog.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += ../QtCache

win32:LIBS += "-L$(DESTDIR)" -lQtCache

FORMS += \
    cacheconnectiondialog.ui

RESOURCES += \
    resources.qrc
