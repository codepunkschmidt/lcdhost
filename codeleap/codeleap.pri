TRISCOPIC = LH_Cursor LH_Graph LH_Dial LH_Weather LH_TS3
win32: TRISCOPIC += LH_Monitoring LH_DataViewer LH_NowPlaying LH_DriveStats LH_LCoreReboot

include($$PWD/ConditionalFormatting/ConditionalFormatting.pri)
include($$PWD/json/json.pri)
