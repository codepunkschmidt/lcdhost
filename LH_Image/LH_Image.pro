TARGET = LH_Image
TEMPLATE = lib
DEFINES += LH_IMAGE_LIBRARY
QT -= core gui

include(../linkdata/linkdata.pri)

SOURCES += LH_Image.c
HEADERS += LH_Image.h
