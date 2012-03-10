#
# lh_api5.pro
#
# Builds the LCDHost plugin API5 library
#

TEMPLATE = lib
CONFIG += staticlib

load($$PWD/../../lcdhost.prf)
TARGET = lh_api5

HEADERS += \
    lh_api5.h \
    lh_plugin.h \
    $$files(QtSupport/*.h)

SOURCES += \
    lh_plugin.c \
    $$files(QtSupport/*.cpp)




