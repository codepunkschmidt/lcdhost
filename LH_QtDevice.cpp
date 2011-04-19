/**
  \file     LH_QtDevice.cpp
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009-2010 Johan Lindh

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
#include "LH_QtDevice.h"

/**
  Device stubs.
  */
#define RECAST(o) reinterpret_cast<LH_QtDevice*>(o)

static lh_setup_item ** obj_setup_data(void *obj) { return RECAST(obj)->setup_data(); }
static void obj_setup_resize(void *obj, lh_setup_item*item, size_t s) { RECAST(obj)->setup_resize(item,s); }
static void obj_setup_change(void *obj, lh_setup_item*item) { RECAST(obj)->setup_change(item); }
static void obj_setup_input(void *obj, lh_setup_item*item, int flags, int value) { RECAST(obj)->setup_input(item,flags,value); }

static const char* obj_open(void*obj) { return RECAST(obj)->open(); }
static const char* obj_render_qimage(void*obj,void*qi) { return RECAST(obj)->render_qimage(static_cast<QImage*>(qi)); }
static const char* obj_render_argb32(void*obj,int w,int h,const void*p) { return RECAST(obj)->render_argb32(w,h,p); }
static const char* obj_render_mono(void*obj,int w,int h,const void*p) { return RECAST(obj)->render_argb32(w,h,p); }
static int obj_buttons(void*obj) { return RECAST(obj)->buttons(); }
static const char* obj_get_backlight(void*obj,lh_device_backlight*p) { return RECAST(obj)->get_backlight(static_cast<lh_device_backlight*>(p)); }
static const char* obj_set_backlight(void*obj,lh_device_backlight*p) { return RECAST(obj)->set_backlight(static_cast<lh_device_backlight*>(p)); }
static const char* obj_close(void*obj) { return RECAST(obj)->close(); }


LH_QtDevice::LH_QtDevice( const char *name, QObject *parent ) : LH_QtObject( name, parent )
{
    memset( &lh_dev_, 0, sizeof(lh_dev_) );
    lh_dev_.size = sizeof(lh_dev_);
    lh_dev_.table.size = sizeof(lh_dev_.table);
    lh_dev_.table.o.size = sizeof(lh_dev_.table.o);
    lh_dev_.table.o.obj_setup_input = obj_setup_input;
    lh_dev_.table.o.obj_setup_resize = obj_setup_resize;
    lh_dev_.table.o.obj_setup_change = obj_setup_change;
    lh_dev_.table.o.obj_setup_data = obj_setup_data;
    lh_dev_.table.obj_open = obj_open;
    lh_dev_.table.obj_render_qimage = obj_render_qimage;
    lh_dev_.table.obj_render_argb32 = obj_render_argb32;
    lh_dev_.table.obj_render_mono = obj_render_mono;
    lh_dev_.table.obj_buttons = obj_buttons;
    lh_dev_.table.obj_get_backlight = obj_get_backlight;
    lh_dev_.table.obj_set_backlight = obj_set_backlight;
    lh_dev_.table.obj_close = obj_close;

    return;
}

LH_QtDevice::~LH_QtDevice()
{
    memset( &lh_dev_, 0, sizeof(lh_dev_) );
    return;
}

void LH_QtDevice::arrive()
{
    Q_ASSERT( !id().isEmpty() );
    Q_ASSERT( !size().isEmpty() );
    Q_ASSERT( depth() > 0 );
    callback(lh_cb_arrive,lh_dev());
}

void LH_QtDevice::leave()
{
    callback(lh_cb_leave,lh_dev());
}

void LH_QtDevice::setId( QString s )
{
    id_ = s.toUtf8();
    lh_dev_.id = id_.constData();
    return;
}

void LH_QtDevice::setName( QString s )
{
    setObjectName(s);
    name_ = s.toUtf8();
    lh_dev_.name = name_.constData();
    return;
}

QString LH_QtDevice::buttonName( int bitmask )
{
    int index = 0;
    while( bitmask && !(bitmask&1) ) { index++; bitmask>>=1; }
    if( index >= LH_DEVICE_MAXBUTTONS || lh_dev_.button[index] == NULL ) return QString();
    return QString::fromUtf8( lh_dev_.button[index] );
}

void LH_QtDevice::setButtonName( int bitmask, QString name )
{
    int index = 0;
    while( bitmask && !(bitmask&1) ) { index++; bitmask>>=1; }
    if( index >= LH_DEVICE_MAXBUTTONS ) return;
    if( name.isEmpty() )
    {
        buttonNames_[index].clear();;
        lh_dev_.button[index] = NULL;
    }
    else
    {
        buttonNames_[index] = name.toUtf8();
        lh_dev_.button[index] = buttonNames_[index].constData();
    }
    return;
}
