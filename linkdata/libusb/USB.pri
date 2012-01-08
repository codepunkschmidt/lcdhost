
# We don't want warnings from 3rd party C code
QMAKE_CFLAGS_WARN_ON = -w

INCLUDEPATH += $$PWD
HEADERS += $$PWD/libusb.h
SOURCES += $$PWD/core.c $$PWD/descriptor.c $$PWD/io.c $$PWD/sync.c

win32 {
        DEFINES += OS_WINDOWS
        SOURCES += $$PWD/os/windows_usb.c $$PWD/os/threads_windows.c $$PWD/os/poll_windows.c
        LIBS += -lole32 -lsetupapi -lcfgmgr32
}

macx {
        DEFINES += OS_DARWIN THREADS_POSIX HAVE_SYS_TIME_H HAVE_POLL_H
        SOURCES += $$PWD/os/darwin_usb.c
	LIBS += -framework CoreFoundation -framework IOKit
}

unix:!macx {
        DEFINES += OS_LINUX THREADS_POSIX HAVE_SYS_TIME_H HAVE_POLL_H
        SOURCES += $$PWD/os/linux_usbfs.c
}
