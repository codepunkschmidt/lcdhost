/**
  \file     LH_QtInstance.cpp
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009-2011 Johan Lindh

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
#include "LH_QtPlugin.h"
#include "LH_QtInstance.h"

#define RECAST(obj) reinterpret_cast<LH_QtInstance*>(obj)

static void obj_prerender(lh_layout_item *obj)
{
    RECAST(obj->obj.ref)->prerender();
}

static int obj_width(lh_layout_item *obj,int h)
{
    return RECAST(obj->obj.ref)->width(h);
}

static int obj_height(lh_layout_item *obj,int w)
{
    return RECAST(obj->obj.ref)->height(w);
}

static const lh_blob * obj_render_blob(lh_layout_item *obj,int w,int h)
{
    return RECAST(obj->obj.ref)->render_blob(w,h);
}

static void * obj_render_qimage(lh_layout_item *obj,int w,int h)
{
    return RECAST(obj->obj.ref)->render_qimage(w,h);
}

LH_QtInstance::LH_QtInstance( lh_callback_t cb, void* cb_id )
    : LH_QtObject(&li_.obj,LH_QtPlugin::instance()), image_(0)
{
    li_.size = sizeof(lh_layout_item);
    li_.obj.cb = cb;
    li_.obj.cb_id = cb_id;
    li_.obj_prerender = obj_prerender;
    li_.obj_width = obj_width;
    li_.obj_height = obj_height;
    li_.obj_render_blob = obj_render_blob;
    li_.obj_render_qimage = obj_render_qimage;
}

LH_QtInstance::~LH_QtInstance()
{
    if( image_ )
    {
        delete image_;
        image_ = NULL;
    }
}

/**
  Basic QImage handling. Call this at the start of
  your render_qimage to ensure that \c image_ is
  of the correct size. Always check the return
  value, and return NULL if the return value is
  NULL.
  */
QImage *LH_QtInstance::initImage(int w, int h)
{
    if( image_ && ( w != image_->width() || h != image_->height() ) )
    {
        delete image_;
        image_ = NULL;
    }
    if( image_ == NULL )
    {
        image_ = new QImage(w,h,QImage::Format_ARGB32_Premultiplied);
        if( image_ && image_->isNull() )
        {
            delete image_;
            image_ = NULL;
        }
    }
    return image_;
}

