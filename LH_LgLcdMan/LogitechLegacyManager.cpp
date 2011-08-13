/**
  \file     LogitechLegacyManager.cpp
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

#include <QTimerEvent>
#include "LogitechLegacyManager.h"
#include "LogitechLegacyDevice.h"

void LogitechLegacyManager::enumerate()
{
    int index, retv;
    lgLcdDeviceDesc deviceDescription;

    foreach( QObject *o, children() )
    {
        LogitechLegacyDevice *lld = qobject_cast<LogitechLegacyDevice*>(o);
        if( lld ) lld->setToRemove(true);
    }

    for( index=0; index>=0; index++ )
    {
        bool found = false;
        memset( &deviceDescription, 0, sizeof(deviceDescription) );
        retv = lgLcdEnumerate( connectContext_.connection, index, &deviceDescription );
        if( retv == ERROR_NO_MORE_ITEMS ) return;
        if( retv != ERROR_SUCCESS )
        {
            LCD_ERR(retv);
            break;
        }

        foreach( QObject *o, children() )
        {
            LogitechLegacyDevice *lld = qobject_cast<LogitechLegacyDevice*>(o);
            if( lld && lld->index() == index )
            {
                lld->setToRemove(false);
                found = true;
                break;
            }
        }

        if( !found )
        {
            if( deviceDescription.Width == 160 && deviceDescription.Height == 43 )
            {
                new LogitechLegacyDevice( true, index, this );
            }
            if( deviceDescription.Width == 320 && deviceDescription.Height == 240 )
            {
                new LogitechLegacyDevice( false, index, this );
            }
        }

    }

    foreach( QObject *o, children() )
    {
        LogitechLegacyDevice *lld = qobject_cast<LogitechLegacyDevice*>(o);
        if( lld && lld->toRemove() ) delete lld;
    }

    return;
}

LogitechLegacyManager::LogitechLegacyManager(QObject *parent) :
    LogitechManager(parent),
    online_(false)
{
    memset( &connectContext_, 0, sizeof(connectContext_) );
    connectContext_.appFriendlyName = 0;
    connectContext_.isAutostartable = 0;
    connectContext_.isPersistent = 0;
    connectContext_.onConfigure.configCallback = NULL;
    connectContext_.onConfigure.configContext = NULL;
    connectContext_.connection = LGLCD_INVALID_CONNECTION;

    int retv = lgLcdConnect( &connectContext_ );
    /* Don't report file not found errors */
    if( retv != -1 && retv != ERROR_FILE_NOT_FOUND && retv != ERROR_ALREADY_EXISTS && LCD_ERR( retv ) )
    {
        lgLcdSetDeviceFamiliesToUse( connectContext_.connection, LGLCD_DEVICE_FAMILY_ALL
#ifdef Q_WS_WIN
                                     ,0
#endif
                                     );
        enum_timer_.start(1000,this);
    }
}

LogitechLegacyManager::~LogitechLegacyManager()
{
    enum_timer_.stop();
    if( connectContext_.connection != LGLCD_INVALID_CONNECTION )
    {
        int retv = lgLcdDisconnect(connectContext_.connection);
        if( retv != ERROR_PIPE_NOT_CONNECTED ) LCD_ERR( retv );
        connectContext_.connection = LGLCD_INVALID_CONNECTION;
    }
    lgLcdDeInit();
}

void LogitechLegacyManager::timerEvent(QTimerEvent *event)
{
    if( enum_timer_.timerId() == event->timerId() )
    {
        enumerate();
    }
}
