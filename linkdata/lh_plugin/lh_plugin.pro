LH_PLUGIN_SOURCES += \
    $$PWD/lh_plugin.c \
    $$PWD/lh_qvariant.cpp \
    $$PWD/lh_object.cpp \
    $$PWD/lh_linkable.cpp \
    $$PWD/lh_source.cpp \
    $$PWD/lh_sink.cpp \
    $$PWD/lh_setup.cpp

LH_PLUGIN_HEADERS += \
    $$PWD/lh_plugin.h \
    $$PWD/lh_interfaces.h \
    $$PWD/lh_qvariant.h \
    $$PWD/lh_object.h \
    $$PWD/lh_linkable.h \
    $$PWD/lh_source.h \
    $$PWD/lh_sink.h \
    $$PWD/lh_setup.h


lh_plugin {
    TARGET = lh_plugin
    TEMPLATE = lib
    CONFIG += staticlib
    INCLUDEPATH += $$PWD
    SOURCES += $$LH_PLUGIN_SOURCES
    HEADERS += $$LH_PLUGIN_HEADERS

    unix:!symbian {
        maemo5 {
            target.path = /opt/usr/lib
        } else {
            target.path = /usr/lib
        }
        INSTALLS += target
    }
}


