QMAKE_CFLAGS_WARN_ON = -w

HID_DIR=$$PWD/../../lh_hidapi
INCLUDEPATH += $$HID_DIR
LIBS += -llh_hidapi

# HEADERS += $$HID_DIR/hidapi.h
# win32:SOURCES += $$HID_DIR/win/hid.cpp
# macx:SOURCES += $$HID_DIR/osx/hid.c
# unix:!macx:SOURCES += $$HID_DIR/lin/hid.c

win32:LIBS += -lsetupapi
macx: LIBS += -framework CoreFoundation -framework IOKit
unix:!macx: LIBS += -ludev
