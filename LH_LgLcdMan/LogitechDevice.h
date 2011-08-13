/**
  \file     LogitechDevice.h
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

#ifndef LOGITECHDEVICE_H
#define LOGITECHDEVICE_H

#include "../LH_QtOutputDevice.h"
#include "../LH_QtInputDevice.h"
#include "LogitechManager.h"

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
#endif

class LogitechDevice : public LH_QtOutputDevice
{
    Q_OBJECT
    bool bw_; // if true a BW device, else QVGA
    unsigned long buttonState_;
    LH_QtInputDevice *indev_;

protected:
    lgLcdBitmap bm_;

public:
    explicit LogitechDevice( bool bw, LogitechManager *parent );
    ~LogitechDevice();

    LogitechManager *parent() const
    {
        return reinterpret_cast<LogitechManager*>(LH_QtOutputDevice::parent());
    }

    bool bw() const { return bw_; }

    virtual int device() const { return LGLCD_INVALID_DEVICE; }

    const char* render_qimage(QImage *p_image);
    const char* get_backlight(lh_device_backlight*) { return 0; }
    const char* set_backlight(lh_device_backlight*) { return 0; }

    void customEvent(QEvent *);

    int buttons() { return (int) buttonState_; }
    unsigned long buttonState() const { return buttonState_; }
    void setButtonState( unsigned long ul );

#ifdef Q_WS_WIN
    static DWORD WINAPI LH_LogitechButtonCB(int device, DWORD dwButtons, const PVOID pContext);
#endif
#ifdef Q_WS_MAC
    static unsigned long LH_LogitechButtonCB(int device, unsigned long dwButtons, const void* pContext);
#endif
};

#endif // LOGITECHDEVICE_H
