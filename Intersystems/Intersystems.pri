CONFIG += cppbind exceptions rtti

INCLUDEPATH += $$PWD

OTHER_FILES += $$PWD/QtCache.xml

cppbind{
    CONFIG(debug, debug|release){
        LIBS += "-Lc:/InterSystems/Cache/dev/cpp/lib/" -lcppbind_msvc120d
    }else{
        LIBS += "-Lc:/InterSystems/Cache/dev/cpp/lib/" -lcppbind_msvc120
    }
    INCLUDEPATH *= "c:/InterSystems/Cache/dev/cpp/include/"
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
