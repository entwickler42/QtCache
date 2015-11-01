QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtCacheUi
TEMPLATE = lib

DESTDIR = ../build

DEFINES += QTCACHEUI_LIBRARY

SOURCES += \
    cacheconnectiondialog.cpp \
    qtcacheui.cpp

HEADERS +=\
        qtcacheui_global.h \
    cacheconnectiondialog.h \
    qtcacheui.h

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

system(lupdate QtCacheUi.pro)
system(lrelease QtCacheUi.pro)

TRANSLATIONS = qtcacheui_de_DE.ts

OTHER_FILES += qtcacheui_de_DE.ts

include(../Intersystems/Intersystems.pri)

