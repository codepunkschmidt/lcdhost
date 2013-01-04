#include <time.h>
#include <stdarg.h>

#include <QApplication>
#include <QThread>
#include <QSettings>

#include "LH_Logger.h"
#include "WebKitServerWindow.h"
#include "EventWebKitHeartbeat.h"
#include "WebKitCommand.h"

#if 0

QString datadir;
const char *get_log_filename()
{
    static char log_filename[512];

    if( !*log_filename )
    {
        QSettings settings;
        QString logdir;
        QString name;
        int len;

        logdir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
        if( !logdir.endsWith('/') ) logdir.append('/');
        logdir.append("logs/");

        QDir dir( logdir );
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
            dir.absolutePath()+"/WebKitServer-" +
            QDateTime::currentDateTime().toString("yyyyMMdd-HHmmss-") +
            QString::number( (quintptr) QThread::currentThreadId() ) + ".log";
        QByteArray ary = name.toLatin1();
        memset( log_filename, 0, sizeof(log_filename) );
        if( ary.size() > (int) (sizeof(log_filename)-1) ) len = sizeof(log_filename)-1;
        else len = ary.size();
        memcpy( log_filename, ary.constData(), len );
    }

    return log_filename;
}

static QtMessageHandler oldMessageHandler = 0;
void newMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    FILE *f;
    struct tm *lt;
    time_t t;
    const char *log_filename;

    log_filename = get_log_filename();

    if( msg.isEmpty() ) return;
    if( msg.startsWith("QGLShader::link") ) return;

#ifndef QT_NO_DEBUG
    if( oldMessageHandler )
        oldMessageHandler( type, context, msg );
    else
    {
        qInstallMessageHandler( 0 );
        switch( type )
        {
        case QtDebugMsg: qDebug() << msg; break;
        case QtWarningMsg: qWarning() << msg; break;
        case QtCriticalMsg: qCritical() << msg; break;
        case QtFatalMsg: qCritical() << msg; break;
        }
        qInstallMessageHandler( newMessageHandler );
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
                case QtDebugMsg:    fprintf( f, "D.. | %s\n", msg.toLatin1().constData() ); break;
                case QtWarningMsg:  fprintf( f, ".W. | %s\n", msg.toLatin1().constData() ); break;
                case QtCriticalMsg: fprintf( f, "..E | %s\n", msg.toLatin1().constData() ); break;
                case QtFatalMsg:    fprintf( f, "*** | %s\n", msg.toLatin1().constData() ); break;
                }

                fclose( f );
#ifndef QT_NO_DEBUG
                fflush( f );
#endif
            }
        }

        if( type == QtFatalMsg )
            qApp->quit();
    }

    return;
}

#if 0
// Work around http://bugreports.qt.nokia.com/browse/QTCREATORBUG-5082
void qFatal(const char *msg, ...)
{
    va_list ap;
    va_start(ap, msg);
    if (msg)
    {
        QByteArray buf;
        buf = QString().vsprintf(msg, ap).toLocal8Bit();
        newMessageHandler( QtFatalMsg, buf.constData() );
    }
    va_end(ap);
}
#endif
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    QString plugindir;
    bool hidden = false;
    bool verbose = false;
    WebKitServerWindow *w = 0;

    QCoreApplication::setOrganizationName("Link Data");
    QCoreApplication::setOrganizationDomain("linkdata.se");
    QCoreApplication::setApplicationName("WebKitServer");
    LH_Logger logger("WebKitServer");

    // parse command line
    QStringList args = QCoreApplication::arguments();
    while( args.size()>1 && args[1].startsWith('-') )
    {
        QString theArg = args[1];
        args.removeAt(1);

        if( theArg=="--hidden" )
        {
            hidden = true;
        }

        if( theArg=="--verbose" )
        {
            verbose = true;
        }

#if 0
        if( theArg=="--datadir" )
        {
            datadir = args[1];
            args.removeAt(1);
        }
#endif

        if( theArg=="--plugindir" )
        {
            plugindir = args[1];
            args.removeAt(1);
        }
    }

    if( !plugindir.isEmpty() )
        QCoreApplication::setLibraryPaths( QStringList(plugindir) );

    // If existing instance is running, show that rather than starting
    QLocalSocket *sock = new QLocalSocket();
    sock->connectToServer("LCDHost_WebKitServer");
    if( sock->waitForConnected(100) )
    {
        if( verbose ) qDebug() << "Asking running instance to show itself";
        WebKitCommand( 'S' ).write(sock);
        return 0;
    }
    delete sock;

    w = new WebKitServerWindow();
    if( verbose )
    {
        qDebug() << "Starting WebKitServer";
        qDebug() << "Library paths:" << QCoreApplication::libraryPaths();
    }
    if( !hidden ) w->show();
    app.exec();
    delete w;

    return 0;
}
