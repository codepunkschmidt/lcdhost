#
# lh_api5.pro
#
# Builds the LCDHost plugin API5 library
#

TARGET = lh_api5
TEMPLATE = lib
CONFIG += staticlib

load($$PWD/../../lcdhost.prf)

HEADERS += \
    lh_api5.h \
    lh_plugin.h \
    QtSupport/LH_Qt_bool.h \
    QtSupport/LH_Qt_float.h \
    QtSupport/LH_Qt_InputState.h \
    QtSupport/LH_Qt_InputValue.h \
    QtSupport/LH_Qt_int.h \
    QtSupport/LH_Qt_QColor.h \
    QtSupport/LH_Qt_QFileInfo.h \
    QtSupport/LH_Qt_QFont.h \
    QtSupport/LH_Qt_QImage.h \
    QtSupport/LH_Qt_QProgressBar.h \
    QtSupport/LH_Qt_QSlider.h \
    QtSupport/LH_Qt_QString.h \
    QtSupport/LH_Qt_QStringList.h \
    QtSupport/LH_Qt_QTextEdit.h \
    QtSupport/LH_QtCPU.h \
    QtSupport/LH_QtDevice.h \
    QtSupport/LH_QtInstance.h \
    QtSupport/LH_QtNetwork.h \
    QtSupport/LH_QtObject.h \
    QtSupport/LH_QtPlugin.h \
    QtSupport/LH_QtSetupItem.h

SOURCES += \
    lh_plugin.c \
    QtSupport/LH_QtCPU.cpp \
    QtSupport/LH_QtDevice.cpp \
    QtSupport/LH_QtInstance.cpp \
    QtSupport/LH_QtNetwork.cpp \
    QtSupport/LH_QtObject.cpp \
    QtSupport/LH_QtPlugin.cpp \
    QtSupport/LH_QtSetupItem.cpp




