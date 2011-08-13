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
#include "LH_QtOutputDevice.h"
#include "LH_QtPlugin.h"

/**
  Device stubs.
  */
#define RECAST(o) reinterpret_cast<LH_QtOutputDevice*>(o)

static const char* obj_open(lh_output_device*obj)
{
    return RECAST(obj->obj.ref)->open();
}

static const char* obj_render_qimage(lh_output_device*obj,void*qi)
{
    return RECAST(obj->obj.ref)->render_qimage(static_cast<QImage*>(qi));
}

static const char* obj_render_argb32(lh_output_device*obj,int w,int h,const void*p)
{
    return RECAST(obj->obj.ref)->render_argb32(w,h,p);
}

static const char* obj_render_mono(lh_output_device*obj,int w,int h,const void*p)
{
    return RECAST(obj->obj.ref)->render_argb32(w,h,p);
}

static const char* obj_get_backlight(lh_output_device*obj,lh_device_backlight*p)
{
    return RECAST(obj->obj.ref)->get_backlight(static_cast<lh_device_backlight*>(p));
}

static const char* obj_set_backlight(lh_output_device*obj,lh_device_backlight*p)
{
    return RECAST(obj->obj.ref)->set_backlight(static_cast<lh_device_backlight*>(p));
}

static const char* obj_close(lh_output_device*obj)
{
    return RECAST(obj->obj.ref)->close();
}

LH_QtOutputDevice::LH_QtOutputDevice( const char *ident, int w, int h, int d, bool noauto ) :
    LH_QtObject( &lh_dev_.obj, ident, LH_QtPlugin::instance() )
{
    lh_dev_.size = sizeof(lh_output_device);
    lh_dev_.width = w;
    lh_dev_.height = h;
    lh_dev_.depth = d;
    lh_dev_.noauto = noauto ? 1 : 0;
    lh_dev_.obj_open = obj_open;
    lh_dev_.obj_render_qimage = obj_render_qimage;
    lh_dev_.obj_render_argb32 = obj_render_argb32;
    lh_dev_.obj_render_mono = obj_render_mono;
    lh_dev_.obj_get_backlight = obj_get_backlight;
    lh_dev_.obj_set_backlight = obj_set_backlight;
    lh_dev_.obj_close = obj_close;

    parent()->callback( lh_cb_output_create, &lh_dev_ );
    return;
}

LH_QtOutputDevice::~LH_QtOutputDevice()
{
    callback( lh_cb_destroy );
    memset( &lh_dev_, 0, sizeof(lh_dev_) );
    return;
}
