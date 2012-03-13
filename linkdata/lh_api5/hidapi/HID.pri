QMAKE_CFLAGS_WARN_ON = -w

HID_DIR=$$PWD/../../lh_hidapi
INCLUDEPATH += $$HID_DIR
HEADERS += $$HID_DIR/hidapi.h

win32:SOURCES += $$HID_DIR/windows/hid.c
macx:SOURCES += $$HID_DIR/mac/hid.c
unix:!macx:SOURCES += $$HID_DIR/linux/hid.c

win32:LIBS += -lsetupapi
macx: LIBS += -framework CoreFoundation -framework IOKit
unix:!macx: LIBS += -ludev
