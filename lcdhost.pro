#
# lcdhost.pro
#

TEMPLATE = subdirs
CONFIG += ordered
isEmpty(LH_DIR_SOURCES): SUBDIRS = qmakecache.pro
SUBDIRS += linkdata plugins
OTHER_FILES = lh_features/*.pri
