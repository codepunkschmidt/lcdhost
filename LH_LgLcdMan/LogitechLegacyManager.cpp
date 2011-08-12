
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
