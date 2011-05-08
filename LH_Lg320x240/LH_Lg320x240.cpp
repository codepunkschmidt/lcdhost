/**
  \file     LH_Lg320x240.cpp
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

#include <QFile>
#include <QDebug>

#include <stdarg.h>

#ifdef Q_WS_WIN
#include <windows.h>
#endif

#include "../LH_QtDevice.h"
#include "LH_Lg320x240.h"
#include "Lg320x240Device.h"
#include "../LH_Qt_QImage.h"

LH_Lg320x240 theDriver;

extern "C"
{
    void libusb_log( const char *fmt, va_list args )
    {
        char buf[1024];
        vsprintf( buf, fmt, args );
        qDebug() << buf;
    }
}

int LH_Lg320x240::lh_notify(int note, void*param)
{
    Q_UNUSED(note);
    Q_UNUSED(param);
    return 0;
}

const char *LH_Lg320x240::lh_load()
{
    Q_ASSERT( g19thread_ == 0 );

#ifdef Q_WS_WIN
    // make sure neither LCDMon.exe nor LCORE.EXE is running on Windows
    if( FindWindowA( "Logitech LCD Monitor Window", "LCDMon" ) ) return "LCDMon.exe is running";
    if( FindWindowA( "QWidget", "LCore" ) ) return "LCore.exe is running";
#endif

    g19thread_ = new LogitechG19Thread(this);
    g19thread_->start();
    return NULL;
}

void LH_Lg320x240::lh_unload()
{
    if( g19thread_ )
    {
        g19thread_->timeToDie();
        g19thread_->wait(2000);
        if( g19thread_->isRunning() ) g19thread_->deleteLater();
        else delete g19thread_;
    }
    g19thread_ = 0;
}
