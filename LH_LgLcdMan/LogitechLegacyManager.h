/**
  \file     LogitechLegacyManager.h
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

#ifndef LOGITECHLEGACYMANAGER_H
#define LOGITECHLEGACYMANAGER_H

#include <QBasicTimer>
#include "LogitechManager.h"

class LogitechLegacyDevice;

#ifdef Q_WS_WIN
# ifndef UNICODE
#  error ("This isn't going to work")
# endif
# include "windows.h"
# include "../wow64.h"
# include "win/lglcd.h"
#endif

#ifdef Q_WS_MAC
# include "mac/lgLcdError.h"
# include "mac/lgLcd.h"
# ifndef ERROR_FILE_NOT_FOUND
#  define ERROR_FILE_NOT_FOUND 2
# endif
# ifndef ERROR_PIPE_NOT_CONNECTED
#  define ERROR_PIPE_NOT_CONNECTED 2
# endif
# ifndef LGLCD_DEVICE_FAMILY_ALL
#  define LGLCD_DEVICE_FAMILY_ALL 0xFF
# endif
#endif

class LogitechLegacyManager : public LogitechManager
{
    Q_OBJECT
    bool online_;
    lgLcdConnectContext connectContext_;
    QBasicTimer enum_timer_;

public:
    explicit LogitechLegacyManager(QObject *parent = 0);
    ~LogitechLegacyManager();
    int connection() const { return connectContext_.connection; }
    void enumerate();
    void timerEvent(QTimerEvent *);
};

#endif // LOGITECHLEGACYMANAGER_H
