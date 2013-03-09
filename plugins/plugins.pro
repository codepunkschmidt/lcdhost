#
# plugins.pro
#

TEMPLATE = subdirs
SUBDIRS = $$files(LH_*)
SUBDIRS -= LH_Lua
!win32: SUBDIRS -= LH_DriveStats LH_LgLcdMan LH_Monitoring LH_NowPlaying LH_LCoreReboot
