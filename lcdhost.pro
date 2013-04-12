#
# lcdhost.pro
#

TEMPLATE = subdirs
CONFIG += ordered
isEmpty(LH_DIR_SOURCES) {
    SUBDIRS += qmakecache
    OTHER_FILES += .qmake.cache.in
}
SUBDIRS += linkdata codeleap plugins
OTHER_FILES += lh_features/*.pri
