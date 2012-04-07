#
# lh_api6.pro
#
# Builds the LCDHost plugin API6 support library
#

TARGET = lh_api6
TEMPLATE = lib
CONFIG += staticlib
INCLUDEPATH += $$PWD

load($$[LCDHOST_PRF])

OTHER_FILES += lh_api6.prf

HEADERS = \
    LH/Type.h \
    LH/Type_Map.h \
    LH/Event_Base.h \
    LH/Event_CreateChild.h \
    LH/Event_Log.h \
    LH/Event_InitChild.h \
    LH/Event_SetProperty.h \
    LH/Lib6.h \
    LH/Lib6_Id.h \
    LH/Lib6_Loader.h \
    LH/Lib6_Input.h \
    LH/Lib6_Variant.h \
    LH/Lib6_Object.h \
    LH/Lib6_Service.h \
    LH/Lib6_Plugin.h \
    LH/Lib6_Linkable.h \
    LH/Lib6_Source.h \
    LH/Lib6_Sink.h \
    LH/Lib6_Setup.h \
    LH/Lib6_LayoutClass.h \
    LH/Lib6_LayoutItem.h \
    LH/Type_Data.h \
    LH/Type_Meta.h

SOURCES = \
    LH/Event_Base.cpp \
    LH/Lib6_Loader.cpp \
    LH/Lib6_Input.cpp \
    LH/Lib6_Variant.cpp \
    LH/Lib6_Object.cpp \
    LH/Lib6_Service.cpp \
    LH/Lib6_Plugin.cpp \
    LH/Lib6_Linkable.cpp \
    LH/Lib6_Source.cpp \
    LH/Lib6_Sink.cpp \
    LH/Lib6_Setup.cpp \
    LH/Lib6_LayoutClass.cpp \
    LH/Lib6_LayoutItem.cpp \
    LH/Type_Data.cpp

