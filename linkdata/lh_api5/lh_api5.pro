#
# lh_api5.pro
#
# Builds the LCDHost plugin API5 -> API6 compatibility library
#

TARGET = lh_api5
TEMPLATE = lib
CONFIG += staticlib

load($$PWD/../../PluginsConfig.prf)

HEADERS += \
    lh_api5.h \
    lh_api5_plugin.h \
    LH_Qt_bool.h \
    LH_Qt_float.h \
    LH_Qt_InputState.h \
    LH_Qt_InputValue.h \
    LH_Qt_int.h \
    LH_Qt_QColor.h \
    LH_Qt_QFileInfo.h \
    LH_Qt_QFont.h \
    LH_Qt_QImage.h \
    LH_Qt_QProgressBar.h \
    LH_Qt_QSlider.h \
    LH_Qt_QString.h \
    LH_Qt_QStringList.h \
    LH_Qt_QTextEdit.h \
    LH_QtCPU.h \
    LH_QtDevice.h \
    LH_QtInstance.h \
    LH_QtNetwork.h \
    LH_QtObject.h \
    LH_QtPlugin.h \
    LH_QtSetupItem.h

SOURCES += \
    lh_api5_plugin.c \
    LH_QtCPU.cpp \
    LH_QtDevice.cpp \
    LH_QtInstance.cpp \
    LH_QtNetwork.cpp \
    LH_QtObject.cpp \
    LH_QtPlugin.cpp \
    LH_QtSetupItem.cpp



