#
# hidapi.pri
#
# Johan Lindh <johan@linkdata.se> is the sole maintainer of this
# file and all the files in this directory and subdirectories.
# Do *NOT* commit here unless you have his explicit approval to do so.
#
# If you find a bug or need a feature or change, submit an issue
# at "http://code.google.com/p/lcdhost/issues/list" and label it
# "linkdata-hidapi".
#

# We don't want warnings from 3rd party C code
QMAKE_CFLAGS_WARN_ON = -w

HIDAPI_DIR = $$PWD/hidapi
INCLUDEPATH += $$HIDAPI_DIR
HEADERS += $$HIDAPI_DIR/hidapi.h

win32 {
        SOURCES += $$HIDAPI_DIR/win/hid.cpp
	LIBS += -lsetupapi
}

macx {
        SOURCES += $$HIDAPI_DIR/osx/hid.c
	LIBS += -framework CoreFoundation -framework IOKit
}

unix:!macx {
        SOURCES += $$HIDAPI_DIR/lin/hid.c
        LIBS += -ludev
}
