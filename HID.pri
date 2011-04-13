
INCLUDEPATH += $$PWD/hidapi
HEADERS += $$PWD/hidapi/hidapi.h

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
}
