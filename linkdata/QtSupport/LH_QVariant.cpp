/**
  \file     LH_QVariant.cpp
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

#include <QDebug>
#include <QColor>
#include <QImage>
#include <QFont>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "LH_QVariant.h"

static bool isHexDigit( const QChar ch )
{
    return ch.isDigit() || ( ch >= QChar('a') && ch <= QChar('f') );
}

static const QChar escape_list[] =
{
    L'_', /* L' ' */
    L'|', /* L'\t' */
    L'~', /* L'\n' */
    L'^', /* L'\r' */
    0x000B, /* vertical tab */
    0x000C, /* form feed */
    QChar::LineSeparator,
    QChar::ParagraphSeparator,
    QChar::Nbsp,
    0x1680,
    0x180E,
    0x2000,
    0x2001,
    0x2002,
    0x2003,
    0x2004,
    0x2005,
    0x2006,
    0x2007,
    0x2008,
    0x2009,
    0x200A,
    0x202F,
    0x205F,
    0x3000
};

enum
{
    mark_list_start = 0xE700,
    mark_list_delim,
    mark_list_end,
    mark_first_escape,
    mark_last_escape = mark_first_escape + (sizeof(escape_list) / sizeof(QChar)) - 1
};

static QVariant decode_list( QString::const_iterator& it, QString::const_iterator end, bool recurse )
{
    QVariantList retv;
    const QChar *start = it;
    while( it != end )
    {
        if( *it == mark_list_end )
        {
            ++ it;
            return retv;
        }
        if( *it == mark_list_delim )
        {
            QVariant member;
            lh_qstring_to_qvariant( QString::fromRawData(start,(it-start)), member );
            retv.append( member );
            ++ it;
            start = it;
            continue;
        }
        if( *it == mark_list_start )
        {
            ++ it;
            QVariant member( decode_list(it,end,recurse) );
            if( recurse ) retv.append( member );
            else retv.append( member.toString() );
        }
    }
    return retv;
}

QVariant::Type lh_qvarianttype( lh_data_info info )
{
    switch( info & lh_format_mask )
    {
    case lh_format_boolean: return QVariant::Bool;
    case lh_format_color: return QVariant::Color;
    case lh_format_integer: return QVariant::LongLong;
    case lh_format_double: return QVariant::Double;
    case lh_format_string: return QVariant::String;
    case lh_format_stringlist: return QVariant::StringList;
    case lh_format_input: return (QVariant::Type) qMetaTypeId<lh_input>();
    case lh_format_png: return QVariant::Image;
    case lh_format_font: return QVariant::Font;
    case lh_format_none:
    case lh_format_unused:
        break;
    }
    return QVariant::Invalid;
}

void lh_qstring_to_qcolor( const QString& s, QColor& c )
{
    int alpha = -1;
    QString::const_iterator it = s.constBegin();
    while( it != s.constEnd() && it->isSpace() ) ++it;
    const QChar *nameend = it;

    // use plain number as Rgba data
    if( it->isDigit() )
    {
        c = QColor::fromRgba( s.toInt() );
        return;
    }

    if( *it == QChar('#') )
    {
        ++it;
        while( it != s.constEnd() && isHexDigit( it->toLower() ) ) ++it;
        nameend = it;
    }
    else if( it->isLetter() )
    {
        while( it != s.constEnd() && it->isLetter() ) ++it;
        nameend = it;
    }

    if( it != s.constEnd() && *it == QChar('@') )
    {
        ++ it;
        const QChar *alphastart = it;
        while( it != s.constEnd() && it->isDigit() ) ++it;
        if( it != alphastart )
        {
            alpha = QString::fromRawData(alphastart,(it-alphastart)).toInt();
            if( it != s.constEnd() && *it == QChar('%') )
            {
                alpha = alpha * 256 / 100;
            }
        }
    }

    c.setNamedColor( QString::fromRawData( s.constBegin(), (nameend-s.constBegin()) ) );
    if( alpha >= 0 && alpha <= 255 ) c.setAlpha(alpha);
    return;
}

