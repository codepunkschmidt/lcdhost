TARGET = LH_Image
TEMPLATE = lib
DEFINES += LH_IMAGE_LIBRARY

include(../Plugins.pri)

SOURCES += \
    LH_Image.c \
    ../lh_plugin.c

HEADERS += \
    LH_Image.h \
    ../lh_plugin.h
