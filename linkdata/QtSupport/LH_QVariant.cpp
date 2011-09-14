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

bool operator>( const QXmlStreamReader& reader, const QVariant& v )
{
    return false;
}

QXmlStreamReader& operator>>( QXmlStreamReader& reader, QVariant& v )
{
    return reader;
}

QXmlStreamWriter& operator<<( QXmlStreamWriter& writer, const QVariant& v )
{
    return writer;
}

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
            QString::fromRawData(start,(it-start)) >> member;
            ++ it;
            start = it;
            retv.append(member);
            continue;
        }
        if( *it == mark_list_start )
        {
            ++ it;
            QVariant member;
            member = decode_list(it,end,recurse);
            if( recurse ) retv.append( member );
            else retv.append( member.toString() );
        }
    }
    return retv;
}

/**
  Converts LCDHost formatted string data into QVariant,
  preserving the QVariant data type. If the QVariant
  does not have a data type set, it will be set to
  QVariant::String.

  "LCDHost formatting" means the string data is
  whitespace separated. Embedded whitespace
  is escaped.

  Returns the number of characters used from the
  string, or zero in case of error.
  */
int operator>>( const QString& s, QVariant& v )
{
    int used = 0;
    const QVariant::Type orgtype = v.type();

    /*
        Data is encoded so as to not contain whitespace.
        ASCII white space is mapped to ASCII non-whitespace
        escape characters. UNICODE white space and the ASCII
        escape characters are then mapped to UNICODE PUA's
        0xE700 .. 0xE73F according to escape_list[].

        This code grabs the first chunk of non-whitespace
        and decodes it using our escape scheme. It also
        sets the number of 'used' characters so we'll
        claim that chunk and any surrounding whitespace.
      */
    QString::const_iterator it = s.constBegin();
    while( it != s.constEnd() && it->isSpace() ) ++it;
    const QChar *contentstart = it;
    while( it != s.constEnd() && !(it->isSpace()) ) ++it;
    QString content = QString::fromRawData( contentstart, (it-contentstart) );
    while( it != s.constEnd() && it->isSpace() ) ++it;
    used = (it - s.constBegin());

    // modifying 'content' will cause a deep copy to occur automatically
    for( int i=0; i<content.size(); ++i )
    {
        ushort ch = content.at(i).unicode();
        if( ch == L'_' ) content[i] = L' ';
        else if( ch == L'|' ) content[i] = L'\t';
        else if( ch == L'~' ) content[i] = L'\n';
        else if( ch == L'^' ) content[i] = L'\r';
        else if( ch >= mark_first_escape && ch <= mark_last_escape )
            content[i] = escape_list[ ch - mark_first_escape ];
    }

    if( orgtype == QVariant::Invalid || orgtype == QVariant::String )
    {
        v.setValue( content );
        return used;
    }

    // try to convert the string data into wanted type
    it = content.constBegin();
    if( it == content.constEnd() ) return 0; // no content to parse

    if( orgtype == QVariant::Color )
    {
        int alpha = -1;
        const QChar *nameend = it;

        // use plain number as Rgba data
        if( it->isDigit() )
        {
            v.setValue( QColor::fromRgba( content.toInt() ) );
            return used;
        }

        if( *it == QChar('#') )
        {
            ++it;
            while( it != content.constEnd() && isHexDigit( it->toLower() ) ) ++it;
            nameend = it;
        }
        else if( it->isLetter() )
        {
            while( it != content.constEnd() && it->isLetter() ) ++it;
            nameend = it;
        }

        if( it != content.constEnd() && *it == QChar('@') )
        {
            ++ it;
            const QChar *alphastart = it;
            while( it != content.constEnd() && it->isDigit() ) ++it;
            if( it != alphastart )
            {
                alpha = QString::fromRawData(alphastart,(it-alphastart)).toInt();
                if( it != content.constEnd() && *it == QChar('%') )
                {
                    alpha = alpha * 256 / 100;
                }
            }
        }

        QColor c( QString::fromRawData( content.constBegin(), (nameend-content.constBegin()) ) );
        if( alpha >= 0 && alpha <= 255 ) c.setAlpha(alpha);
        v.setValue(c);
    }
    else if( orgtype == QVariant::List || orgtype == QVariant::StringList )
    {
        if( *it == mark_list_start )
        {
            ++it;
            v.setValue( decode_list( it, content.constEnd(), (orgtype == QVariant::List) ) );
        }
        else
        {
            // Not our encoding scheme, let Qt try to convert
            v.setValue(content);
        }
    }
    else if( orgtype == (QVariant::Type) qMetaTypeId<lh_input>() )
    {
        lh_input in;
        const QChar *identstart = it;
        while( it != content.constEnd() && *it != QChar('/') ) ++it;
        const QString ident = QString::fromRawData(identstart,(it-identstart));
        if( ident.size() == 0 || ident.size() >= LH_MAX_IDENT ) return 0;
        memset( &in, 0, sizeof(in) );
        for( int i=0; i<ident.size(); ++i )
            in.ident[i] = ident.at(i).toAscii();

        if( it != content.constEnd() && *it == QChar('/') )
        {
            ++ it;
            if( it != content.constEnd() && (*it == QChar('+') || *it == QChar('-')) )
            {
                const QChar *itemstart = it;
                ++ it;
                while( it != content.constEnd() && it->isDigit() ) ++it;
                in.item = QString::fromRawData(itemstart,it-itemstart).toInt();

                if( it != content.constEnd() && *it == QChar('/') )
                {
                    ++ it;
                    const QChar *flagstart = it;
                    while( it != content.constEnd() && isHexDigit(it->toLower()) ) ++it;
                    in.flags = QString::fromRawData(flagstart,it-flagstart).toInt(0,16);
                }
            }
        }
        v.setValue( in );
    }
    else // try to have Qt convert it
    {
        v.setValue( content );
    }

    if( orgtype != v.type() && !v.convert(orgtype) ) return 0;
    return used;
}

