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
#include <limits.h>
#include <float.h>
#include "LH_QtSetupItem.h"

#define RECAST(obj) reinterpret_cast<LH_QtSetupItem*>(obj)

static void obj_value_changed( lh_setup_item *obj )
{
    RECAST(obj->obj.ref)->value_changed();
}

static void obj_param_changed( lh_setup_item *obj )
{
    RECAST(obj->obj.ref)->param_changed();
}

static void obj_link_changed( lh_setup_item *obj )
{
    RECAST(obj->obj.ref)->link_changed();
}

LH_QtSetupItem::LH_QtSetupItem( LH_QtObject *parent, const char *ident, lh_meta_type type, int flags ) :
    LH_QtObject( &item_.obj, ident, parent ),
    value_( item_.value, this ),
    min_( item_.param.min, this ),
    max_( item_.param.max, this ),
    other_( item_.param.other, this )
{
    // handle A18 flags
    Q_ASSERT( flags >= 0 );
    if( flags >= lh_meta_flag_unused )
    {
        int newflags = lh_meta_default;
        if( flags & LH_FLAG_READONLY )      newflags &= ~(lh_meta_enabled);
        if( flags & LH_FLAG_HIDDEN )        newflags &= ~(lh_meta_visible);
        if( flags & LH_FLAG_FOCUS)          newflags |=  (lh_meta_focus);
        if( flags & LH_FLAG_AUTORENDER )    newflags |=  (lh_meta_autorender);
        if( flags & LH_FLAG_FIRST )         newflags |=  (lh_meta_first);
        if( flags & LH_FLAG_LAST )          newflags |=  (lh_meta_last);
        if( flags & LH_FLAG_NOSAVE )        newflags &= ~(lh_meta_save);
        if( flags & LH_FLAG_INDENTTITLE )   newflags |=  (lh_meta_indent);
        if( flags & LH_FLAG_NOSOURCE )      newflags &= ~(lh_meta_source);
        if( flags & LH_FLAG_NOSINK )        newflags &= ~(lh_meta_sink);
        if( flags & LH_FLAG_HIDETITLE )     newflags &= ~(lh_meta_show_title);
        if( flags & LH_FLAG_HIDEVALUE )     newflags &= ~(lh_meta_show_value);
        if( flags & LH_FLAG_MINMAX )
        {
            qWarning() << parent->metaObject()->className()
                       << parent->objectName()
                       << objectName()
                       << "is using LH_FLAG_MIN/MAX";
        }
        qWarning() << parent->metaObject()->className()
                   << parent->objectName()
                   << objectName()
                   << "is using A18 flags";
        flags = newflags;
    }
    Q_ASSERT( flags < lh_meta_flag_unused );

    if( ident )
    {
        // check for ident warnings
        if( *ident == '^' && (flags&lh_meta_show_title) && !(flags&lh_meta_indent) )
        {
            flags &= ~lh_meta_show_title;
            flags |= lh_meta_indent;
            qWarning() << parent->metaObject()->className()
                       << parent->objectName()
                       << objectName()
                       << "removed lh_meta_show_title, added lh_meta_indent";
        }

        if( *ident == '~' && (flags&lh_meta_show_title) )
        {
            flags &= ~lh_meta_show_title;
            qWarning() << parent->metaObject()->className()
                       << parent->objectName()
                       << objectName()
                       << "removed lh_meta_show_title";
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
        if( flags&lh_meta_save )
        {
            flags &= ~lh_meta_save;
            qWarning() << parent->metaObject()->className()
                       << parent->objectName()
                       << objectName()
                       << "removed lh_meta_save";
        }
    }

    item_.size = sizeof(lh_setup_item);
    item_.meta.type = type;
    item_.meta.flags = flags;
    item_.meta.order = ((flags&lh_meta_first)?-1:0) + ((flags&lh_meta_last)?+1:0);
    item_.meta.help = 0;

    memset( &item_.value, 0, sizeof(item_.value) );
    memset( &item_.param, 0, sizeof(item_.param) );
    memset( &item_.link, 0, sizeof(lh_setup_link) );

    item_.obj_value_changed = obj_value_changed;
    item_.obj_param_changed = obj_param_changed;
    item_.obj_link_changed = obj_link_changed;

    parent->callback( lh_cb_setup_create, &item_ );
}

LH_QtSetupItem::~LH_QtSetupItem()
{
    callback( lh_cb_destroy );
    memset( &item_, 0, sizeof(lh_setup_item) );
}

void LH_QtSetupItem::emitSpecific()
{
    if( value_.type() == QVariant::LongLong || value_.type() == QVariant::Int ) emit intChanged( qVariantValue<int>(value_) );
    else if( value_.type() == QVariant::String ) emit stringChanged( qVariantValue<QString>(value_) );
    else if( value_.type() == QVariant::Bool ) emit boolChanged( qVariantValue<bool>(value_) );
    else if( value_.type() == QVariant::Double ) emit doubleChanged( qVariantValue<double>(value_) );
    else if( value_.type() == QVariant::Color ) emit colorChanged( qVariantValue<QColor>(value_) );
    else if( value_.type() == QVariant::Font ) emit fontChanged( qVariantValue<QFont>(value_) );
    else if( value_.type() == QVariant::Image ) emit imageChanged( qVariantValue<QImage>(value_) );
    return;
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

void LH_QtSetupItem::value_changed()
{
    if( meta().flags & lh_meta_autorender ) parent()->requestRender();
    value_.read();
    emit valueChanged();
    emitSpecific();
    return;
}

void LH_QtSetupItem::param_changed()
{
    if( meta().flags & lh_meta_autorender ) parent()->requestRender();
    min_.read();
    max_.read();
    other_.read();
    emit paramChanged();
    return;
}

void LH_QtSetupItem::link_changed()
{
    emit linkChanged();
    return;
}

void LH_QtSetupItem::setLink( const char *path, int flags, const char *filter )
{
    if( link_path_array_ != path || item_.link.flags != flags || link_filter_array_ != filter )
    {
        link_path_array_ = path;
        link_filter_array_ = filter;
        item_.link.path = link_path_array_.isEmpty() ? 0 : link_path_array_.constData();
        item_.link.filter = link_filter_array_.isEmpty() ? 0 : link_filter_array_.constData();
        item_.link.flags = flags;
        refreshLink();
        emit linkSet( item_.link.path, item_.link.flags, item_.link.filter );
    }
    return;
}

void LH_QtSetupItem::setLinkFilter( const char *filter )
{
    if( link_filter_array_ != filter )
    {
        link_filter_array_ = filter;
        item_.link.filter = link_filter_array_.isEmpty() ? 0 : link_filter_array_.constData();
        refreshLink();
    }
}

void LH_QtSetupItem::setHelp( const char *s )
{
    if( help_array_ != s )
    {
        help_array_ = s;
        if( help_array_.isEmpty() ) item_.meta.help = 0;
        else
        {
            const char *newhelp = help_array_.constData();
            item_.meta.help = newhelp;
        }
        refreshMeta();
        emit helpSet( help() );
    }
    return;
}

void LH_QtSetupItem::setHelp( const QString& s )
{
    setHelp( s.toUtf8().constData() );
}

void LH_QtSetupItem::setFlags( int f )
{
    if( flags() != f )
    {
        item_.meta.flags = f;
        refreshMeta();
        emit flagsSet( flags() );
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
        item_.meta.order = neworder;
        refreshMeta();
        emit orderSet( order() );
    }
    return;
}

void LH_QtSetupItem::setType( lh_meta_type newtype )
{
    if( type() == newtype ) return;
    item_.meta.type = newtype;
    refreshMeta();
    return;
}

void LH_QtSetupItem::setValue( const QVariant& newvalue )
{
    if( value_ == newvalue ) return;
    value_.setValue(newvalue);
    value_.write();
    refreshValue();
    emit valueSet();
    emitSpecific();
    return;
}

void LH_QtSetupItem::setMinimum( const QVariant& newmin )
{
    if( min_ == newmin ) return;
    min_.setValue(newmin);
    min_.write();
    refreshParam();
    emit paramSet();
    return;
}

void LH_QtSetupItem::setMaximum( const QVariant& newmax )
{
    if( max_ == newmax ) return;
    max_.setValue(newmax);
    max_.write();
    refreshParam();
    emit paramSet();
    return;
}

void LH_QtSetupItem::setOther( const QVariant& newother )
{
    if( other_ == newother ) return;
    other_.setValue(newother);
    other_.write();
    refreshParam();
    emit paramSet();
    return;
}
