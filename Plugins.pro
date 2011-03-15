TARGET = Plugins
TEMPLATE = subdirs
SUBDIRS = LH_Text LH_Mailcount LH_Image LH_Decor LH_Lua LH_Bar LH_VirtualLCD LH_WebKit LH_Dial LH_Graph
win32:SUBDIRS += LH_LgLcdMan
macx:SUBDIRS += LH_LgLcdMan