QString& operator>>( QString& s, QVariant& v )
{
    int used = operator>>( (const QString&) s, v );
    s.remove(0,used);
    return s;
}

QString& operator<<( QString& s, const QVariant& v )
{
    QString retv;

    if( v.type() == QVariant::Bool )
    {

    }

    if( v.type() == QVariant::Int || v.type() == QVariant::LongLong )
    {

    }

    if( v.type() == QVariant::Double )
    {

    }

    if( v.type() == QVariant::ByteArray )
    {

    }

    if( v.type() == QVariant::Color )
    {
        const QColor& c = qVariantValue<QColor>(v);

        if( !c.isValid() )
        {
            s.append(QLatin1String("invalid"));
            return s;
        }

        if( c == QColor(Qt::transparent) )
        {
            s.append(QLatin1String("transparent"));
            return s;
        }

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

        s.append(retv);
        return s;
    }

    if( v.type() == (QVariant::Type) qMetaTypeId<lh_input>() )
    {
        const lh_input &in = qVariantValue<lh_input>(v);
        if( *in.ident )
        {
            QTextStream ts(&retv,QIODevice::WriteOnly);
            ts << in.ident << '/' << forcesign << in.item;
            ts << '/' << noforcesign << hex << in.flags;
        }
        s.append(retv);
        return s;
    }

    retv = v.toString();
    s.append(retv);
    return s;
}

bool operator>>( const QVariant& v, lh_variant& lhv )
{
    Q_UNUSED(v);
    Q_UNUSED(lhv);
    // Q_ASSERT(0);
    return false;
}

QVariant::Type lh_varianttype( lh_format fmt )
{
    switch( fmt )
    {
    case lh_format_boolean: return QVariant::Bool;
    case lh_format_color: return QVariant::Color;
    case lh_store_integer:
    case lh_format_integer: return QVariant::LongLong;
    case lh_store_double:
    case lh_format_double: return QVariant::Double;
    case lh_store_pointer:
    case lh_format_pointer: return (QVariant::Type) QMetaType::VoidStar;
    case lh_format_qimage: return QVariant::Image;
    case lh_store_buffer:
    case lh_format_buffer: return QVariant::ByteArray;
    case lh_format_string: return QVariant::String;
    case lh_format_input: return (QVariant::Type) qMetaTypeId<lh_input>();
    case lh_format_png: return QVariant::Image;
    case lh_format_font: return QVariant::Font;
    case lh_format_array: return QVariant::List;

    case lh_format_none:
    case lh_format_unused:
        break;
    }

    return QVariant::Invalid;
}

void qVariantSetValue( QVariant& v, const lh_variant& lhv )
{
    switch( lhv.fmt )
    {
    case lh_format_boolean: v.setValue( (bool) lhv.data.i ); break;
    case lh_format_color: v.setValue( QColor::fromRgba(lhv.data.i) ); break;
    case lh_format_integer: v.setValue( lhv.data.i ); break;
    case lh_format_double: v.setValue( lhv.data.d ); break;
    case lh_format_pointer: v.setValue( lhv.data.p ); break;
    case lh_format_qimage: v.setValue( *(QImage*)lhv.data.p ); break;
    case lh_format_buffer: v.setValue( QByteArray::fromRawData(lhv.data.b.p,lhv.data.b.n) ); break;
    case lh_format_string: v.setValue( QString::fromUtf8(lhv.data.b.p,lhv.data.b.n) ); break;
    case lh_format_input: v.setValue( (lh_input&)lhv.data.b.p ); break;
    case lh_format_png: v.setValue( QImage::fromData(QByteArray::fromRawData(lhv.data.b.p,lhv.data.b.n)) ); break;

    case lh_format_font:
        {
            QFont f;
            f.fromString( QLatin1String(lhv.data.b.p) );
            v.setValue(f);
        }
        break;
    case lh_format_array:
        {
            QVariantList vl;
            const lh_variant *pv = (const lh_variant *) lhv.data.b.p;
            if( pv )
            {
                int count = lhv.data.b.n/sizeof(lh_variant);
                while( count-- )
                {
                    vl.append( *pv );
                    ++ pv;
                }
            }
            v.setValue(vl);
        }
        break;

    case lh_format_none:
        v.clear();
        break;

    default:
        Q_ASSERT(0);
        return;
    }

    Q_ASSERT( v.type() == lh_varianttype(lhv.fmt) );
    return;
}
