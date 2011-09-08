
# We don't want warnings from 3rd party C code
QMAKE_CFLAGS_WARN_ON = -w

LIBUSB_DIR = $$PWD/libusb
INCLUDEPATH += $$LIBUSB_DIR
HEADERS += $$LIBUSB_DIR/libusb.h
SOURCES += $$LIBUSB_DIR/core.c $$LIBUSB_DIR/descriptor.c $$LIBUSB_DIR/io.c $$LIBUSB_DIR/sync.c

win32 {
        DEFINES += OS_WINDOWS
        SOURCES += $$LIBUSB_DIR/os/windows_usb.c $$LIBUSB_DIR/os/threads_windows.c $$LIBUSB_DIR/os/poll_windows.c
        LIBS += -lole32 -lsetupapi -lcfgmgr32
}

macx {
        DEFINES += OS_DARWIN THREADS_POSIX HAVE_SYS_TIME_H HAVE_POLL_H
        SOURCES += $$LIBUSB_DIR/os/darwin_usb.c
	LIBS += -framework CoreFoundation -framework IOKit
}

unix:!macx {
        DEFINES += OS_LINUX THREADS_POSIX HAVE_SYS_TIME_H HAVE_POLL_H
        SOURCES += $$LIBUSB_DIR/os/linux_usbfs.c
}
