#include "time.h"

#include <QApplication>
#include <QThread>
#include "WebKitServerWindow.h"
#include "EventWebKitHeartbeat.h"
#include "../WebKitCommand.h"

const char *get_log_filename()
{
    static char log_filename[512];

    if( !*log_filename )
    {
        QString installPath;
        QString name;
        int len;

        // Create log dir
#ifdef Q_WS_MAC
        installPath = ".";
#else
        installPath = QCoreApplication::applicationDirPath();
#endif
        QDir dir( installPath+"/logs/" );
        dir.mkpath( dir.absolutePath() );
        // Clean up old logs
        foreach( QFileInfo info, dir.entryInfoList( QDir::Files | QDir::Writable ) )
        {
            name = info.fileName();
            if( name.startsWith("WebKitServer-") && name.endsWith(".log") )
            {
                if( info.lastModified().daysTo( QDateTime::currentDateTime() ) > 10 )
                    dir.remove( name );
            }
        }

        // Make our filename
        name =
            installPath+"/logs/WebKitServer-" +
            QDateTime::currentDateTime().toString("yyyyMMdd-HHmmss-") +
            QString::number( (quintptr) QThread::currentThreadId() ) + ".log";
        QByteArray ary = name.toAscii();
        memset( log_filename, 0, sizeof(log_filename) );
        if( ary.size() > (int) (sizeof(log_filename)-1) ) len = sizeof(log_filename)-1;
        else len = ary.size();
        memcpy( log_filename, ary.constData(), len );
    }

    return log_filename;
}

static QtMsgHandler oldMessageHandler = 0;
void newMessageHandler( QtMsgType type, const char *msg )
{
    FILE *f;
    struct tm *lt;
    time_t t;
    const char *log_filename;

    log_filename = get_log_filename();

    if( msg == NULL ) return;

#ifndef QT_NO_DEBUG
    if( oldMessageHandler )
        oldMessageHandler( type, msg );
    else
    {
        qInstallMsgHandler( 0 );
        switch( type )
        {
        case QtDebugMsg: qDebug() << msg; break;
        case QtWarningMsg: qWarning() << msg; break;
        case QtCriticalMsg: qCritical() << msg; break;
        case QtFatalMsg: qFatal( "%s", msg ); break;
        }
        qInstallMsgHandler( newMessageHandler );
    }
#endif

    if( *log_filename )
    {
        time( &t );
        lt = localtime( &t );
        if( lt )
        {
            f = fopen( log_filename, "a" );
            if( f )
            {
                fprintf( f, "%04d-%02d-%02d %02d:%02d:%02d | ",
                         lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
                         lt->tm_hour, lt->tm_min, lt->tm_sec );

                switch( type )
                {
                case QtDebugMsg:    fprintf( f, "D.. | %s\n", msg ); break;
                case QtWarningMsg:  fprintf( f, ".W. | %s\n", msg ); break;
                case QtCriticalMsg: fprintf( f, "..E | %s\n", msg ); break;
                case QtFatalMsg:    fprintf( f, "*** | %s\n", msg ); break;
                }

                fclose( f );
            }
        }

        if( type == QtFatalMsg )
        {
            QApplication::quit();
        }
    }

    return;
}
int main(int argc, char *argv[])
{
    WebKitServerWindow *w;
    QApplication app(argc,argv);
    QStringList args = app.arguments();

    QCoreApplication::setOrganizationName("Link Data");
    QCoreApplication::setOrganizationDomain("linkdata.se");
    QCoreApplication::setApplicationName("WebKitServer");

    oldMessageHandler = qInstallMsgHandler( newMessageHandler );

    // If existing instance is running, show that rather than starting
    QLocalSocket *sock = new QLocalSocket();
    sock->connectToServer("LCDHost_WebKitServer");
    if( sock->waitForConnected(100) )
    {
        WebKitCommand( 'S' ).write(sock);
        return 0;
    }
    delete sock;

    w = new WebKitServerWindow();
    if( !args.contains("--hidden") ) w->show();
    app.exec();
    delete w;

    return 0;
}
