#
# plugins.pro
#

TARGET = plugins
TEMPLATE = subdirs
CONFIG += ordered

load($$PWD/../lcdhost.prf)

isEmpty(LH_PLUGINS_LIST): LH_PLUGINS_LIST=$$files(LH_*)
SUBDIRS += $$LH_PLUGINS_LIST

