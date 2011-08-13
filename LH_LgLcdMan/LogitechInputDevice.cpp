#include "LogitechInputDevice.h"

LogitechInputDevice::LogitechInputDevice(const char *ident, int flags, QObject *parent) :
    LH_QtInputDevice(ident,flags,parent)
{
}

const char* LogitechInputDevice::input_name(int item)
{
    switch(item)
    {
    case 0x0001: return "Softbutton 0";
    case 0x0002: return "Softbutton 1";
    case 0x0004: return "Softbutton 2";
    case 0x0008: return "Softbutton 3";
    case 0x0100: return "Left";
    case 0x0200: return "Right";
    case 0x0400: return "Ok";
    case 0x0800: return "Cancel";
    case 0x1000: return "Up";
    case 0x2000: return "Down";
    case 0x4000: return "Menu";
    }
    return 0;
}
