TEMPLATE = lib
CONFIG += shared lh_shared
TARGET = lh_json
DEFINES += JSON
SOURCES += \
    json.cpp

HEADERS += \
    json.h

LH_SHARED_HEADERS += $$HEADERS

DESTDIR=$$lh_destdir()
