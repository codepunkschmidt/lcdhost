TARGET = Plugins
TEMPLATE = subdirs

# SirReals
SUBDIRS = LH_Bar LH_Decor LH_Image LH_Lua LH_Mailcount LH_Text LH_VirtualLCD LH_WebKit
win32:SUBDIRS += LH_LgLcdMan
macx:SUBDIRS += LH_LgLcdMan

# Triscopics
SUBDIRS += LH_Cursor LH_DataViewer LH_Graph LH_Dial LH_Monitoring LH_Weather
win32:SUBDIRS += LH_NowPlaying
