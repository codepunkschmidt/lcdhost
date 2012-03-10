#
# plugins.pro
#
# To control which plugins get built you must create the file
# "../lcdhost.prf" and define LH_PLUGINS_LIST in that. If
# you leave LH_PLUGINS_LIST empty, it will build them all.
#

TARGET = plugins
TEMPLATE = subdirs
CONFIG += ordered

load($$PWD/../lcdhost.prf)
isEmpty(LH_PLUGINS_LIST): LH_PLUGINS_LIST=$$files(LH_*)

SUBDIRS += $$LH_PLUGINS_LIST
