/**
  \file     LH_Lg320x240.h
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

#ifndef LH_LG320x240_H
#define LH_LG320x240_H

#include "../LH_QtPlugin.h"
#include "LogitechG19Thread.h"
#include "LogitechG19.h"

class LH_Lg320x240 : public LH_QtPlugin
{
    Q_OBJECT

    LogitechG19Thread *g19thread_;

public:
    LH_Lg320x240() : LH_QtPlugin() { g19thread_ = 0; }

    virtual const char *lh_name() { return "Lg320x240"; }
    virtual const char *lh_shortdesc() { return "Logitech 320x240 LCD"; }
    virtual const char *lh_author() { return "Johan \"SirReal\" Lindh"; }
    virtual const char *lh_homepage() { return "<a href=\"http://www.linkdata.se/software/lcdhost\">Link Data Stockholm</a>"; }
    virtual const char *lh_longdesc()
    {
        return "USB-level driver for Logitech 320x240 LCD displays, such as the G19.<br>"
                "On Windows, you'll need a <a href=\"http://en.wikipedia.org/wiki/WinUSB\">WinUSB</a> driver for your device.<br>"
                "The easiest way to do that is to download <a href=\"http://sourceforge.net/projects/libwdi/files/zadig/zadig_v1.1.1.137.7z/download\">zadig</a> which can generate a driver for your G19."
                ;
    }
    virtual const char *lh_load();
    virtual void lh_unload();
    virtual int lh_notify(int,void*);

    const lh_buildinfo * lh_version( int amaj, int amin )
    {
        static lh_buildinfo buildinfo = LH_STD_BUILDINFO;
        Q_UNUSED( amaj );
        Q_UNUSED( amin );
        Q_ASSERT( amaj == LH_API_MAJOR );
        Q_ASSERT( amin >= LH_API_MINOR );
        return &buildinfo;
    }
};

#endif // LH_LG320x240_H
