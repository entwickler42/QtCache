QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = app
TARGET = QtCache

DESTDIR = ../build

win32:VERSION = 1.0.3.0
else:VERSION = 1.0.3

SOURCES += main.cpp\
        mainwindow.cpp \
    qtcachetoolsettings.cpp

HEADERS  += mainwindow.h \
    config.h \
    qtcachetoolsettings.h

FORMS    += mainwindow.ui

TRANSLATIONS = qtcachetool_de_DE.ts

RESOURCES += \
    QtCacheTool.qrc

RC_FILE = qtcachetool.rc

OTHER_FILES += qtcachetool_de_DE.ts \
    qtcachetool.rc

INCLUDEPATH += ../QtCache ../QtCacheUi

win32:LIBS += "-L$(DESTDIR)" -lQtCache -lQtCacheUi
linux-g++|linux-g++-64:LIBS += "-L$(DESTDIR)" -lQtCache -lQtCacheUi

QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01

CONFIG(debug, debug|release){
}else{
    #system(lupdate -no-obsolete QtCacheTool.pro)
    #system(lrelease QtCacheTool.pro)
}

include(../Intersystems/Intersystems.pri)
