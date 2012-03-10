#
# libusb.pro
#
# Builds the libusb library
#

TARGET = lh_libusb
TEMPLATE = lib
CONFIG += staticlib

load($$PWD/../../lcdhost.prf)

QMAKE_CFLAGS_WARN_ON = -w

HEADERS += libusb.h
SOURCES += core.c descriptor.c io.c sync.c

win32 {
        DEFINES += OS_WINDOWS
        SOURCES += os/windows_usb.c os/threads_windows.c os/poll_windows.c
}

macx {
        DEFINES += OS_DARWIN THREADS_POSIX HAVE_SYS_TIME_H HAVE_POLL_H
        SOURCES += os/darwin_usb.c os/threads_posix.c
}

unix:!macx {
        DEFINES += OS_LINUX THREADS_POSIX HAVE_SYS_TIME_H HAVE_POLL_H
        SOURCES += os/linux_usbfs.c os/threads_posix.c
}

