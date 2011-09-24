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

static void obj_update_data( lh_setup_item* obj, int datainfo, const void *ptr  )
{
    RECAST(obj->obj.ref)->update_data( datainfo, ptr );
}


#ifdef LH_API5_COMPAT

void LH_QtSetupItem::api5create( const char *ident, int flags, lh_setup_type type )
{
    int metainfo = lh_meta_default;

    api5flags_ = 0;
    api5type_ = type;

    setIdent( ident );
    setFlags( flags );

    switch( type )
    {
    case lh_type_none:

    case lh_type_integer:
    case lh_type_integer_boolean:
    case lh_type_integer_color:
    case lh_type_integer_slider:
    case lh_type_integer_progress:

    case lh_type_double:

    case lh_type_string:
    case lh_type_string_script:
    case lh_type_string_filename:
    case lh_type_string_font:
    case lh_type_string_inputstate:
    case lh_type_string_inputvalue:
    case lh_type_string_button:
    case lh_type_string_htmllink:
    case lh_type_string_list:
    case lh_type_string_listbox:
    case lh_type_string_combobox:

    case lh_type_pointer:
    case lh_type_pointer_qimage:

    case lh_type_array:
    case lh_type_array_png:
    case lh_type_array_qint64:
    case lh_type_array_double:
    case lh_type_array_string:
        Q_ASSERT(0);
        break;
    default:
        Q_ASSERT(0);
        break;
    }

    item_.size = sizeof(lh_setup_item);
    item_.meta.info = metainfo;
    item_.meta.order = ((metainfo&lh_meta_first)?-1:0) + ((metainfo&lh_meta_last)?+1:0);
    item_.meta.help = 0;
    item_.obj_update_data = obj_update_data;
    parent()->callback( lh_cb_setup_create, &item_ );
    return;
}

void LH_QtSetupItem::setFlags( int flags )
{
    Q_ASSERT( api5type_ != lh_type_none );
    if( api5flags_ != flags )
    {
        int metaflags = lh_meta_default;

        if( flags & LH_FLAG_READONLY )      metaflags &= ~(lh_meta_enabled);
        if( flags & LH_FLAG_HIDDEN )        metaflags &= ~(lh_meta_visible);
        if( flags & LH_FLAG_FOCUS)          metaflags |=  (lh_meta_focus);
        if( flags & LH_FLAG_AUTORENDER )    metaflags |=  (lh_meta_autorender);
        if( flags & LH_FLAG_FIRST )         metaflags |=  (lh_meta_first);
        if( flags & LH_FLAG_LAST )          metaflags |=  (lh_meta_last);
        if( flags & LH_FLAG_INDENTTITLE )   metaflags |=  (lh_meta_indent);
        if( flags & LH_FLAG_HIDETITLE )     metaflags &= ~(lh_meta_show_title);
        if( flags & LH_FLAG_HIDEVALUE )     metaflags &= ~(lh_meta_show_value);
        /*
        if( flags & LH_FLAG_NOSAVE )        metainfo &= ~(lh_meta_save);
        if( flags & LH_FLAG_NOSOURCE )      metainfo &= ~(lh_meta_source);
        if( flags & LH_FLAG_NOSINK )        metainfo &= ~(lh_meta_sink);
        */
        if( flags & LH_FLAG_MINMAX )
        {
            qWarning() << parent()->metaObject()->className()
                       << parent()->objectName()
                       << objectName()
                       << "is using LH_FLAG_MIN/MAX";
        }

        api5flags_ = flags;
        item_.meta.info = (item_.meta.info&~lh_meta_flag_mask) | metaflags;
        refreshMeta();
        emit flagsSet( flags );
    }
}

void LH_QtSetupItem::setFlag( int f, bool state )
{
    int newflags = flags() & ~f;
    if( state ) newflags |= f;
    setFlags( newflags );
    return;
}

#endif

