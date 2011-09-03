/**
  \file     LogitechDevice.cpp
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

#include <QCoreApplication>
#include "LogitechDevice.h"
#include "EventLgLcdButton.h"

#ifdef Q_WS_WIN
DWORD WINAPI LogitechDevice::LH_LogitechButtonCB(int device, DWORD dwButtons, const PVOID pContext)
#endif
#ifdef Q_WS_MAC
unsigned long LogitechDevice::LH_LogitechButtonCB(int device, unsigned long dwButtons, const void* pContext)
#endif
{
    LogitechDevice *ld = reinterpret_cast<LogitechDevice*>( (void*) pContext );
    Q_ASSERT( device != LGLCD_INVALID_DEVICE );
    if( ld && ld->indev_ )
        ld->indev_->setButtonState(dwButtons);
    return 1;
}

LogitechDevice::LogitechDevice( bool bw, LogitechManager *parent ) :
    LH_QtOutputDevice( bw ? "LH_LgLcdMan:BW" : "LH_LgLcdMan:QVGA",
          bw ? 160 : 320,
          bw ? 43 : 240,
          bw ? 1 : 32,
          false, parent ),
    bw_(bw),
    indev_(0)
{
    setTitle( bw_ ? "Logitech B/W LCD" : "Logitech QVGA LCD" );
    indev_ = new LogitechInputDevice(
                bw_ ? "LH_LgLcdMan:BW:Keys":
                      "LH_LgLcdMan:QVGA:Keys",
                lh_input_keyboard );
    indev_->setTitle( title() );
}

LogitechDevice::~LogitechDevice()
{
    if( indev_ )
    {
        delete indev_;
        indev_ = 0;
    }
}

const char* LogitechDevice::render_qimage(QImage *p_image)
{
    if( p_image == NULL || p_image->isNull() )
        return 0;

    if( bw_ )
    {
        if( p_image->width() != 160 || p_image->height() != 43 )
            return "invalid image size";

        bm_.bmp_mono.hdr.Format = LGLCD_BMP_FORMAT_160x43x1;
        for( int y=0; y<43; ++y )
        {
            for( int x=0; x<160; ++x )
            {
                Q_ASSERT( (size_t)(y*160+x) < sizeof(bm_.bmp_mono.pixels) );
                bm_.bmp_mono.pixels[y*160 + x] = ( qGray( p_image->pixel(x,y) ) > 128) ? 0xFF : 0x00;
            }
        }
    }
    else
    {
        Q_ASSERT( p_image->numBytes() == sizeof( bm_.bmp_qvga32.pixels ) );
        bm_.bmp_qvga32.hdr.Format = LGLCD_BMP_FORMAT_QVGAx32;
        memcpy( bm_.bmp_qvga32.pixels,
    #ifdef Q_WS_MAC
                p_image->rgbSwapped().bits(),
    #else
                p_image->bits(),
    #endif
                sizeof( bm_.bmp_qvga32.pixels ) );
    }

    if( device() != LGLCD_INVALID_DEVICE && bm_.hdr.Format )
    {
        if( lgLcdUpdateBitmap( device(), &bm_.hdr, LGLCD_PRIORITY_NORMAL ) != ERROR_SUCCESS )
        {
            close();
        }
    }

    return 0;
}
