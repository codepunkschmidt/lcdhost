
INCLUDEPATH += $$PWD/libusb $$PWD/libusb
HEADERS += $$PWD/libusb/libusb.h
SOURCES += $$PWD/libusb/core.c $$PWD/libusb/descriptor.c $$PWD/libusb/io.c $$PWD/libusb/sync.c

win32 {
        DEFINES += OS_WINDOWS
        SOURCES += $$PWD/libusb/os/windows_usb.c $$PWD/libusb/os/threads_windows.c $$PWD/libusb/os/poll_windows.c
        LIBS += -lole32
}

macx {
        DEFINES += OS_DARWIN THREADS_POSIX
        SOURCES += $$PWD/libusb/os/darwin_usb.c
	LIBS += -framework CoreFoundation -framework IOKit
}

unix:!macx {
        DEFINES += OS_LINUX THREADS_POSIX
        SOURCES += $$PWD/libusb/os/linux_usbfs.c
}
