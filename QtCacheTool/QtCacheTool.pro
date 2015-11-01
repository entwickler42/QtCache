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

system(lupdate QtCacheTool.pro)
system(lrelease QtCacheTool.pro)

TRANSLATIONS = qtcachetool_de_DE.ts

OTHER_FILES += qtcachetool_de_DE.ts

include(../Intersystems/Intersystems.pri)

