TARGET = LH_Image
TEMPLATE = lib
DEFINES += LH_IMAGE_LIBRARY

include(../Plugins.pri)

SOURCES += \
    LH_Image.c \
    ../linkdata/lh_plugin.c

HEADERS += \
    LH_Image.h \
    ../linkdata/lh_plugin.h
