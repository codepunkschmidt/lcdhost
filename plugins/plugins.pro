#
# plugins.pro
#

TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS *= LH_Image LH_Text LH_Bar LH_Graph LH_Dial LH_Decor LH_ColorSwatch
SUBDIRS *= $$files(LH_*)
SUBDIRS -= LH_Lua LH_DriveStats
!win32: SUBDIRS -= LH_LgLcdMan LH_Monitoring LH_NowPlaying LH_LCoreReboot
