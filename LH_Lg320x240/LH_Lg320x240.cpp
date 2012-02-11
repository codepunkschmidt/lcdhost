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
#include <QCoreApplication>

#include <stdarg.h>

#ifdef Q_WS_WIN
#include <windows.h>
#endif

#include "LH_QtDevice.h"
#include "LH_Lg320x240.h"
#include "Lg320x240Device.h"
#include "LH_Qt_QImage.h"

LH_PLUGIN(LH_Lg320x240)

char __lcdhostplugin_xml[] =
"<?xml version=\"1.0\"?>"
"<lcdhostplugin>"
  "<id>Lg320x240</id>"
  "<rev>" STRINGIZE(REVISION) "</rev>"
  "<api>" STRINGIZE(LH_API_MAJOR) "." STRINGIZE(LH_API_MINOR) "</api>"
  "<ver>" "r" STRINGIZE(REVISION) "</ver>"
  "<versionurl>http://www.linkdata.se/lcdhost/version.php?arch=$ARCH</versionurl>"
  "<author>Johan \"SirReal\" Lindh</author>"
  "<homepageurl><a href=\"http://www.linkdata.se/software/lcdhost\">Link Data Stockholm</a></homepageurl>"
  "<logourl></logourl>"
  "<shortdesc>"
  "Logitech 320x240 LCD via USB"
  "</shortdesc>"
  "<longdesc>"
    "USB-level driver for Logitech 320x240 LCD displays, such as the G19.<br/>"
    "On Windows, you'll need a <a href=\"http://en.wikipedia.org/wiki/WinUSB\">WinUSB</a> driver for your device.<br/>"
    "The easiest way to do that is to download <a href=\"http://sourceforge.net/projects/libwdi/files/zadig/zadig_v1.1.1.137.7z/download\">zadig</a>"
    "which can generate a driver for your G19."
  "</longdesc>"
"</lcdhostplugin>";

extern "C"
{
    void libusb_log( const char *fmt, va_list args )
    {
        char buf[1024];
        vsprintf( buf, fmt, args );
        qDebug() << buf;
    }
}

const char *LH_Lg320x240::userInit()
{
    Q_ASSERT( g19thread_ == 0 );

#ifdef Q_WS_WIN
    // make sure neither LCDMon.exe nor LCORE.EXE is running on Windows
    if( FindWindowA( "Logitech LCD Monitor Window", "LCDMon" ) ||
        FindWindowA( "QWidget", "LCore" ) )
        return "Logitech drivers are loaded";
#endif

    g19thread_ = new LogitechG19Thread(this);
    g19thread_->start();
    return NULL;
}

void LH_Lg320x240::userTerm()
{
    if( g19thread_ )
    {
        g19thread_->timeToDie();

        for( int n=0; n<100; ++n )
        {
            if( g19thread_->wait(100) ) break;
            g19thread_->quit();
            QCoreApplication::processEvents();
        }

        if( g19thread_->wait(1000) )
            delete g19thread_;
    }
    g19thread_ = 0;
}
