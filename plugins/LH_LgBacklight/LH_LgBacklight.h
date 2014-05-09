/**
  \file     LH_Lg160x43.h
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

#ifndef LH_LGBACKLIGHT_H
#define LH_LGBACKLIGHT_H

#include "LH_QtPlugin.h"
#include "LH_Qt_QString.h"
#include "LH_Qt_QStringList.h"
#include "LH_Qt_QColor.h"
#include "LH_Qt_int.h"

class LH_HidDevice;
class LgBacklightDevice;
class QTimerEvent;

class LH_LgBacklight : public LH_QtPlugin
{
    Q_OBJECT

    LH_Qt_QStringList *devselect_;
    LH_Qt_QColor *devcolor_;
    LH_Qt_QColor *allcolor_;
    LH_Qt_int *poll_interval_;

    QList<LgBacklightDevice *> dev_list_;

public:
    LH_LgBacklight() :
        LH_QtPlugin(),
        devselect_(0),
        devcolor_(0),
        allcolor_(0),
        poll_interval_(0)
    {}
    const char *userInit();
    void userTerm();

    void timerEvent(QTimerEvent*);

public slots:
    void onlineChanged(LH_HidDevice *hd, bool b);
    void colorChanged();
    void refreshList();
    void changeDev();
    void changeColor();
    void setAllColor();
    void pollIntervalChanged();
};

#endif // LH_LGBACKLIGHT_H
