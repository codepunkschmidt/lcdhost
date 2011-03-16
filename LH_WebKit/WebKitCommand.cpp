
#include <QDebug>
#include <QDataStream>

#include "WebKitCommand.h"

WebKitCommand::WebKitCommand( int c, QSize s, QUrl u, QString h ) : command_(c), size_(s), url_(u), html_(h)
{
    if( !command_ )
    {
        if( !s.isValid() ) command_ = 'Q';
        else if( s.isNull() ) command_ = 'P';
        else if( u.isEmpty() ) command_ = 'R';
        else if( h.isEmpty() ) command_ = 'U';
        else command_ = 'H';
    }
    return;
}

QByteArray WebKitCommand::encode()
{
    QByteArray array;
    QDataStream dat(&array,QIODevice::WriteOnly);

    dat << command_;
    switch( command_ )
    {
    case 'H': dat << html_; // fall through
    case 'U': dat << url_; // fall through
    case 'R': dat << size_; // fall through
    case 'P': // fall through
    case 'S':
    case 's':
    case 'Q': dat << '\n'; // fall through
        break;
    default:
        Q_ASSERT(!"WebKitCommand::encode() internal error");
        break;
    }
    return array;
}

bool WebKitCommand::decode( const QByteArray& array )
{
    QDataStream dat( array );
    int type, lf;
    QSize s;
    QUrl u;
    QString h;

    dat >> type;

    switch(type)
    {
    case 'H': dat >> h; // fall through
    case 'U': dat >> u; // fall through
    case 'R': dat >> s; // fall through
    case 'P': // fall through
    case 'S': // fall through
    case 's': // fall through
    case 'Q': dat >> lf;
        if( lf == '\n' )
        {
            command_ = type;
            size_ = s;
            url_ = u;
            html_ = h;
            encode();
            return true;
        }
        Q_ASSERT( !"WebKitCommand::decode() incorrect encoding" );
        break;
    default:
        Q_ASSERT( !"WebKitCommand::decode() unknown type" );
        break;
    }
    return false;
}

bool WebKitCommand::write( QIODevice *dev )
{
    QByteArray array = encode();
    qint32 bytes = array.size();
    if( dev->write( (const char*)(const void*)&bytes, sizeof(bytes) ) == sizeof(bytes) )
        if( dev->write( array ) == bytes ) return true;
    return false;
}

QByteArray WebKitResult::encode()
{
    QByteArray array;
    QDataStream dat(&array,QIODevice::WriteOnly);
    dat << '=' << progress_ << image_ << '\n';
    return array;
}

bool WebKitResult::decode( const QByteArray& array )
{
    QDataStream dat( array );
    int eq,lf,pr;
    QImage im;
    dat >> eq;
    if( eq == '=' )
    {
        dat >> pr >> im >> lf;
        if( lf == '\n')
        {
            progress_ = pr;
            image_ = im;
            return true;
        }
        Q_ASSERT( !"WebKitResult::decode() incorrect encoding" );
    }
    Q_ASSERT( !"WebKitResult::decode() unknown type" );
    return false;
}
