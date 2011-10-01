#
# hidapi.pro
#
# Builds the HID API library
#

TARGET = lh_hidapi
TEMPLATE = lib
CONFIG += staticlib

load($$PWD/../../lcdhost.prf)

QMAKE_CFLAGS_WARN_ON = -w

HEADERS += hidapi.h

win32:SOURCES += win/hid.cpp
macx:SOURCES += osx/hid.c
unix:!macx:SOURCES += lin/hid.c

