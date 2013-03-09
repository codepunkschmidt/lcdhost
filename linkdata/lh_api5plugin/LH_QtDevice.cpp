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

static const char* obj_open(void*obj) { return RECAST(obj)->open(); }
static const char* obj_render_qimage(void*obj,void*qi) { return RECAST(obj)->render_qimage(static_cast<QImage*>(qi)); }
static const char* obj_render_argb32(void*obj,int w,int h,const void*p) { return RECAST(obj)->render_argb32(w,h,p); }
static const char* obj_render_mono(void*obj,int w,int h,const void*p) { return RECAST(obj)->render_argb32(w,h,p); }
static const char* obj_get_backlight(void*obj,lh_device_backlight*p) { return RECAST(obj)->get_backlight(static_cast<lh_device_backlight*>(p)); }
static const char* obj_set_backlight(void*obj,lh_device_backlight*p) { return RECAST(obj)->set_backlight(static_cast<lh_device_backlight*>(p)); }
static const char* obj_close(void*obj) { return RECAST(obj)->close(); }


LH_QtDevice::LH_QtDevice( LH_QtObject *parent ) : LH_QtObject( parent )
{
    memset( &lh_dev_, 0, sizeof(lh_dev_) );
    LH_QtObject::build_object_calltable( & lh_dev_.objtable );
    lh_dev_.size = sizeof(lh_dev_);
    lh_dev_.obj = this;
    lh_dev_.table.size = sizeof(lh_dev_.table);
    lh_dev_.table.obj_open = obj_open;
    lh_dev_.table.obj_render_qimage = obj_render_qimage;
    lh_dev_.table.obj_render_argb32 = obj_render_argb32;
    lh_dev_.table.obj_render_mono = obj_render_mono;
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
    Q_ASSERT( !devid().isEmpty() );
    Q_ASSERT( !size().isEmpty() );
    Q_ASSERT( depth() > 0 );
    if( parent() ) parent()->callback(lh_cb_arrive,lh_dev());
}

void LH_QtDevice::leave()
{
    if( parent() ) parent()->callback(lh_cb_leave,lh_dev());
}

void LH_QtDevice::setDevid(const QString &a)
{
    devid_ = a;
    devid_latin1_ = devid_.toLatin1();
    lh_dev_.devid = devid_latin1_.constData();
    return;
}

void LH_QtDevice::setObjectName(const QString &s)
{
    QObject::setObjectName(s);
    name_utf8_ = s.toUtf8();
    lh_dev_.name = name_utf8_.constData();
    return;
}
