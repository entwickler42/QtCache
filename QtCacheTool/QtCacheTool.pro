QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtCache
TEMPLATE = app

DESTDIR = ../build

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += ../QtCache ../QtCacheUi

win32:LIBS += "-L$(DESTDIR)" -lQtCache -lQtCacheUi

RESOURCES += \
    resources.qrc

RC_FILE = qtcachetool.rc

include(../Intersystems/Intersystems.pri)

