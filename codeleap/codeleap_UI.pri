#
# codeleap_UI.pri
#
# Andy "Triscopic" Bridges is the sole maintainer of this
# file and all the files in this directory and subdirectories. 
# Do *NOT* commit here unless you have his explicit approval to do so.
#
# If you find a bug or need a feature or change, submit an issue
# at "http://code.google.com/p/lcdhost/issues/list" and label it
# "codeleap-ui".
#

LH_CODELEAP_UI_DIR = $$PWD/codeleap_UI

INCLUDEPATH += $$LH_CODELEAP_UI_DIR

HEADERS += \
    $$LH_CODELEAP_UI_DIR/LH_Qt_array_int_ui.h \
    $$LH_CODELEAP_UI_DIR/LH_Qt_array_double_ui.h \
    $$LH_CODELEAP_UI_DIR/LH_Qt_array_string_ui.h

SOURCES += \
    $$LH_CODELEAP_UI_DIR/LH_Qt_array_int_ui.cpp \
    $$LH_CODELEAP_UI_DIR/LH_Qt_array_double_ui.cpp \
    $$LH_CODELEAP_UI_DIR/LH_Qt_array_string_ui.cpp