void LH_QtSetupItem::setIdent( const char *ident )
{
    if( ident )
    {
        // check for ident warnings
        if( *ident == '^' && (info()&lh_meta_show_title) && !(info()&lh_meta_indent) )
        {
            item_.meta.info &= ~lh_meta_show_title;
            item_.meta.info |= lh_meta_indent;
            qWarning() << parent()->metaObject()->className()
                       << parent()->objectName()
                       << objectName()
                       << "removed lh_meta_show_title, added lh_meta_indent";
        }

        if( *ident == '~' && (info()&lh_meta_show_title) )
        {
            item_.meta.info &= ~lh_meta_show_title;
            qWarning() << parent()->metaObject()->className()
                       << parent()->objectName()
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
        if( isSaved() )
        {
            qWarning() << parent()->metaObject()->className()
                       << parent()->objectName()
                       << objectName()
                       << "isSaving() with autogenerated id";
        }
    }
}

void LH_QtSetupItem::create( const char *ident, int metainfo )
{
#ifdef LH_API5_COMPAT
    api5flags_ = 0;
    api5type_ = lh_type_none;
#endif

    Q_ASSERT( metainfo >= 0 );
    Q_ASSERT( metainfo < lh_meta_flag_unused );

    setIdent( ident );
    item_.size = sizeof(lh_setup_item);
    item_.meta.info = metainfo;
    item_.meta.order = ((metainfo&lh_meta_first)?-1:0) + ((metainfo&lh_meta_last)?+1:0);
    item_.meta.help = 0;
    item_.obj_update_data = obj_update_data;
    parent()->callback( lh_cb_setup_create, &item_ );
}

LH_QtSetupItem::~LH_QtSetupItem()
{
    callback( lh_cb_destroy );
    memset( &item_, 0, sizeof(lh_setup_item) );
}

void LH_QtSetupItem::update_data( const lh_meta prop, const void *ptr )
{
    QList<LH_QVariant>::iterator it;
    for( it = properties_.begin(); it != properties_.end(); ++ it )
    {
        if( it->role() == lh_prop_role(prop) )
        {
            if( lh_data_format(prop) == lh_format_qvariant )
                *it = *(const QVariant*)ptr;
            else
                *it = lh_qvariant_from_setup_data( prop, ptr );
            switch( lh_data_role(prop) )
            {
            case lh_role_value:
                emit valueChanged( *it );
                emitSpecific();
                break;
            case lh_role_minimum:
                emit minimumChanged( *it );
                break;
            case lh_role_maximum:
                emit maximumChanged( *it );
                break;
            case lh_role_list:
                emit listChanged( *it );
                break;
            case lh_role_link:
                emit linkChanged( *it );
                break;
            default:
                emit userChanged( *it );
                break;
            }
            return;
        }
    }
}

const LH_QVariant& LH_QtSetupItem::data( int role ) const
{
    QList<LH_QVariant>::const_iterator it;
    for( it = properties_.constBegin(); it != properties_.constEnd(); ++ it )
        if( it->role() == role ) return *it;
    return LH_QVariant::invalid();
}

bool LH_QtSetupItem::isSaved() const
{
    QList<LH_QVariant>::const_iterator it;
    for( it = properties_.constBegin(); it != properties_.constEnd(); ++ it )
        if( it->isSaved() ) return true;
    return false;
}

void LH_QtSetupItem::emitSpecific()
{
    const LH_QVariant& v = value();
    switch( v.type() )
    {
    case QVariant::LongLong:
    case QVariant::Int:
        emit intChanged( qVariantValue<int>(v) );
        break;
    case QVariant::String:
        emit stringChanged( qVariantValue<QString>(v) );
        break;
    case QVariant::Bool:
        emit boolChanged( qVariantValue<bool>(v) );
        break;
    case QVariant::Double:
        emit doubleChanged( qVariantValue<double>(v) );
        break;
    case QVariant::Color:
        emit colorChanged( qVariantValue<QColor>(v) );
        break;
    case QVariant::Font:
        emit fontChanged( qVariantValue<QFont>(v) );
        break;
    case QVariant::Image:
        emit imageChanged( qVariantValue<QImage>(v) );
        break;
    default:
        break;
    }
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

void LH_QtSetupItem::setProperty( const LH_QVariant& newdata )
{
    QList<LH_QVariant>::iterator it;
    for( it = properties_.begin(); it != properties_.end(); ++ it )
    {
        if( it->role() == newdata.role() )
        {
            if( newdata == *it ) return;
            *it = newdata;
            break;
        }
    }

    if( it == properties_.end() )
        properties_.append(newdata);

    switch( newdata.role() )
    {
    case lh_role_value:
        emit valueSet(newdata);
        break;
    case lh_role_minimum:
        emit minimumSet(newdata);
        break;
    case lh_role_maximum:
        emit maximumSet(newdata);
        break;
    case lh_role_list:
        emit listSet(newdata);
        break;
    case lh_role_link:
        emit linkSet(newdata);
        break;
    case lh_role_user:
        emit userSet(newdata);
        break;
    }

    return;
}
