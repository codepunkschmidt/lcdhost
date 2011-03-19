/**
  \file     LH_LgLcdMan.h
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

/*
  The Logitech drivers make the (fairly reasonable) assumption that
  an application will only connect to one device at a time. So their
  drivers try to 'simplify' by only exposing one B/W or QVGA device
  even if more than one is available.

  This class will provide access to a B/W or QVGA device as exposed
  by the Logitech drivers. If you have multiple B/W Logitech devices,
  you need to use another method than Logitech drivers to access them
  individually.
  */

#ifndef LH_LGLCDMAN_H
#define LH_LGLCDMAN_H

#include <QImage>
#include "../LH_QtPlugin.h"
#include "LH_LgLcdThread.h"


class LogitechDevice;

class LH_LgLcdMan : public LH_QtPlugin
{
    Q_OBJECT
    LH_LgLcdThread *thread_;
    LogitechDevice *bw_;
    LogitechDevice *qvga_;

public:
    LH_LgLcdMan() : LH_QtPlugin(), thread_(NULL), bw_(NULL), qvga_(NULL) {}

    virtual const char *lh_name() { return "LgLcdMan"; }
    virtual const char *lh_shortdesc() { return "Logitech G-series keyboard driver using the Logitech software"; }
    virtual const char *lh_author() { return "Johan \"SirReal\" Lindh"; }
    virtual const char *lh_homepage() { return "<a href=\"http://www.linkdata.se/software/lcdhost\">Link Data Stockholm</a>"; }
    virtual const char *lh_longdesc() { return "This driver interfaces with the Logitech LCD Manager, which is part of the Logitech G-series keyboard driver software."; }
    virtual const lh_blob *lh_logo();
    virtual const char *lh_load();
    virtual void lh_unload();
    virtual int lh_notify(int code,void *param);
    const lh_buildinfo * lh_get_buildinfo()
    {
        static lh_buildinfo buildinfo =
        {
            LH_BUILDINFO_SIG,
            sizeof(lh_buildinfo),
            REVISION,
            "http://www.linkdata.se/lcdhost/version.php",
            "r" STRINGIZE(REVISION)
        };
        return &buildinfo;
    }
    bool event( QEvent * e );

    void setBW( QImage img ) { if( thread_ ) thread_->setBW(img); }
    void setQVGA( QImage img ) { if( thread_ ) thread_->setQVGA(img); }

    static const char *lglcd_Err( int result, const char *filename = NULL, unsigned line = 0 );
};

#endif // LH_LGLCDMAN_H
