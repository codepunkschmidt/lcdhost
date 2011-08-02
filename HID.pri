# We don't want warnings from 3rd party C code
QMAKE_CFLAGS_WARN_ON = -w

INCLUDEPATH += $$PWD/hidapi
HEADERS += $$PWD/hidapi/hidapi.h \
    ../LH_Qt_array_int.h \
    ../LH_Qt_array_double.h \
    ../LH_Qt_array.h \
    ../LH_QtMemory.h

win32 {
        SOURCES += $$PWD/hidapi/win/hid.cpp
	LIBS += -lsetupapi
}

macx {
        SOURCES += $$PWD/hidapi/osx/hid.c
	LIBS += -framework CoreFoundation -framework IOKit
}

unix:!macx {
        SOURCES += $$PWD/hidapi/lin/hid.c
        LIBS += -ludev
}

SOURCES += \
    ../LH_Qt_array_int.cpp \
    ../LH_QtMemory.cpp
