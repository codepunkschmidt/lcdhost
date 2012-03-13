#
# hidapi.pro
#
# Builds the HID API library
#

TARGET = lh_hidapi
TEMPLATE = lib
CONFIG += staticlib

load($$[LCDHOST_PRF])

QMAKE_CFLAGS_WARN_ON = -w

HEADERS += hidapi.h

win32:SOURCES += windows/hid.c
macx:SOURCES += mac/hid.c
unix:!macx:SOURCES += linux/hid.c
