/**
  \file     LH_QtObject.cpp
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009-2011:

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
#include "LH_QtObject.h"

#define RECAST(obj) reinterpret_cast<LH_QtObject*>(obj)

LH_QtLoader *LH_QtLoader::first_ = 0;
void LH_QtLoader::load( LH_QtObject *parent )
{
    for( LH_QtLoader *l=first_; l; l=l->next_ )
        if( l->func_ ) l->func_(parent);
}

static const char *obj_init( lh_object *obj )
{
    return RECAST(obj->ref)->init();
}

static int obj_polling( lh_object *obj )
{
    return RECAST(obj->ref)->polling();
}

static int obj_notify( lh_object *obj, int code, void *param )
{
    return RECAST(obj->ref)->notify(code,param);
}

LH_QtObject::LH_QtObject( lh_object *p, const char *ident, QObject *parent ) :
    QObject( parent ),
    p_obj_( p ),
    clean_init_( false )
{
    // You must *not* create LH_QtObject children before init()
    Q_ASSERT( qobject_cast<LH_QtObject*>(parent) ? qobject_cast<LH_QtObject*>(parent)->isValid() : true );

    memset( p_obj_, 0, sizeof(lh_object) );
    p_obj_->size = sizeof(lh_object);
    p_obj_->ref = this;
    p_obj_->obj_init = obj_init;
    p_obj_->obj_notify = obj_notify;
    p_obj_->obj_polling = obj_polling;
    p_obj_->title = title_array_.constData();

    if( ident )
    {
        strncpy( p_obj_->ident, ident, sizeof(p_obj_->ident)-1 );
        for( char *p = p_obj_->ident; *p; ++p )
        {
            if( *p == '<' ) *p = '(';
            if( *p == '/' ) *p = '|';
            if( *p == '\\' ) *p = '|';
            if( *p == '\"' ) *p = '\'';
            if( *p == '>' ) *p = ')';
        }

        if( strcmp( ident, p_obj_->ident ) )
        {
            QString where;
            if( parent )
            {
                where.append( parent->metaObject()->className() );
                where.append(':');
                where.append( parent->objectName() );
                where.append(' ');
            }
            where.append( metaObject()->className() );
            where.append(' ');
            qWarning() << where
                       << QString(p_obj_->ident)
                       << "ident changed";
        }
    }
    else
    {
        qsnprintf( p_obj_->ident, sizeof(p_obj_->ident)-1,
                   "0x%08X", (qptrdiff)this );
    }
    setObjectName( QString::fromAscii(p_obj_->ident) );
}

LH_QtObject::~LH_QtObject()
{
    p_obj_ = 0;
}

void LH_QtObject::callback( lh_callbackcode code, void *param ) const
{
    Q_ASSERT( p_obj_ );
    if( isValid() )
    {
        p_obj_->cb( p_obj_->cb_id, code, param );
        return;
    }
    qCritical() << "Invalid" << metaObject()->className() << objectName()
                << "is trying to use" << callbackName(code);
    return;
}

const char *LH_QtObject::init()
{
    const char *retv;
    setObjectName( QString::fromAscii(p_obj_->ident) );
    retv = userInit();
    if( !clean_init_ )
    {
        qCritical() << metaObject()->className() << objectName()
                    << "did not complete userInit() chain";
    }
    return retv;
}

const char *LH_QtObject::userInit()
{
    clean_init_ = true;
    return 0;
}

int LH_QtObject::notify( int code, void * )
{
    if( code & LH_NOTE_INITIALIZED ) emit initialized();
    if( code & LH_NOTE_TITLE ) emit titleChanged( title() );
    return 0;
}

void LH_QtObject::setTitle( const char *newtitle )
{
    if( newtitle && title_array_ != newtitle )
    {
        title_array_ = newtitle;
        p_obj_->title = title_array_.constData();
        if( isValid() ) callback( lh_cb_title_refresh );
        emit titleChanged( title() );
    }
    return;
}

void LH_QtObject::setTitle( QString newtitle )
{
    setTitle( newtitle.toUtf8().constData() );
}

QString LH_QtObject::dir_layout() const
{
    const LH_QtObject *par = qobject_cast<const LH_QtObject*>(parent());
    if( par ) return par->dir_layout();
    return dir_data();
}

QString LH_QtObject::dir_binaries() const
{
    const char *p = 0;
    callback( lh_cb_dir_binaries, &p );
    return p ? QString::fromUtf8(p) : QString();
}

QString LH_QtObject::dir_plugins() const
{
    const char *p = 0;
    callback( lh_cb_dir_plugins, &p );
    return p ? QString::fromUtf8(p) : QString();
}

QString LH_QtObject::dir_data() const
{
    const char *p = 0;
    callback( lh_cb_dir_data, &p );
    return p ? QString::fromUtf8(p) : QString();
}

const char *LH_QtObject::callbackName( lh_callbackcode code )
{
    switch(code)
    {
    case lh_cb_utf8_to_local8bit: return "lh_cb_utf8_to_local8bit";
    case lh_cb_load_layout: return "lh_cb_load_layout";
    case lh_cb_dir_binaries: return "lh_cb_dir_binaries";
    case lh_cb_dir_plugins: return "lh_cb_dir_plugins";
    case lh_cb_dir_data: return "lh_cb_dir_data";
    case lh_cb_qvariant_to_data: return "lh_cb_qvariant_to_data";
    case lh_cb_data_to_qvariant: return "lh_cb_data_to_qvariant";
    case lh_cb_unload: return "lh_cb_unload";
    case lh_cb_reload: return "lh_cb_reload";
    case lh_cb_log: return "lh_cb_log";
    case lh_cb_polling: return "lh_cb_polling";
    case lh_cb_notify: return "lh_cb_notify";
    case lh_cb_title_refresh: return "lh_cb_title_refresh";
    case lh_cb_setup_create: return "lh_cb_setup_create";
    case lh_cb_output_create: return "lh_cb_output_create";
    case lh_cb_input_create: return "lh_cb_input_create";
    case lh_cb_class_create: return "lh_cb_class_create";
    case lh_cb_input: return "lh_cb_input";
    case lh_cb_destroy: return "lh_cb_destroy";
    case lh_cb_setup_refresh_meta: return "lh_cb_setup_refresh_meta";
    case lh_cb_setup_refresh_link: return "lh_cb_setup_refresh_link";
    case lh_cb_setup_refresh_param: return "lh_cb_setup_refresh_param";
    case lh_cb_setup_refresh_value: return "lh_cb_setup_refresh_data";
    case lh_cb_render: return "lh_cb_render";
    case lh_cb_sethidden: return "lh_cb_sethidden";
    case lh_cb_unused: return "lh_cb_unused";
    }
    return "(unknown callback)";
}
