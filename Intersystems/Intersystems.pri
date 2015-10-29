INCLUDEPATH += $$PWD

OTHER_FILES += $$PWD/QtCache.xml

cppbind{
    debug:LIBS += "-Lc:/InterSystems/TryCache/dev/cpp/lib/" -lcppbind_msvc120d
    release:LIBS += "-Lc:/InterSystems/TryCache/dev/cpp/lib/" -lcppbind_msvc120
    INCLUDEPATH *= "c:/InterSystems/TryCache/dev/cpp/include/"
    INCLUDEPATH += $$PWD/cppbind/
    DEFINES += CACHECPPBIND
    SOURCES += $$PWD/cppbind/Qt_CacheTool.cpp
    HEADERS += $$PWD/cppbind/Qt_CacheTool.h
}

cbind{
    LIBS += "-Lc:/InterSystems/TryCache/dev/cpp/lib/" -lcbind
    INCLUDEPATH *= "c:/InterSystems/TryCache/dev/cpp/include/"
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
