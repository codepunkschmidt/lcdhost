#
# json.pri
#
# Andy "Triscopic" Bridges is the sole maintainer of this
# file and all the files in this directory and subdirectories. 
# Do *NOT* commit here unless you have his explicit approval to do so.
#
# If you find a bug or need a feature or change, submit an issue
# at "http://code.google.com/p/lcdhost/issues/list" and label it
# "codeleap-json".
#

LH_JSON_DIR = $$PWD/json

INCLUDEPATH += $$LH_JSON_DIR
HEADERS += $$LH_JSON_DIR/json.h
SOURCES += $$LH_JSON_DIR/json.cpp
