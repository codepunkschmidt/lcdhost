QMAKE_CFLAGS_WARN_ON = -w

USB_DIR=$$PWD/../../lh_libusb
INCLUDEPATH += $$USB_DIR
HEADERS += $$USB_DIR/libusb.h
SOURCES += $$USB_DIR/core.c $$USB_DIR/descriptor.c $$USB_DIR/io.c $$USB_DIR/sync.c

win32 {
        DEFINES += OS_WINDOWS
        SOURCES += $$USB_DIR/os/windows_usb.c $$USB_DIR/os/threads_windows.c $$USB_DIR/os/poll_windows.c
		LIBS += -lole32 -lsetupapi -lcfgmgr32
}

macx {
        DEFINES += OS_DARWIN THREADS_POSIX HAVE_SYS_TIME_H HAVE_POLL_H
        SOURCES += $$USB_DIR/os/darwin_usb.c
        LIBS += -framework CoreFoundation -framework IOKit
}

unix:!macx {
        DEFINES += OS_LINUX THREADS_POSIX HAVE_SYS_TIME_H HAVE_POLL_H
        SOURCES += $$USB_DIR/os/linux_usbfs.c
}
