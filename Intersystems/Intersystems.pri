CONFIG += cppbind exceptions rtti

INCLUDEPATH += $$PWD

RESOURCES += \
    $$PWD/Intersystems.qrc

OTHER_FILES += $$PWD/QtCache.xml

cppbind{
    cache2008{
        win32:CACHEDIR=c:/InterSystems/Cache2008
        linux-g++:CACHEDIR=/usr/cachesys
        CONFIG(debug, debug|release){
            CACHELIB=cppbind_msvc80d
        }else{
            CACHELIB=cppbind_msvc80
        }
        DEFINES += CACHE2008
        INCLUDEPATH += $$PWD/cppbind/2008
        SOURCES += $$PWD/cppbind/2008/Qt_CacheTool.cpp
        HEADERS += $$PWD/cppbind/2008/Qt_CacheTool.h
    }else{
        win32:CACHEDIR=c:/InterSystems/Cache
        linux-g++:CACHEDIR=/usr/cachesys
        CONFIG(debug, debug|release){
            win32:CACHELIB=cppbind_msvc120d
            linux-g++:CACHELIB=cppbind
        }else{
            win32:CACHELIB=cppbind_msvc120
            linux-g++:CACHELIB=cppbind
        }
        INCLUDEPATH += $$PWD/cppbind/
        SOURCES += $$PWD/cppbind/Qt_CacheTool.cpp
        HEADERS += $$PWD/cppbind/Qt_CacheTool.h
    }
    win32:LIBS += "-L$$CACHEDIR/dev/cpp/lib" -l$$CACHELIB
    linux-g++:LIBS += "-L$$CACHEDIR/bin" -lcppbind

    INCLUDEPATH *= "$$CACHEDIR/dev/cpp/include/"
    DEFINES += CACHECPPBIND
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
