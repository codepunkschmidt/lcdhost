# lh_plugin.pro

lh_api6 {
    LH_API6_SOURCES += \
        $$PWD/lh_global.c \
        $$PWD/lh_qvariant.cpp \
        $$PWD/lh_object.cpp \
        $$PWD/lh_linkable.cpp \
        $$PWD/lh_source.cpp \
        $$PWD/lh_sink.cpp \
        $$PWD/lh_setup.cpp

    LH_API6_HEADERS += \
        $$PWD/lh_global.h \
        $$PWD/lh_id.h \
        $$PWD/lh_interfaces.h \
        $$PWD/lh_qvariant.h \
        $$PWD/lh_object.h \
        $$PWD/lh_linkable.h \
        $$PWD/lh_source.h \
        $$PWD/lh_sink.h \
        $$PWD/lh_setup.h

    INCLUDEPATH += $$PWD
    SOURCES += $$LH_API6_SOURCES
    HEADERS += $$LH_API6_HEADERS
}

not_lh_plugin {
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

