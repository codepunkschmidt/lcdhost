/**
  \file     LH_QtObject.cpp
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
#include <QTextStream>
#include "LH_QtObject.h"
#include "LH_QtSetupItem.h"

#define RECAST(obj) reinterpret_cast<LH_QtObject*>(obj)

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

LH_QtObject::LH_QtObject( lh_object *p, QObject *parent ) : QObject( parent ), p_obj_(p)
#ifndef QT_NO_DEBUG
  ,clean_init_(false)
#endif
{
    Q_ASSERT( p_obj_ );
    memset( p_obj_, 0, sizeof(lh_object) );
    p_obj_->size = sizeof(lh_object);
    p_obj_->ref = this;
    p_obj_->obj_init = obj_init;
    p_obj_->obj_notify = obj_notify;
    p_obj_->obj_polling = obj_polling;
}

LH_QtObject::LH_QtObject( lh_object *p, const char *ident, QObject *parent ) : QObject( parent ), p_obj_(p)
#ifndef QT_NO_DEBUG
  ,clean_init_(false)
#endif
{
    Q_ASSERT( p_obj_ );
    memset( p_obj_, 0, sizeof(lh_object) );
    p_obj_->size = sizeof(lh_object);
    p_obj_->ref = this;
    p_obj_->obj_init = obj_init;
    p_obj_->obj_notify = obj_notify;
    p_obj_->obj_polling = obj_polling;

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
                if( parent->metaObject() )
                {
                    where.append( parent->metaObject()->className() );
                    where.append(':');
                }
                where.append( parent->objectName() );
                where.append(' ');
            }
            if( metaObject() )
            {
                where.append( metaObject()->className() );
                where.append(' ');
            }
            qWarning() << where
                       << QString(p_obj_->ident)
                       << "ident changed";
        }
    }
    else
    {
        QByteArray ba = QString("%1:%2").arg(parent->objectName()).arg((qptrdiff)this,0,36).toAscii();
        ba.truncate( LH_MAX_IDENT-1 );
        strncpy( p_obj_->ident, ba.constData(), sizeof(p_obj_->ident)-1 );
    }

    setObjectName( QString::fromAscii(p_obj_->ident) );
}

LH_QtObject::~LH_QtObject()
{
}

const char *LH_QtObject::init()
{
    const char *retv = 0;

    Q_ASSERT( isValid() );
    setObjectName( QString::fromAscii(p_obj_->ident) );

#ifndef QT_NO_DEBUG
    // Make sure there's no children yet, as constructor should be 'clean'
    if( children().count() )
        qWarning() << metaObject()->className() << objectName() << "has children before init()";
#endif

    retv = userInit();

#ifndef QT_NO_DEBUG
    if( !clean_init_ )
        qWarning() << metaObject()->className() << objectName() << "did not complete userInit() chain";
#endif

    return retv;
}

const char *LH_QtObject::userInit()
{
#ifndef QT_NO_DEBUG
    clean_init_ = true;
#endif
    return 0;
}

int LH_QtObject::notify( int code, void * )
{
    if( code & LH_NOTE_INITIALIZED )
    {
        emit initialized();
    }
    return 0;
}

void LH_QtObject::setTitle(const char *s)
{
    title_array_ = QByteArray(s);
    p_obj_->title = title_array_.data();
    callback( lh_cb_title_refresh );
    return;
}

void LH_QtObject::setTitle(const QString& s)
{
    title_array_ = s.toUtf8();
    p_obj_->title = title_array_.data();
    callback( lh_cb_title_refresh );
    return;
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
    return QString::fromUtf8(p);
}

QString LH_QtObject::dir_plugins() const
{
    const char *p = 0;
    callback( lh_cb_dir_plugins, &p );
    return QString::fromUtf8(p);
}

QString LH_QtObject::dir_data() const
{
    const char *p = 0;
    callback( lh_cb_dir_data, &p );
    return QString::fromUtf8(p);
}

#if 0
const char *LH_QtObject::input_name( const char *devid, int item )
{
    static QByteArray ary;
    if( devid == 0 ) return 0;
    if( item == 0 ) return devid;
    QTextStream s(&ary);
    s << devid << '/' << forcesign << item;
    return ary.constData();
}
#endif
