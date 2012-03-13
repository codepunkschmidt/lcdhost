
INCLUDEPATH += $$PWD
HEADERS += $$PWD/hidapi.h

win32 {
        SOURCES += $$PWD/win/hid.c
	LIBS += -lsetupapi
}

macx {
        SOURCES += $$PWD/osx/hid.c
	LIBS += -framework CoreFoundation -framework IOKit
}

unix:!macx {
        SOURCES += $$PWD/lin/hid.c
}
