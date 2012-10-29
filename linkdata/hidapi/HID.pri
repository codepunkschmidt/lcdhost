# We don't want warnings from 3rd party C code
QMAKE_CFLAGS_WARN_ON = -w

INCLUDEPATH += $$PWD
HEADERS += $$PWD/hidapi.h

win32 {
        SOURCES += $$PWD/windows/hid.c
	LIBS += -lsetupapi
}

macx {
        SOURCES += $$PWD/mac/hid.c
	LIBS += -framework CoreFoundation -framework IOKit
}

unix:!macx {
        SOURCES += $$PWD/linux/hid.c
        LIBS += -ludev
}
