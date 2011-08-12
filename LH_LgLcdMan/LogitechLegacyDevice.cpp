#include "LogitechLegacyDevice.h"

LogitechLegacyDevice::LogitechLegacyDevice(bool bw, int index, LogitechLegacyManager *parent ) :
    LogitechDevice( bw, parent ),
    toremove_(false)
{
    cxt_.index = index;
    cxt_.connection = LGLCD_INVALID_CONNECTION;
    cxt_.device = LGLCD_INVALID_DEVICE;
    cxt_.onSoftbuttonsChanged.softbuttonsChangedCallback = LH_LogitechButtonCB;
    cxt_.onSoftbuttonsChanged.softbuttonsChangedContext = this;
}

const char* LogitechLegacyDevice::open()
{
    if( cxt_.device == LGLCD_INVALID_DEVICE )
    {
        cxt_.connection = parent()->connection();
        if( LCD_ERR( lgLcdOpen(&cxt_) ) )
            lgLcdSetAsLCDForegroundApp(cxt_.device,LGLCD_LCD_FOREGROUND_APP_YES);
    }
    return 0;
}

const char* LogitechLegacyDevice::close()
{
    if( cxt_.device != LGLCD_INVALID_DEVICE )
    {
        lgLcdClose(cxt_.device);
        cxt_.device = LGLCD_INVALID_DEVICE;
    }
    return 0;
}
