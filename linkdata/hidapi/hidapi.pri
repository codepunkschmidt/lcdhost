
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
}
