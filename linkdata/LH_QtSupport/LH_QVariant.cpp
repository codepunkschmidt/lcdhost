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
#include "LH_QVariant.h"

void LH_QVariant::fromString( const QString& s )
{
    if( type() == QVariant::Color )
    {
        QString colortext;
        int alpha = 255;
        int index = s.indexOf('@');
        if( ++index > 0 )
        {
            int len = s.indexOf('%',index);
            if( len >= 0 )
            {
                len = len - index;
                alpha = s.mid(index,len).trimmed().toInt() * 256 / 100;
            }
            else
            {
                alpha = s.mid(index,len).trimmed().toInt();
            }
            colortext = s.left(index-1).trimmed();
        }
        else
            colortext = s.trimmed();

        if( colortext == QLatin1String("transparent") )
        {
            setValue( QColor(Qt::transparent) );
            return;
        }

        QColor c(colortext);
        c.setAlpha(alpha);
        setValue(c);
        return;
    }

    if( type() == (QVariant::Type) qMetaTypeId<lh_input>() )
    {
        lh_input in;
        memset( &in, 0, sizeof(in) );
        QStringList sl = s.split('/');
        if( sl.size() > 0 )
        {
            QByteArray ba = sl.first().toAscii();
            ba.truncate( sizeof(in.ident)-1 );
            memcpy( in.ident, ba.constData(), ba.size() );
        }
        if( sl.size() > 1 )
        {
            in.item = sl.at(1).toInt();
        }
        if( sl.size() > 2 )
        {
            in.flags = (lh_input_flag) sl.at(2).toUInt(0,16);
        }
        setValue( in );
        return;
    }

    QVariant::Type t = type();
    setValue(s);
    convert(t);
    return;
}

QString LH_QVariant::toString() const
{
    if( type() == QVariant::Color )
    {
        QString retv;
        const QColor& c = qVariantValue<QColor>(*this);

        if( !c.isValid() ) return QLatin1String("invalid");
        if( c == QColor(Qt::transparent) ) return QLatin1String("transparent");

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
            retv.append(" @ ");
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

    if( type() == (QVariant::Type) qMetaTypeId<lh_input>() )
    {
        QString retv;
        const lh_input &in = qVariantValue<lh_input>(*this);
        if( *in.ident )
        {
            QTextStream ts(&retv,QIODevice::WriteOnly);
            ts << in.ident << '/' << forcesign << in.item;
            ts << '/' << noforcesign << hex << in.flags;
        }
        return retv;
    }

    return QVariant::toString();
}

bool LH_QVariant::read( const lh_variant& lhv )
{
    switch( lhv.fmt )
    {
    case lh_format_boolean: setValue( (bool) lhv.data.i ); break;
    case lh_format_color: setValue( QColor::fromRgba(lhv.data.i) ); break;
    case lh_format_integer: setValue( lhv.data.i ); break;
    case lh_format_double: setValue( lhv.data.d ); break;
    case lh_format_pointer: setValue( lhv.data.p ); break;
    case lh_format_qimage: setValue( *(QImage*)lhv.data.p ); break;
    case lh_format_buffer: setValue( QByteArray::fromRawData(lhv.data.b.p,lhv.data.b.n) ); break;
    case lh_format_string: setValue( QString::fromUtf8(lhv.data.b.p,lhv.data.b.n) ); break;
    case lh_format_input: setValue( (lh_input&)lhv.data.b.p ); break;
    case lh_format_png: setValue( QImage::fromData(QByteArray::fromRawData(lhv.data.b.p,lhv.data.b.n)) ); break;

    case lh_format_font:
        {
            QFont f;
            f.fromString( QLatin1String(lhv.data.b.p) );
            setValue(f);
        }
        break;
    case lh_format_array:
        {
            QVariantList vl;
            const lh_variant *v = (const lh_variant *) lhv.data.b.p;
            if( v )
            {
                int count = lhv.data.b.n/sizeof(lh_variant);
                while( count-- )
                {
                    vl.append( LH_QVariant(*v) );
                    ++ v;
                }
            }
            setValue(vl);
        }
        break;

    case lh_format_none:
        clear();
        break;

    default:
        Q_ASSERT(0);
        return false;
    }

    Q_ASSERT( type() == variantType(lhv.fmt) );
    return true;
}

void LH_QVariant::write( lh_variant& lhv ) const
{
    Q_UNUSED(lhv);
    // Q_ASSERT(0);
}

QVariant::Type LH_QVariant::variantType( lh_format fmt )
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
