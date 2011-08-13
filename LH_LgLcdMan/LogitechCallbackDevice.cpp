/**
  \file     LogitechCallbackDevice.cpp
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
#include <QString>

#include "LogitechCallbackDevice.h"
#include "EventLgLcdButton.h"

LogitechCallbackDevice::LogitechCallbackDevice( bool bw, LogitechCallbackManager *parent ) :
    LogitechDevice( bw, parent )
{
    cxt_.connection = LGLCD_INVALID_CONNECTION;
    cxt_.deviceType = bw ? LGLCD_DEVICE_BW : LGLCD_DEVICE_QVGA;
    cxt_.onSoftbuttonsChanged.softbuttonsChangedCallback = LH_LogitechButtonCB;
    cxt_.onSoftbuttonsChanged.softbuttonsChangedContext = this;
    cxt_.device = LGLCD_INVALID_DEVICE;
    return;
}

LogitechCallbackDevice::~LogitechCallbackDevice()
{
    close();
}

const char* LogitechCallbackDevice::open()
{
    if( cxt_.device == LGLCD_INVALID_DEVICE )
    {
        Q_ASSERT( cxt_.onSoftbuttonsChanged.softbuttonsChangedContext == this );
        cxt_.connection = parent()->connection();
        if( LCD_ERR( lgLcdOpenByType(&cxt_) ) )
            lgLcdSetAsLCDForegroundApp(cxt_.device,LGLCD_LCD_FOREGROUND_APP_YES);
    }
    return 0;
}

const char *LogitechCallbackDevice::close()
{
    if( cxt_.device != LGLCD_INVALID_DEVICE )
    {
        lgLcdClose( cxt_.device );
        cxt_.device = LGLCD_INVALID_DEVICE;
    }
    return 0;
}

