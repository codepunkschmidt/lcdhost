#include <time.h>
#include <stdarg.h>

#include <QApplication>
#include <QThread>
#include <QSettings>

#include "LH_Logger.h"
#include "WebKitServerWindow.h"
#include "EventWebKitHeartbeat.h"
#include "WebKitCommand.h"

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
    LH_Logger logger;

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