QString lh_qstring_from_qcolor( const QColor& c )
{
    if( !c.isValid() ) return QLatin1String("invalid");
    if( c == QColor(Qt::transparent) ) return QLatin1String("transparent");

    QString retv;
    foreach( const QString& colorName, QColor::colorNames() )
    {
        if( c.rgb() == QColor(colorName).rgb() )
        {
            retv = colorName;
            break;
        }
    }

    if( retv.isEmpty() ) retv = c.name();

    if( c.alpha() != 255  )
    {
        int pct = c.alpha()*100/256;
        retv.append('@');
        if( pct*256/100 == c.alpha()  )
        {
            retv.append( QString::number( pct ) );
            retv.append('%');
        }
        else
        {
            retv.append( QString::number( c.alpha() ) );
        }
    }

    return retv;
}

void lh_qstring_to_lhinput( const QString& s, lh_input& in )
{
    memset( &in, 0, sizeof(in) );

    QString::const_iterator it = s.constBegin();
    while( it != s.constEnd() && it->isSpace() ) ++it;
    const QChar *identstart = it;
    while( it != s.constEnd() && *it != QChar('/') ) ++it;
    const QString ident = QString::fromRawData(identstart,(it-identstart));
    if( ident.size() == 0 || ident.size() >= LH_MAX_IDENT ) return;

    for( int i=0; i<ident.size(); ++i )
        in.ident[i] = ident.at(i).toAscii();

    if( it != s.constEnd() && *it == QChar('/') )
    {
        ++ it;
        if( it != s.constEnd() && (*it == QChar('+') || *it == QChar('-')) )
        {
            const QChar *itemstart = it;
            ++ it;
            while( it != s.constEnd() && it->isDigit() ) ++it;
            in.item = QString::fromRawData(itemstart,it-itemstart).toInt();

            if( it != s.constEnd() && *it == QChar('/') )
            {
                ++ it;
                const QChar *flagstart = it;
                while( it != s.constEnd() && isHexDigit(it->toLower()) ) ++it;
                in.flags = QString::fromRawData(flagstart,it-flagstart).toInt(0,16);
            }
        }
    }

    return;
}

QString lh_qstring_from_lhinput( const lh_input& in )
{
    QString retv;
    if( *in.ident )
    {
        QTextStream ts(&retv,QIODevice::WriteOnly);
        ts << in.ident << '/' << forcesign << in.item;
        ts << '/' << noforcesign << hex << in.flags;
    }
    return retv;
}

