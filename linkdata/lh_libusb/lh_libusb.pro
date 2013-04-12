TEMPLATE = lib
CONFIG += shared lh_shared
TARGET = lh_libusb

QMAKE_CFLAGS_WARN_ON = -w

HEADERS += libusb.h
INCLUDEPATH *= $$PWD
SOURCES += core.c descriptor.c io.c sync.c

win32 {
    DEFINES += OS_WINDOWS
    SOURCES += os/windows_usb.c os/threads_windows.c os/poll_windows.c
    LIBS += -luser32 -lole32 -lsetupapi
# -lcfgmgr32
}

macx {
    DEFINES += OS_DARWIN THREADS_POSIX HAVE_SYS_TIME_H HAVE_POLL_H
    DEFINES += MAC_OS_X_VERSION_MIN_REQUIRED=1060
    SOURCES += os/darwin_usb.c os/threads_posix.c
    LIBS += -framework CoreFoundation -framework IOKit -lobjc
}

unix:!macx {
    DEFINES += OS_LINUX THREADS_POSIX HAVE_SYS_TIME_H HAVE_POLL_H
    SOURCES += os/linux_usbfs.c os/threads_posix.c
}

LH_SHARED_HEADERS += libusb.h

DESTDIR = $$lh_destdir()
