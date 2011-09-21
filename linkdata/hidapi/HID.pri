# We don't want warnings from 3rd party C code
QMAKE_CFLAGS_WARN_ON = -w

INCLUDEPATH += $$PWD
HEADERS += $$PWD/hidapi.h

win32 {
        SOURCES += $$PWD/win/hid.cpp
	LIBS += -lsetupapi
}

macx {
        SOURCES += $$PWD/osx/hid.c
	LIBS += -framework CoreFoundation -framework IOKit
}

unix:!macx {
        SOURCES += $$PWD/lin/hid.c
        LIBS += -ludev
}
