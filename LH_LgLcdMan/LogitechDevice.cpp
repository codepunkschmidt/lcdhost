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

LogitechDevice::LogitechDevice( LH_LgLcdMan *drv, bool bw ) : LH_QtDevice(drv)
{
    drv_ = drv;
    opened_ = false;
    bw_ = bw;
    buttonState_ = 0;

    if( bw_ )
    {
        setDevid( "BW" );
        setName( QObject::tr("Logitech B/W device") );
        setSize( 160, 43 );
        setDepth( 1 );
    }
    else
    {
        setDevid( "QVGA" );
        setName( QObject::tr("Logitech QVGA device") );
        setSize( 320, 240 );
        setDepth( 32 );
    }

    arrive();
    return;
}

LogitechDevice::~LogitechDevice()
{
    leave();
    if( opened() ) close();
}

void LogitechDevice::setButtonState( unsigned long button )
{
    if( buttonState_ != button )
    {
        for( int bit=0; bit<8; ++bit )
        {
            int mask = 1<<bit;
            if( (button&mask) != (buttonState_&mask) )
            {
                lh_device_input di;
                di.devid = lh_dev()->devid;
                switch( mask )
                {
                case 0x0001: di.control = "Softbutton 0"; break;
                case 0x0002: di.control = "Softbutton 1"; break;
                case 0x0004: di.control = "Softbutton 2"; break;
                case 0x0008: di.control = "Softbutton 3"; break;
                case 0x0100: di.control = "Left"; break;
                case 0x0200: di.control = "Right"; break;
                case 0x0400: di.control = "Ok"; break;
                case 0x0800: di.control = "Cancel"; break;
                case 0x1000: di.control = "Up"; break;
                case 0x2000: di.control = "Down"; break;
                case 0x4000: di.control = "Menu"; break;
                default: di.control = "Unknown"; break;
                }
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


