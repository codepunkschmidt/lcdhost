TRISCOPIC = LH_Cursor LH_Graph LH_Dial LH_Weather LH_TS3 LH_Logic LH_DataViewer LH_ColorSwatch
win32: TRISCOPIC += LH_Monitoring LH_LCoreReboot
win32-g++: TRISCOPIC += LH_NowPlaying
# win32: TRISCOPIC += LH_DriveStats

include($$PWD/ConditionalFormatting/ConditionalFormatting.pri)
include($$PWD/SimpleTranslator/SimpleTranslator.pri)
include($$PWD/DataCollection/DataCollection.pri)
include($$PWD/json/json.pri)
