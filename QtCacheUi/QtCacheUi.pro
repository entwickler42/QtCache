QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = lib
TARGET = QtCacheUi

DESTDIR = ../build
DEFINES += QTCACHEUI_LIBRARY

#win32:VERSION = 1.0.3.0
#else:VERSION = 1.0.3

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

TRANSLATIONS = qtcacheui_de_DE.ts

RESOURCES += \
    QtCacheUi.qrc

OTHER_FILES += qtcacheui_de_DE.ts

CONFIG(release, debug|release){
    #system(lupdate -no-obsolete QtCacheUi.pro)
    #system(lrelease QtCacheUi.pro)
}

include(../Intersystems/Intersystems.pri)
