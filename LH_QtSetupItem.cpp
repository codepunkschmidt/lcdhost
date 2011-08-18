/**
  \file     LH_QtSetupItem.cpp
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
#include "LH_QtSetupItem.h"

#define RECAST(obj) reinterpret_cast<LH_QtSetupItem*>(obj)

static void obj_setup_resize( lh_setup_item *obj, size_t needed )
{
    RECAST(obj->obj.ref)->setup_resize(needed);
}

static void obj_setup_change( lh_setup_item *obj )
{
    RECAST(obj->obj.ref)->setup_change();
}

LH_QtSetupItem::LH_QtSetupItem( LH_QtObject *parent, const char *ident, lh_setup_type type, int flags )
    : LH_QtObject( &item_.obj, ident, parent )

{
    if( ident )
    {
        // check for ident warnings
        if( *ident == '^' && !(flags&LH_FLAG_BLANKTITLE) )
        {
            flags |= LH_FLAG_BLANKTITLE;
            qWarning() << parent->metaObject()->className()
                       << parent->objectName()
                       << objectName()
                       << "added LH_FLAG_BLANKTITLE";
        }

        if( *ident == '~' && !(flags&LH_FLAG_HIDETITLE) )
        {
            flags |= LH_FLAG_HIDETITLE;
            qWarning() << parent->metaObject()->className()
                       << parent->objectName()
                       << objectName()
                       << "added LH_FLAG_HIDETITLE";
        }

        // set a default title
        const char *title = ident;
        while( *title == '^' || *title == '~' ) ++ title;
        setTitle( title );
    }
    else
    {
        // auto generated id's for setup items don't make sense
        // if they're stored
        if( !(flags&LH_FLAG_NOSAVE) )
        {
            flags |= LH_FLAG_NOSAVE;
            qWarning() << parent->metaObject()->className()
                       << parent->objectName()
                       << objectName()
                       << "added LH_FLAG_NOSAVE";
        }
    }


    item_.size = sizeof(lh_setup_item);
    item_.help = 0;
    item_.filter = 0;
    item_.order = 0;
    item_.type = type;
    item_.flags = flags;
    memset( &item_.param, 0, sizeof(lh_setup_param) );

    item_.states = 0;
    item_.link = 0;
    memset( &item_.data, 0, sizeof(lh_setup_data) );

    item_.obj_setup_resize = obj_setup_resize;
    item_.obj_setup_change = obj_setup_change;

    if( flags & LH_FLAG_FIRST ) -- item_.order;
    if( flags & LH_FLAG_LAST ) ++ item_.order;

    parent->callback( lh_cb_setup_create, &item_ );
}

LH_QtSetupItem::~LH_QtSetupItem()
{
    callback( lh_cb_destroy );
    memset( &item_, 0, sizeof(lh_setup_item) );
}

int LH_QtSetupItem::notify( int note, void *param )
{
    if( note & LH_NOTE_INPUT )
    {
        lh_input *p = (lh_input*)param;
        emit input( p->flags, p->value );
    }
    if( note & LH_NOTE_WARNING )
    {
        qptrdiff w = (qptrdiff) param;
        if( w == LH_WARNING_DUPLICATE_SOURCE )
            emit duplicateSource();
    }
    return LH_NOTE_WARNING|LH_NOTE_INPUT;
}

void LH_QtSetupItem::setup_resize( size_t needed )
{
    data_array_.resize(needed);
    item_.data.b.p = data_array_.data();
    item_.data.b.n = data_array_.size();
    return;
}

void LH_QtSetupItem::setup_change()
{
    emit changed();
    if( item_.flags & LH_FLAG_AUTORENDER ) parent()->requestRender();
    return;
}

void LH_QtSetupItem::setLink(const char *s, bool issource)
{
    item_.states &= ~LH_STATE_SOURCE;
    if( s && *s )
    {
        link_array_ = QByteArray(s);
        if( link_array_.startsWith('=') )
        {
            qWarning() << parent()->objectName() << objectName() << "link starts with '='";
            link_array_.remove(0,1);
        }
        if( link_array_.startsWith('@') )
        {
            qWarning() << parent()->objectName() << objectName() << "link starts with '@'";
            link_array_.remove(0,1);
            issource = true;
        }
        item_.link = link_array_.data();
        if( issource ) item_.states |= LH_STATE_SOURCE;
    }
    else
    {
        link_array_.clear();
        item_.link = 0;
    }
    refreshMeta();
    return;
}


void LH_QtSetupItem::setLinkFilter( const char *s )
{
    if( s && *s )
    {
        filter_array_ = QByteArray(s);
        item_.filter = filter_array_.data();
    }
    else
    {
        filter_array_.clear();
        item_.filter = 0;
    }
    refreshMeta();
    return;
}

void LH_QtSetupItem::setHelp( const char *s )
{
    if( s && help_array_ != s )
    {
        help_array_ = s;
        item_.help = help_array_.constData();
        refreshMeta();
        emit helpChanged( help() );
    }
}

void LH_QtSetupItem::setHelp( QString s )
{
    setHelp( s.toUtf8().constData() );
}

void LH_QtSetupItem::setFlags( int f )
{
    if( flags() != f )
    {
        item_.flags = f;
        refreshMeta();
        emit flagsChanged( flags() );
    }
}

void LH_QtSetupItem::setFlag( int f, bool state )
{
    int newflags = flags() & ~f;
    if( state ) newflags |= f;
    setFlags( newflags );
    return;
}

void LH_QtSetupItem::setOrder( int neworder )
{
    if( order() != neworder )
    {
        item_.order = neworder;
        refreshMeta();
        emit orderChanged( order() );
    }
}

void LH_QtSetupItem::setValue( bool b )
{
    if( ! (item_.type & lh_type_integer) )
    {
        Q_ASSERT(!"incorrect underlying data type");
        return;
    }
    if( (bool) item_.data.i != b )
    {
        item_.data.i = (int) b;
        refreshData();
        emit set();
    }
}

void LH_QtSetupItem::setValue( int n )
{
    if( ! (item_.type & lh_type_integer) )
    {
        Q_ASSERT(!"incorrect underlying data type");
        return;
    }
    if( item_.data.i != n )
    {
        item_.data.i = n;
        refreshData();
        emit set();
    }
}

void LH_QtSetupItem::setValue( const QColor& c )
{
    if( item_.type != lh_type_integer_color )
    {
        Q_ASSERT(!"incorrect underlying data type");
        return;
    }
    QRgb rgba = c.rgba();
    if( item_.data.i != rgba )
    {
        item_.data.i = rgba;
        refreshData();
        emit set();
    }
}

void LH_QtSetupItem::setValue( qint64 ll )
{
    if( ! (item_.type & lh_type_integer) )
    {
        Q_ASSERT(!"incorrect underlying data type");
        return;
    }
    if( item_.data.i != ll )
    {
        item_.data.i = ll;
        refreshData();
        emit set();
    }
}

void LH_QtSetupItem::setValue( double d )
{
    if( ! (item_.type & lh_type_double) )
    {
        Q_ASSERT(!"incorrect underlying data type");
        return;
    }
    if( !qFuzzyCompare( item_.data.d, d ) )
    {
        item_.data.d = d;
        refreshData();
        emit set();
    }
}

void LH_QtSetupItem::setValue( void* p )
{
    if( ! (item_.type & lh_type_pointer) )
    {
        Q_ASSERT(!"incorrect underlying data type");
        return;
    }
    if( item_.param.p != p )
    {
        item_.param.p = p;
        refreshData();
        emit set();
    }
}


void LH_QtSetupItem::setValue( const QString& s )
{
    if( ! (item_.type & lh_type_string) )
    {
        Q_ASSERT(!"incorrect underlying data type");
        return;
    }
    if( str_ != s )
    {
        setString( s );
        refreshData();
        emit set();
    }
}

void LH_QtSetupItem::setValue( const QByteArray& a )
{
    if( ! (item_.type & lh_type_array) )
    {
        Q_ASSERT(!"incorrect underlying data type");
        return;
    }
    if( data_array_ != a )
    {
        data_array_ = a;
        item_.data.b.p = data_array_.data();
        item_.data.b.n = data_array_.size();
        refreshData();
        emit set();
    }
}

void LH_QtSetupItem::setValue( const char *s, int len )
{
    if( len < 0 )
    {
        if( ! (item_.type & lh_type_string) )
        {
            Q_ASSERT(!"incorrect underlying data type");
            return;
        }
        len = strlen(s) + 1; // include terminating nul char
    }
    else
    {
        if( ! (item_.type & lh_type_array) )
        {
            Q_ASSERT(!"incorrect underlying data type");
            return;
        }
    }
    if( len > data_array_.size() ) data_array_.resize( len );
    memcpy( data_array_.data(), s, len );
    item_.data.b.p = data_array_.data();
    item_.data.b.n = data_array_.size();
    if( item_.type & lh_type_string ) getString();
    refreshData();
    emit set();
    return;
}

void LH_QtSetupItem::setList( const QByteArray& list )
{
    if( type() == lh_type_string_list ||
            type() == lh_type_string_listbox ||
            type() == lh_type_string_combobox )
    {
        list_array_ = list;
        item_.param.list = list_array_.constData();
        return;
    }
    Q_ASSERT(!"LH_QtSetupItem::setList() called for non-list type");
    return;
}

void LH_QtSetupItem::refreshList()
{
    if( type() == lh_type_string_list ||
            type() == lh_type_string_listbox ||
            type() == lh_type_string_combobox )
    {
        item_.param.list = list_array_.constData();
        refreshMeta();
        return;
    }
    Q_ASSERT(!"LH_QtSetupItem::refreshList() called for non-list type");
    return;
}

void LH_QtSetupItem::setMin( qint64 min )
{
    if( item_.param.i.min != min || !(item_.flags&LH_FLAG_MIN) )
    {
        item_.flags |= LH_FLAG_MIN;
        item_.param.i.min = min;
        refreshMeta();
    }
}

void LH_QtSetupItem::setMax( qint64 max )
{
    if( item_.param.i.max != max || !(item_.flags&LH_FLAG_MAX) )
    {
        item_.flags |= LH_FLAG_MAX;
        item_.param.i.max = max;
        refreshMeta();
    }
}

void LH_QtSetupItem::setMinMax( qint64 min, qint64 max )
{
    if( item_.param.i.min != min || item_.param.i.max != max ||
            (item_.flags&LH_FLAG_MINMAX) != LH_FLAG_MINMAX )
    {
        item_.flags |= LH_FLAG_MINMAX;
        item_.param.i.min = min;
        item_.param.i.max = max;
        refreshMeta();
    }
}

void LH_QtSetupItem::setMin( double min )
{
    if( item_.param.d.min != min || !(item_.flags&LH_FLAG_MIN) )
    {
        item_.flags |= LH_FLAG_MIN;
        item_.param.d.min = min;
        refreshMeta();
    }
}

void LH_QtSetupItem::setMax( double max )
{
    if( item_.param.d.max != max || !(item_.flags&LH_FLAG_MAX) )
    {
        item_.flags |= LH_FLAG_MAX;
        item_.param.d.max = max;
        refreshMeta();
    }
}

void LH_QtSetupItem::setMinMax( double min, double max )
{
    if( item_.param.d.min != min || item_.param.d.max != max ||
            (item_.flags&LH_FLAG_MINMAX) != LH_FLAG_MINMAX )
    {
        item_.flags |= LH_FLAG_MINMAX;
        item_.param.d.min = min;
        item_.param.d.max = max;
        refreshMeta();
    }
}

