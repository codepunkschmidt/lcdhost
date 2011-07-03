TARGET = LH_Mailcount
TEMPLATE = lib
DEFINES += LH_MAILCOUNT_LIBRARY

include(../Plugins.pri)

SOURCES += \
    ../lh_plugin.c \
    ../LH_QtPlugin.cpp \
    ../LH_QtInstance.cpp \
    ../LH_QtObject.cpp \
    ../LH_QtSetupItem.cpp \
    LH_Mailcount.cpp \
    ../LH_Text/LH_Text.cpp \
    LH_QtPlugin_Mailcount.cpp \
    LH_MailcountText.cpp

HEADERS += \
    ../lh_plugin.h \
    ../LH_QtPlugin.h \
    ../LH_QtInstance.h \
    ../LH_QtObject.h \
    ../LH_QtSetupItem.h \
    LH_Mailcount.h \
    ../LH_Text/LH_Text.h \
    LH_QtPlugin_Mailcount.h \
    LH_MailcountText.h

RESOURCES += LH_Mailcount.qrc
