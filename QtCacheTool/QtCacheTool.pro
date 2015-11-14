QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = app
TARGET = QtCache

DESTDIR = ../build

win32:VERSION = 1.0.3.0
else:VERSION = 1.0.3

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    config.h

FORMS    += mainwindow.ui

INCLUDEPATH += ../QtCache ../QtCacheUi

win32:LIBS += "-L$(DESTDIR)" -lQtCache -lQtCacheUi

RESOURCES += \
    resources.qrc

RC_FILE = qtcachetool.rc

system(lupdate -no-obsolete QtCacheTool.pro)
system(lrelease QtCacheTool.pro)

TRANSLATIONS = qtcachetool_de_DE.ts

OTHER_FILES += qtcachetool_de_DE.ts \
    qtcachetool.rc

# include(../Intersystems/Intersystems.pri)

