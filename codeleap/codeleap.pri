TRISCOPIC = LH_Cursor LH_Graph LH_Dial LH_Weather LH_TS3 LH_Logic LH_DataViewer
win32: TRISCOPIC += LH_Monitoring LH_NowPlaying LH_DriveStats LH_LCoreReboot

include($$PWD/ConditionalFormatting/ConditionalFormatting.pri)
include($$PWD/SimpleTranslator/SimpleTranslator.pri)
include($$PWD/json/json.pri)
