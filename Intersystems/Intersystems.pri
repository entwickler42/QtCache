CONFIG += cppbind exceptions rtti

INCLUDEPATH += $$PWD

RESOURCES += \
    $$PWD/Intersystems.qrc

OTHER_FILES += $$PWD/QtCache.xml

cppbind{
    CONFIG(debug, debug|release){
        win32:CACHELIB=cppbind_msvc120d
        linux-g++:CACHELIB=cppbind
    }else{
        win32:CACHELIB=cppbind_msvc120
        linux-g++:CACHELIB=cppbind
    }
    win32{
        CACHEDIR=c:/InterSystems/Cache
        LIBS += "-L$$CACHEDIR/dev/cpp/lib" -l$$CACHELIB
    }
    linux-g++{
        CACHEDIR=/usr/cachesys
        LIBS += "-L$$CACHEDIR/bin" -lcppbind
    }
    DEFINES += CACHECPPBIND
    SOURCES += $$PWD/cppbind/Qt_CacheTool.cpp
    HEADERS += $$PWD/cppbind/Qt_CacheTool.h
    INCLUDEPATH *= $$PWD/cppbind/
    INCLUDEPATH *= "$$CACHEDIR/dev/cpp/include/"
}
