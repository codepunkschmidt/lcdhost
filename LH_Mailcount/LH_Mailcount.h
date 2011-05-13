/**
  \file     LH_Mailcount.h
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

#ifndef LH_MAILCOUNT_H
#define LH_MAILCOUNT_H

#include <QDebug>
#include <QTime>
#include <QImage>

#include "../LH_Text/LH_Text.h"

#include "../LH_QtPlugin.h"
#include "../LH_QtInstance.h"
#include "../LH_Qt_QString.h"
#include "../LH_Qt_int.h"
#include "../LH_Qt_QFileInfo.h"
#include "../LH_Qt_bool.h"

class LH_QtPlugin_Mailcount : public LH_QtPlugin
{
public:
    const char * lh_name() { return "Mailcount"; }
    const char * lh_shortdesc() { return "Unread mail indicator."; }
    const char * lh_author() { return "Johan \"SirReal\" Lindh"; }
    const char * lh_homepage() { return "<a href=\"http://www.linkdata.se/software/lcdhost\">Link Data Stockholm</a>"; }
    const char * lh_longdesc() { return "Show unread e-mails. Requires an e-mail client with shell support, like Mozilla Thunderbird or Microsoft Outlook."; }
    const lh_blob *lh_logo();
    const char *lh_load();
    void lh_unload();
};

class LH_Mailcount : public LH_QtInstance
{
    Q_OBJECT

    LH_Qt_QString *email_addr_;
    LH_Qt_int *email_days_;
    LH_Qt_QFileInfo *mail_image_;
    LH_Qt_int *check_interval_;
    LH_Qt_bool *flashing_;
    LH_Qt_bool *smoothflash_;

    int count_;
    int envelope_count_;
    QImage envelope_;
    QTime last_check_;
    bool flash_on_;

public:
    LH_Mailcount();

    void input(lh_setup_item *, int, int);

    int polling();
    int notify(int n,void*p);
    void prerender();
    int width(int h=-1);
    int height(int w=-1);
    QImage *render_qimage( int w, int h );
    lh_blob *render_blob(int,int) { return NULL; }

    int getUnreadMailcount();

    static lh_class *classInfo();

public slots:
    void makeEnvelope();
};

class LH_MailcountText: public LH_Text
{
    Q_OBJECT
    LH_Qt_QString *email_addr_;
    LH_Qt_int *email_days_;
    LH_Qt_int *check_interval_;
    LH_Qt_bool *hide_zero_;

    int count_;
    int envelope_count_;
    QImage envelope_;
    QTime last_check_;

public:
    LH_MailcountText();

    void input(lh_setup_item *, int, int);

    int notify(int n,void*p);

    int getUnreadMailcount();

    static lh_class *classInfo();

public slots:
    void update();
};

#endif // LH_MAILCOUNT_H
