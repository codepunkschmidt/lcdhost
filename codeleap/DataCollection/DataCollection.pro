TEMPLATE = lib
CONFIG += shared lh_shared
TARGET = lh_datacollection
DEFINES += DATACOLLECTION
INCLUDEPATH += $$PWD
HEADERS += \
    DataCollection.h
LH_SHARED_HEADERS += $$HEADERS
DESTDIR=$$lh_destdir()
