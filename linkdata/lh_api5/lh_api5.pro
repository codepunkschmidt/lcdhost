#
# lh_api5.pro
#
# Builds the LCDHost plugin API5 library
#

TARGET = lh_api5
TEMPLATE = lib
CONFIG += staticlib

load($$[LCDHOST_PRF])

OTHER_FILES += lh_api5.prf

HEADERS += \
    lh_api5.h \
    lh_plugin.h \
    $$files(QtSupport/*.h)

SOURCES += \
    lh_plugin.c \
    $$files(QtSupport/*.cpp)
