TEMPLATE = lib
CONFIG += shared lh_shared
TARGET = lh_json
DEFINES += JSON
SOURCES += \
    json.cpp

HEADERS += \
    json.h

DESTDIR=$$lh_destdir()
