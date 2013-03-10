#
# plugins.pro
#

TEMPLATE = subdirs
SUBDIRS = $$files(LH_*)
SUBDIRS -= LH_Lua LH_DriveStats
!win32: SUBDIRS -= LH_LgLcdMan LH_Monitoring LH_NowPlaying LH_LCoreReboot
