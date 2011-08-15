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

#include "../LH_QtInstance.h"
#include "../LH_Qt_QString.h"
#include "../LH_Qt_int.h"
#include "../LH_Qt_QFileInfo.h"
#include "../LH_Qt_bool.h"

class LH_Mailcount : public LH_QtInstance
{
    Q_OBJECT

    LH_Qt_int *email_count_;
    LH_Qt_QFileInfo *mail_image_;
    LH_Qt_bool *flashing_;
    LH_Qt_bool *smoothflash_;

    int envelope_count_;
    QImage envelope_;
    bool flash_on_;

public:
    LH_Mailcount() : LH_QtInstance(), email_count_(0), mail_image_(0),
        flashing_(0), smoothflash_(0), envelope_count_(0), flash_on_(false) {}
    const char *userInit();

    int count() const { return email_count_->value(); }
    int polling();
    int notify(int n,void*p);
    void prerender();
    int width(int h=-1);
    int height(int w=-1);
    QImage *render_qimage( int w, int h );
    lh_blob *render_blob(int,int) { return NULL; }

    static lh_class *classInfo();

public slots:
    void makeEnvelope();
};

#endif // LH_MAILCOUNT_H
