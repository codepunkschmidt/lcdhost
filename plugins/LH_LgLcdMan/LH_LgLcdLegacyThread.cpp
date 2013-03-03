/**
  \file     LH_LgLcdLegacyThread.cpp
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
  Logitech software is quite unstable and buggy, so it's advisable to
  isolate it as much as possible. This legacy (circa Logitech version 3)
  driver thread is needed because the Logitech drivers on OS/X is even
  worse than on Windows.
  */

#include <QDebug>
#include <QCoreApplication>
#include <QTime>

#include "LH_LgLcdLegacyThread.h"
#include "EventLgLcdButton.h"
#include "LH_LgLcdMan.h"

lgLcdOpenContext LH_LgLcdLegacyThread::bw_cxt;
lgLcdOpenContext LH_LgLcdLegacyThread::qvga_cxt;

#define LCD_ERR(x) ( LH_LgLcdMan::lglcd_Err((x),__FILE__,__LINE__) == NULL )

#ifdef Q_OS_WIN
DWORD WINAPI LH_LgLcdLegacyThread::LH_LogitechButtonCB(int device, DWORD dwButtons, const PVOID pContext)
#endif
#ifdef Q_OS_MAC
unsigned long LH_LgLcdLegacyThread::LH_LogitechButtonCB(int device, unsigned long dwButtons, const void* pContext)
#endif
{
    LH_LgLcdLegacyThread *thread = reinterpret_cast<LH_LgLcdLegacyThread *>( (void*) pContext );
    Q_ASSERT( device != LGLCD_INVALID_DEVICE );
    Q_ASSERT( (device == bw_cxt.device) || (device == qvga_cxt.device) );
    if( thread && thread->parent() )
        QCoreApplication::postEvent(
                thread->parent(),
                new EventLgLcdButton((bw_cxt.device == device), dwButtons),
                Qt::HighEventPriority
                );
    return 1;
}

bool LH_LgLcdLegacyThread::enumerate( int conn )
{
    int index, retv;
    lgLcdDeviceDesc deviceDescription;

    /*
      Get indices for the first device of each type.
      We limit ourselves to the first, since post 3.00 versions
      of the Logitech drivers has deprecated the enumeration
      and really only support accessing one compound device
      which is then multiplexed in the LCD Manager.
      However, using the lgOpenByType() function doesn't work
      on OS/X, so we are still forced to use these old ones.
      */

    int bw = -1;
    int qvga = -1;

    for( index=0; index>=0; index++ )
    {
        memset( &deviceDescription, 0, sizeof(deviceDescription) );
        retv = lgLcdEnumerate( conn, index, &deviceDescription );
        if( retv == ERROR_NO_MORE_ITEMS )
        {
            if( bw_cxt.device == LGLCD_INVALID_DEVICE ) bw_cxt.index = bw;
            if( qvga_cxt.device == LGLCD_INVALID_DEVICE ) qvga_cxt.index = qvga;
            return true;
        }
        if( retv != ERROR_SUCCESS ) break;
        if( bw == -1 && deviceDescription.Width == 160 && deviceDescription.Height == 43 )
            bw = index;
        if( qvga == -1 && deviceDescription.Width == 320 && deviceDescription.Height == 240 )
            qvga = index;
    }

    if( bw_cxt.device == LGLCD_INVALID_DEVICE ) bw_cxt.index = bw;
    if( qvga_cxt.device == LGLCD_INVALID_DEVICE ) qvga_cxt.index = qvga;
    LCD_ERR(retv);
    return false;
}

