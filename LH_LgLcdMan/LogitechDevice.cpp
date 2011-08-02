/**
  \file     LogitechDevice.cpp
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009-2011, Johan Lindh

  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of Link Data Stockholm nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
  */

#include <QDebug>
#include <QCoreApplication>
#include <QEvent>
#include <QString>
#include <QFile>

#include "EventLgLcdNotification.h"
#include "LogitechDevice.h"

LogitechDevice::LogitechDevice( LH_LgLcdMan *drv, bool bw )
    : LH_QtDevice( bw ? "LH_LgLcdMan:BW" : "LH_LgLcdMan:QVGA", drv)
{
    drv_ = drv;
    opened_ = false;
    bw_ = bw;
    buttonState_ = 0;

    if( bw_ )
    {
        setObjectName( QObject::tr("Logitech B/W LCD") );
        setSize( 160, 43 );
        setDepth( 1 );
    }
    else
    {
        setObjectName( QObject::tr("Logitech QVGA LCD") );
        setSize( 320, 240 );
        setDepth( 32 );
    }
    return;
}

LogitechDevice::~LogitechDevice()
{
    if( opened() ) close();
}

const char *LogitechDevice::input_name(const char *devid, int n)
{
    switch(n)
    {
    case 0:
        if( bw_ ) return "Logitech B/W LCD";
        return "Logitech QVGA LCD";
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
    return LH_QtObject::input_name(devid,n);
}

void LogitechDevice::setButtonState( unsigned long button )
{
    if( buttonState_ != button )
    {
        for( unsigned long bit=0; bit<32; ++bit )
        {
            unsigned long mask = 1<<bit;
            if( (button&mask) != (buttonState_&mask) )
            {
                lh_input di;
                strncpy( di.devid, lh_dev()->devid, sizeof(di.devid) );
                di.devid[sizeof(di.devid)-1] = 0;
                di.item = bit;
                di.flags = lh_df_button;
                if( button & mask )
                {
                    di.flags |= lh_df_down;
                    di.value = 0xFFFF;
                }
                else
                {
                    di.flags |= lh_df_up;
                    di.value = 0x0;
                }
                callback( lh_cb_input, (void*) &di );
            }
        }
        buttonState_ = button;
    }
    return;
}

const char* LogitechDevice::render_qimage(QImage *p_image)
{
    if( p_image == NULL ) return NULL;
    if( bw_ ) drv()->setBW( *p_image );
    else drv()->setQVGA( *p_image );
    return NULL;
}

const char *LogitechDevice::close()
{
    if( bw_ ) drv()->setBW( QImage() );
    else drv()->setQVGA( QImage() );
    opened_  = false;
    return NULL;
}

const char* LogitechDevice::get_backlight(lh_device_backlight*)
{
    return "get_backlight not implemented";
}

const char* LogitechDevice::set_backlight(lh_device_backlight*)
{
    return "set_backlight not implemented";
}


