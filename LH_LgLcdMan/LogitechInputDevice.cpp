
#include <QtDebug>
#include "LogitechInputDevice.h"
#include "EventLgLcdButton.h"

LogitechInputDevice::LogitechInputDevice(const char *ident, int flags, QObject *parent) :
    LH_QtInputDevice(ident,flags,parent)
{
    memset( &input_, 0, sizeof(lh_input) );
    strncpy( input_.ident, ident, sizeof(input_.ident)-1 );
}

void LogitechInputDevice::setButtonState( unsigned long button )
{
    if( buttonState_ != button )
    {
        for( unsigned long bit=0; bit<32; ++bit )
        {
            unsigned long mask = 1<<bit;
            if( (button&mask) != (buttonState_&mask) )
            {
                input_.item = mask;
                input_.flags = flags() | lh_df_button;
                if( button & mask )
                {
                    input_.flags |= lh_df_down;
                    input_.value = 0xFFFF;
                }
                else
                {
                    input_.flags |= lh_df_up;
                    input_.value = 0x0;
                }
                callback( lh_cb_input, (void*) &input_ );
            }
        }
        buttonState_ = button;
    }
    return;
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

void LogitechInputDevice::customEvent(QEvent *event)
{
    if( event->type() == EventLgLcdButton::type() )
    {
        EventLgLcdButton *be = static_cast<EventLgLcdButton*>(event);
        setButtonState( be->buttons );
        return;
    }
    LH_QtInputDevice::customEvent(event);
}
