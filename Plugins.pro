#
# Plugins.pro
#
# Johan Lindh <johan@linkdata.se> is the sole maintainer of this
# file. Do *NOT* commit to this file unless you have his explicit
# approval to do so.
#
# If you find a bug or need a feature or change, submit an issue
# at "http://code.google.com/p/lcdhost/issues/list" and label it
# "linkdata-plugins".

TARGET = Plugins
TEMPLATE = subdirs
CONFIG += ordered

# To control which plugins should get built,
# write qmake commands to populate BUILD_PLUGINS in
# Custom.pri. This file must not be versioned, and will not
# exist in a newly cloned copy of this repo.
# If you choose to not create it, there's code
# below that simply tries to build all available plugins.

# Custom.pri is also the correct place to set
# the defines that "linkdata/LCDHost.pri" uses
# when signing a plugin, unless you happen to be a regular 
# committer and have the keys automatically checked for.

# Read "linkdata/LCDHost.pri" for more details.

exists(Custom.pri) {
    include(Custom.pri)
} else {
    exists($$PWD/../triscopic-private.pem) {
        message("Triscopic: move these into your Custom.pri and delete this exists() section plz")
        BUILD_PLUGINS = LH_Cursor LH_Graph LH_Dial LH_Weather LH_TorrentMon
        win32: BUILD_PLUGINS += LH_NowPlaying LH_TS3 LH_LCoreReboot
    }
    isEmpty(BUILD_PLUGINS):BUILD_PLUGINS = $$files(LH_*)
}

SUBDIRS = $$BUILD_PLUGINS
