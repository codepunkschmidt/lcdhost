# lh_api6.pro

LH_API6_HEADERS += \
    $$PWD/lh_id.h \
    $$PWD/lh_input.h \
    $$PWD/lh_qvariant.h \
    $$PWD/lh_event.h \
    $$PWD/lh_object.h \
    $$PWD/lh_plugin.h \
    $$PWD/lh_linkable.h \
    $$PWD/lh_source.h \
    $$PWD/lh_sink.h \
    $$PWD/lh_setup.h \
    $$PWD/lh_widget.h

LH_API6_SOURCES += \
    $$PWD/lh_qvariant.cpp \
    $$PWD/lh_event.cpp \
    $$PWD/lh_object.cpp \
    $$PWD/lh_plugin.cpp \
    $$PWD/lh_linkable.cpp \
    $$PWD/lh_source.cpp \
    $$PWD/lh_sink.cpp \
    $$PWD/lh_setup.cpp \
    $$PWD/lh_widget.cpp

PRECOMPILED_HEADER = $$PWD/lh_api6.h

lh_api6 {
    LIBS += -L$$PWD/.. -llh_api6
} else {
    TARGET = lh_api6
    TEMPLATE = lib
    CONFIG += staticlib
    INCLUDEPATH += $$PWD/..
    DESTDIR = $$PWD/..
    SOURCES += $$LH_API6_SOURCES
    HEADERS += $$LH_API6_HEADERS
}