QVariant lh_qvariant_from_setup_data( const lh_data_info& info, const void *p )
{
    if( p == 0 || info.store == lh_store_none || info.format == lh_format_none )
        return QVariant();

    if( info.store == lh_store_qvariant )
        return *(const QVariant*)p;

    if( info.store == lh_store_input )
        return qVariantFromValue( *(const lh_input*)p );

    const lh_buffer *buf = 0;
    if( info.store == lh_store_buffer )
    {
        buf = (const lh_buffer *)p;
        if( buf->p == 0 || buf->n < 1 ) buf = 0;
    }

    switch( info.format )
    {
    case lh_format_integer:
        if( info.store == lh_store_longlong ) return qVariantFromValue( *(const long long*)p );
        if( info.store == lh_store_double ) return qVariantFromValue( (long long) qRound(*(const double*)p) );
        if( buf ) return qVariantFromValue( QByteArray::fromRawData(buf->p,buf->n).toLongLong() );
        break;

    case lh_format_boolean:
        if( info.store == lh_store_longlong ) return qVariantFromValue( *(const long long*)p ? true : false );
        if( info.store == lh_store_double ) return qVariantFromValue( qFuzzyIsNull(*(const double*)p) );
        if( buf ) return qVariantFromValue( QVariant(QString::fromUtf8(buf->p,buf->n)).toBool() );
        break;

    case lh_format_color:
        if( info.store == lh_store_longlong ) return qVariantFromValue( QColor::fromRgba(*(const long long*)p) );
        if( info.store == lh_store_double ) return qVariantFromValue( QColor::fromRgba((int) *(const double*)p) );
        if( buf )
        {
            QColor c;
            lh_qstring_to_qcolor( QString::fromUtf8(buf->p,buf->n), c );
            return qVariantFromValue(c);
        }
        break;

    case lh_format_double:
        if( info.store == lh_store_longlong ) return qVariantFromValue( (double) *(const long long*)p );
        if( info.store == lh_store_double ) return qVariantFromValue( *(const double*)p );
        if( buf ) return qVariantFromValue( QByteArray::fromRawData(buf->p,buf->n).toDouble() );
        break;

    case lh_format_string:
        if( info.store == lh_store_longlong ) return qVariantFromValue( QString::number( *(const long long*)p ) );
        if( info.store == lh_store_double ) return qVariantFromValue( QString::number( *(const double*)p ) );
        if( buf ) return qVariantFromValue( QString::fromUtf8(buf->p,buf->n) );
        break;

    case lh_format_stringlist:
        if( buf ) return qVariantFromValue( QString::fromUtf8(buf->p,buf->n).split(QChar(0)) );
        break;

    case lh_format_font:
        if( buf )
        {
            QFont f;
            f.fromString( QString::fromUtf8(buf->p,buf->n) );
            return qVariantFromValue( f );
        }
        break;

    case lh_format_png:
        if( buf ) return qVariantFromValue( QImage::fromData(QByteArray::fromRawData(buf->p,buf->n) ) );
        break;

    case lh_format_input:
        if( buf )
        {
            lh_input in;
            lh_qstring_to_lhinput( QString::fromUtf8(buf->p,buf->n), in );
            return qVariantFromValue(in);
        }
        break;
    }

    qCritical() << "unhandled format or conversion" << lh_data_format_to_name(info.format);
    return QVariant();
}

QString lh_qstring_from_qvariant( const QVariant& v )
{
    if( v.type() == QVariant::Color )
        return lh_qstring_from_qcolor( qVariantValue<QColor>(v) );
    if( v.type() == (QVariant::Type) qMetaTypeId<lh_input>() )
        return lh_qstring_from_lhinput( qVariantValue<lh_input>(v) );
    return v.toString();
}

void lh_qstring_to_qvariant( const QString& s, QVariant& v )
{
    const QVariant::Type orgtype = v.type();

    if( orgtype == QVariant::Color )
    {
        QColor c;
        lh_qstring_to_qcolor( s, c );
        v.setValue(c);
        return;
    }

    if( orgtype == (QVariant::Type) qMetaTypeId<lh_input>() )
    {
        lh_input in;
        lh_qstring_to_lhinput(s,in);
        v.setValue(in);
        return;
    }

    v.setValue( s );
    if( orgtype == QVariant::Invalid || orgtype == QVariant::String ) return;
    v.convert(orgtype);
    return;
}

bool operator>( const QXmlStreamReader& reader, const QVariant& )
{
    return ( reader.isStartElement() && reader.name() == QLatin1String("value") );
}

QXmlStreamReader& operator>>( QXmlStreamReader& reader, QVariant& v )
{
    Q_ASSERT( reader > v ); // always check for acceptance before loading!

    while( !reader.atEnd() )
    {
        reader.readNext();

        if( reader.isCharacters() )
        {
            QStringRef ref(reader.text());
            lh_qstring_to_qvariant( QString::fromRawData(ref.unicode(),ref.size()), v );
            continue;
        }

        if( reader.isEndElement() )
            return reader;
    }

    // Unrecognized XML or premature end-of-stream
    return reader;
}

QXmlStreamWriter& operator<<( QXmlStreamWriter& writer, const QVariant& v )
{
    if( !v.isValid() || v.isNull() ) return writer;
    writer.writeStartElement( QLatin1String("value") );
    writer.writeCharacters( lh_qstring_from_qvariant(v) );
    writer.writeEndElement();
    return writer;
}
