/**
  \file     LH_QtPlugin.cpp
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

#include <QtDebug>
#include "LH_QtPlugin.h"
#include "LH_QtInstance.h"

LH_SIGNATURE();

LH_QtLayoutClassLoader *LH_QtLayoutClassLoader::first_ = NULL;
LH_QtLayoutClassLoader::LH_QtLayoutClassLoader(
    const lh_class *classinfo,
    obj_layout_item_create_t fc,
    obj_layout_item_destroy_t fd )
{
    Q_ASSERT( classinfo );
    Q_ASSERT( classinfo->size == sizeof(lh_class) );
    Q_ASSERT( fc );
    Q_ASSERT( fd );
    memset( &cls_, 0, sizeof(lh_layout_class) );
    cls_.obj.size = sizeof(lh_object);
    cls_.size = sizeof(lh_layout_class);
    if( classinfo )
    {
        cls_.path = classinfo->path;
        cls_.ident = classinfo->ident;
        cls_.name = classinfo->name;
        cls_.width = classinfo->width;
        cls_.height = classinfo->height;
    }
    cls_.obj_layout_item_create = fc;
    cls_.obj_layout_item_destroy = fd;
    next_ = first_;
    first_ = this;
}

LH_QtPlugin *LH_QtPlugin::instance_ = 0;

QString LH_QtPlugin::dir_binaries()
{
    static QString retv;
    if( retv.isEmpty() && instance_ )
    {
        const char *p = 0;
        instance_->callback( lh_cb_dir_binaries, &p );
        retv = QString::fromUtf8(p);
    }
    return retv;
}

QString LH_QtPlugin::dir_plugins()
{
    static QString retv;
    if( retv.isEmpty() && instance_ )
    {
        const char *p = 0;
        instance_->callback( lh_cb_dir_plugins, &p );
        retv = QString::fromUtf8(p);
    }
    return retv;
}

QString LH_QtPlugin::dir_data()
{
    static QString retv;
    Q_ASSERT( instance_ );
    if( retv.isEmpty() && instance_ )
    {
        const char *p = 0;
        instance_->callback( lh_cb_dir_data, &p );
        retv = QString::fromUtf8(p);
        Q_ASSERT( retv.endsWith('/') );
    }
    return retv;
}

LH_QtPlugin::LH_QtPlugin( lh_callback_t cb, void* cb_id ) :
    LH_QtObject(&obj_,0)
{
    Q_ASSERT( instance_ == 0 );
    obj_.cb = cb;
    obj_.cb_id = cb_id;
    instance_ = this;
}

LH_QtPlugin::~LH_QtPlugin()
{
    for( LH_QtLayoutClassLoader *to_load = LH_QtLayoutClassLoader::first_; to_load; to_load=to_load->next_ )
    {
        Q_ASSERT( to_load->cls_.obj.size == sizeof(lh_object) );
        Q_ASSERT( to_load->cls_.size == sizeof(lh_layout_class) );
        if( to_load->cls_.obj.cb && to_load->cls_.obj.cb_id )
            to_load->cls_.obj.cb( to_load->cls_.obj.cb_id, lh_cb_destroy, 0 );
    }
    Q_ASSERT( instance_ == this );
    instance_ = 0;
}

const char *LH_QtPlugin::userInit()
{
    if( const char *err = LH_QtObject::userInit() ) return err;
    for( LH_QtLayoutClassLoader *to_load = LH_QtLayoutClassLoader::first_; to_load; to_load=to_load->next_ )
    {
        Q_ASSERT( to_load->cls_.obj.size == sizeof(lh_object) );
        Q_ASSERT( to_load->cls_.size == sizeof(lh_layout_class) );
        callback( lh_cb_class_create, &to_load->cls_ );
    }
    return 0;
}

void LH_QtPlugin::requestReload( const char *msg )
{
    callback( lh_cb_reload, (void*) msg );
}
