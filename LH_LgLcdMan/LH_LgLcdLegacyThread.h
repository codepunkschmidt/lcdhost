/**
  \file     LH_LgLcdLegacyThread.h
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

#ifndef LH_LGLCDLEGACYTHREAD_H
#define LH_LGLCDLEGACYTHREAD_H

#include "LH_LgLcdThread.h"
#include <QMutex>
#include <QSemaphore>
#include <QImage>

class LH_LgLcdLegacyThread : public LH_LgLcdThread
{
    Q_OBJECT
    QMutex mtx_;

    bool enumerate( int conn );
    bool render( int conn );

public:
    static lgLcdOpenContext bw_cxt;
    static lgLcdOpenContext qvga_cxt;

    explicit LH_LgLcdLegacyThread(QObject *parent = 0) : LH_LgLcdThread(parent) {}

    void run();

    bool hasBW() const { return bw_cxt.index != -1; }
    bool hasQVGA() const { return qvga_cxt.index != -1; }

#ifdef Q_WS_WIN
    static DWORD WINAPI LH_LogitechButtonCB(int device, DWORD dwButtons, const PVOID pContext);
#endif
#ifdef Q_WS_MAC
    static unsigned long LH_LogitechButtonCB(int device, unsigned long dwButtons, const void* pContext);
#endif
};

#endif // LH_LGLCDLEGACYTHREAD_H
