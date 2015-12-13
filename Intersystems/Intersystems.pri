CONFIG += cppbind exceptions rtti

INCLUDEPATH += $$PWD

RESOURCES += \
    $$PWD/Intersystems.qrc

OTHER_FILES += $$PWD/QtCache.xml

cppbind{
    CONFIG(debug, debug|release){
        win32:LIBS += "-Lc:/InterSystems/Cache/dev/cpp/lib/" -lcppbind_msvc120d
        win64:LIBS += "-Lc:/InterSystems/Cache/dev/cpp/lib/" -lcppbind_msvc120d64
        linux-g++:LIBS += "-L/usr/cachesys/bin" -lcppbind
    }else{
        win32:LIBS += "-Lc:/InterSystems/Cache/dev/cpp/lib/" -lcppbind_msvc120
        win64:LIBS += "-Lc:/InterSystems/Cache/dev/cpp/lib/" -lcppbind_msvc120r64
        linux-g++:LIBS += "-L/usr/cachesys/bin" -lcppbind
    }
    win32:INCLUDEPATH *= "c:/InterSystems/Cache/dev/cpp/include/"
    win64:INCLUDEPATH *= "c:/InterSystems/Cache/dev/cpp/include/"
    linux-g++:INCLUDEPATH *= "/usr/cachesys/dev/cpp/include/"
    INCLUDEPATH += $$PWD/cppbind/
    DEFINES += CACHECPPBIND
    SOURCES += $$PWD/cppbind/Qt_CacheTool.cpp
    HEADERS += $$PWD/cppbind/Qt_CacheTool.h
}

cbind{
    LIBS += "-Lc:/InterSystems/Cache/dev/cpp/lib/" -lcbind
    INCLUDEPATH *= "c:/InterSystems/Cache/dev/cpp/include/"
    DEFINES += CACHECBIND
}

vismocx{
    QT += axcontainer
    DEFINES += VISM_EXPORT=__declspec(dllexport)
    DEFINES += CACHEVISM
    INCLUDEPATH += $$PWD/vism/
    SOURCES += $$PWD/vism/vismocx.cpp
    HEADERS += $$PWD/vism/vismocx.h
}
