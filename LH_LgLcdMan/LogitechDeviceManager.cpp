
#include <QCoreApplication>
#include <QtDebug>

#include "LogitechDeviceManager.h"
#include "LogitechOutputDevice.h"
#include "EventLgLcdNotification.h"
#include "EventLgLcdButton.h"

#ifdef Q_WS_WIN
DWORD WINAPI LogitechDeviceManager::LH_LogitechCB( int connection, const PVOID pContext, DWORD code, DWORD p1, DWORD p2, DWORD p3, DWORD p4 )
#endif
#ifdef Q_WS_MAC
unsigned long LogitechDeviceManager::LH_LogitechCB( int connection, const void* pContext, unsigned long code, unsigned long p1, unsigned long p2, unsigned long p3, unsigned long p4 )
#endif
{
    Q_UNUSED(connection);
    LogitechDeviceManager *ldm = reinterpret_cast<LogitechDeviceManager*>( (quintptr) pContext);
    if( ldm ) QCoreApplication::postEvent( ldm, new EventLgLcdNotification(code,p1,p2,p3,p4) );
    return 1;
}

LogitechDeviceManager::LogitechDeviceManager(QObject *parent) :
    LogitechManager(parent),
    online_(false),
    bw_(0),
    qvga_(0)
{
    if( lgLcdInit() == ERROR_SUCCESS )
    {
        memset( &connectContextEx_, 0, sizeof(connectContextEx_) );
        connectContextEx_.appFriendlyName = appname_;
        connectContextEx_.isPersistent = 0;
        connectContextEx_.isAutostartable = 0;
        connectContextEx_.onConfigure.configCallback = NULL;
        connectContextEx_.onConfigure.configContext = NULL;
        connectContextEx_.connection = LGLCD_INVALID_CONNECTION;
        connectContextEx_.dwAppletCapabilitiesSupported = LGLCD_APPLET_CAP_BW | LGLCD_APPLET_CAP_QVGA;
        connectContextEx_.onNotify.notificationCallback = LH_LogitechCB;
        connectContextEx_.onNotify.notifyContext = this;

        int retv = lgLcdConnectEx( &connectContextEx_ );
        /* Don't report file not found errors */
        if( retv != -1 && retv != ERROR_FILE_NOT_FOUND && retv != ERROR_ALREADY_EXISTS && LCD_ERR( retv ) )
        {
            online_ = true;
        }
    }
}


/*

            while( stayAlive() && online_ )
            {
                sem_.tryAcquire( 1, 100 );
                if( !stayAlive() ) break;
                if( !render( connectContextEx.connection ) ) break;
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

  */

LogitechDeviceManager::~LogitechDeviceManager()
{
    if( bw_ ) delete bw_;
    if( qvga_ ) delete qvga_;
    if( online_ )
    {
        int retv = lgLcdDisconnect(connectContextEx_.connection);
        if( retv != ERROR_PIPE_NOT_CONNECTED ) LCD_ERR( retv );
        online_ = false;
    }
    connectContextEx_.connection = LGLCD_INVALID_CONNECTION;
    lgLcdDeInit();
}

void LogitechDeviceManager::customEvent(QEvent *event)
{
    if( event->type() == EventLgLcdNotification::type() )
    {
        EventLgLcdNotification *e = static_cast<EventLgLcdNotification*>(event);
        switch( e->code )
        {
        case LGLCD_NOTIFICATION_DEVICE_ARRIVAL:
            if( e->p1 == LGLCD_DEVICE_BW )
            {
                Q_ASSERT( bw_ == 0 );
                if( !bw_ ) bw_ = new LogitechOutputDevice( true, this );
                return;
            }
            if( e->p1 == LGLCD_DEVICE_QVGA )
            {
                Q_ASSERT( qvga_ == 0 );
                if( !qvga_ ) qvga_ = new LogitechOutputDevice( false, this );
                return;
            }
            break;
        case LGLCD_NOTIFICATION_DEVICE_REMOVAL:
            if( e->p1 == LGLCD_DEVICE_BW )
            {
                Q_ASSERT( bw_ );
                if( bw_ ) delete bw_;
                bw_ = 0;
                return;
            }
            if( e->p1 == LGLCD_DEVICE_QVGA )
            {
                Q_ASSERT( qvga_ );
                if( qvga_ ) delete qvga_;
                return;
            }
            break;
        case LGLCD_NOTIFICATION_APPLET_ENABLED:
            // qDebug() << "LGLCD_NOTIFICATION_APPLET_ENABLED";
            break;
        case LGLCD_NOTIFICATION_APPLET_DISABLED:
            // qDebug() << "LGLCD_NOTIFICATION_APPLET_DISABLED";
            break;
        case LGLCD_NOTIFICATION_CLOSE_CONNECTION:
            // qDebug() << "LGLCD_NOTIFICATION_CLOSE_CONNECTION";
            online_ = false;
            break;
        default:
            qWarning() << "Unknown notification from Logitech software:" << e->code;
            break;
        }
        return;
    }
    LogitechManager::customEvent(event);
}

