CONFIG += cppbind exceptions rtti cache2008

INCLUDEPATH += $$PWD

RESOURCES += \
    $$PWD/Intersystems.qrc

OTHER_FILES += $$PWD/QtCache.xml

cppbind{
    cache2008{
        win32:CACHEDIR=c:/InterSystems/Cache2008
        linux-g++:CACHEDIR=/usr/cachesys
        CONFIG(debug, debug|release){
            CACHELIB=cppbind_msvc80
        }else{
            CACHELIB=cppbind_msvc80d
        }
        DEFINES += CACHE2008
    }else{
        win32:CACHEDIR=c:/InterSystems/Cache
        linux-g++:CACHEDIR=/usr/cachesys
        CONFIG(debug, debug|release){
            win32:CACHELIB=cppbind_msvc120
            linux-g++:CACHELIB=cppbind
        }else{
            win32:CACHELIB=cppbind_msvc120d
            linux-g++:CACHELIB=cppbind
        }
    }
    win32:LIBS += "-L$$CACHEDIR/dev/cpp/lib" -l$$CACHELIB
    linux-g++:LIBS += "-L$$CACHEDIR/bin" -lcppbind

    INCLUDEPATH *= "$$CACHEDIR/dev/cpp/include/"
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