bool LH_LgLcdLegacyThread::render( int conn )
{
    // Close open contexts no longer in use
    if( !bw_bm.hdr.Format && bw_cxt.device != LGLCD_INVALID_DEVICE )
    {
        lgLcdClose(bw_cxt.device);
        bw_cxt.device = LGLCD_INVALID_DEVICE;
    }
    if( !qvga_bm.hdr.Format && qvga_cxt.device != LGLCD_INVALID_DEVICE )
    {
        lgLcdClose(qvga_cxt.device);
        qvga_cxt.device = LGLCD_INVALID_DEVICE;
    }

    // Open context that are available and needed
    if( bw_bm.hdr.Format && bw_cxt.device == LGLCD_INVALID_DEVICE )
    {
        bw_cxt.connection = conn;
        if( LCD_ERR( lgLcdOpen(&bw_cxt) ) )
            lgLcdSetAsLCDForegroundApp(bw_cxt.device,LGLCD_LCD_FOREGROUND_APP_YES);
    }
    if( qvga_bm.hdr.Format && qvga_cxt.device == LGLCD_INVALID_DEVICE )
    {
        qvga_cxt.connection = conn;
        if( LCD_ERR( lgLcdOpen(&qvga_cxt) ) )
            lgLcdSetAsLCDForegroundApp(qvga_cxt.device,LGLCD_LCD_FOREGROUND_APP_YES);
    }

    // Push bits to open devices that has images
    if( bw_cxt.device != LGLCD_INVALID_DEVICE && bw_bm.hdr.Format )
    {
        if( lgLcdUpdateBitmap( bw_cxt.device, &bw_bm.hdr, LGLCD_PRIORITY_NORMAL ) != ERROR_SUCCESS )
        {
            lgLcdClose(bw_cxt.device);
            bw_cxt.device = LGLCD_INVALID_DEVICE;
        }
    }

    if( qvga_cxt.device != LGLCD_INVALID_DEVICE && qvga_bm.hdr.Format )
    {
        if( lgLcdUpdateBitmap( qvga_cxt.device, &qvga_bm.hdr, LGLCD_PRIORITY_NORMAL ) != ERROR_SUCCESS )
        {
            lgLcdClose(qvga_cxt.device);
            qvga_cxt.device = LGLCD_INVALID_DEVICE;
        }
    }

    return true;
}

void LH_LgLcdLegacyThread::run()
{
    int index, retv;
    lgLcdConnectContext connectContext;

    bw_cxt.index = -1;
    bw_cxt.connection = LGLCD_INVALID_CONNECTION;
    bw_cxt.device = LGLCD_INVALID_DEVICE;
    bw_cxt.onSoftbuttonsChanged.softbuttonsChangedCallback = LH_LogitechButtonCB;
    bw_cxt.onSoftbuttonsChanged.softbuttonsChangedContext = this;

    qvga_cxt.index = -1;
    qvga_cxt.connection = LGLCD_INVALID_CONNECTION;
    qvga_cxt.device = LGLCD_INVALID_DEVICE;
    qvga_cxt.onSoftbuttonsChanged.softbuttonsChangedCallback = LH_LogitechButtonCB;
    qvga_cxt.onSoftbuttonsChanged.softbuttonsChangedContext = this;

    while( stayAlive() )
    {
        /* Ignore errors here to avoid spamming errors when G15 broken on not present */
        if( lgLcdInit() == ERROR_SUCCESS )
        {
            memset( &connectContext, 0, sizeof(connectContext) );
            connectContext.appFriendlyName = appname_;
            connectContext.isAutostartable = 0;
            connectContext.isPersistent = 0;
            connectContext.onConfigure.configCallback = NULL;
            connectContext.onConfigure.configContext = NULL;
            connectContext.connection = LGLCD_INVALID_CONNECTION;

            retv = lgLcdConnect( &connectContext );
            /* Don't report file not found errors */
            if( retv != -1 && retv != ERROR_FILE_NOT_FOUND && retv != ERROR_ALREADY_EXISTS && LCD_ERR( retv ) )
            {
                QTime last_enum;
                last_enum.start();
                lgLcdSetDeviceFamiliesToUse( connectContext.connection, LGLCD_DEVICE_FAMILY_ALL
#ifdef Q_OS_WIN
                                             ,0
#endif
                                             );
                while( stayAlive() )
                {
                    sem_.tryAcquire(1,100);
                    if( !stayAlive() ) break;
                    if( last_enum.elapsed() >= 1000 )
                    {
                        if( !enumerate( connectContext.connection ) ) break;
                        last_enum.start();
                    }
                    if( !render( connectContext.connection ) ) break;
                }
                if( bw_cxt.device != LGLCD_INVALID_DEVICE )
                {
                    lgLcdClose( bw_cxt.device );
                    bw_cxt.device = LGLCD_INVALID_DEVICE;
                }
                if( qvga_cxt.device != LGLCD_INVALID_DEVICE )
                {
                    lgLcdClose( qvga_cxt.device );
                    qvga_cxt.device = LGLCD_INVALID_DEVICE;
                }
                retv = lgLcdDisconnect(connectContext.connection);
                if( retv != ERROR_PIPE_NOT_CONNECTED ) LCD_ERR( retv );
                connectContext.connection = LGLCD_INVALID_CONNECTION;
            }
            lgLcdDeInit();
        }
        for( index=0; stayAlive() && index<20; index ++ ) msleep(100);
    }

    return;
}
