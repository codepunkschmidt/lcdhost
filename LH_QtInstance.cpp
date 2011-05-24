/**
  \file     LH_QtInstance.cpp
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
#include "LH_QtInstance.h"

LH_QtClassLoader *LH_QtClassLoader::first_ = NULL;
static QList<lh_layout_class> *manual_list_ = NULL;
static lh_class **classlist_ = NULL;

#define RECAST(obj) reinterpret_cast<LH_QtInstance*>(obj)
static void obj_prerender(void *obj) { RECAST(obj)->prerender(); }
static int obj_width(void *obj,int h) { return RECAST(obj)->width(h); }
static int obj_height(void *obj,int w) { return RECAST(obj)->height(w); }
static const lh_blob * obj_render_blob(void *obj,int w,int h) { return RECAST(obj)->render_blob(w,h); }
static void * obj_render_qimage(void *obj,int w,int h) { return RECAST(obj)->render_qimage(w,h); }
static void obj_delete(void *obj) { delete RECAST(obj); }

void LH_QtInstance::build_calltable( lh_instance_calltable *ct, lh_class_factory_t cf )
{
    if( ct )
    {
        LH_QtObject::build_calltable( & ct->o );
        ct->size = sizeof(lh_instance_calltable);
        ct->obj_new = cf;
        ct->obj_prerender = obj_prerender;
        ct->obj_width = obj_width;
        ct->obj_height = obj_height;
        ct->obj_render_blob = obj_render_blob;
        ct->obj_render_qimage = obj_render_qimage;
        ct->obj_delete = obj_delete;
    }
    return;
}

void LH_QtInstance::term()
{
    if( image_ )
    {
        delete image_;
        image_ = NULL;
    }
    LH_QtObject::term();
    return;
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

/**
  Exported from the plugin.
*/
EXPORT lh_class ** lh_class_list(void)
{
    int count = 0;

    // count and check classes
    for( LH_QtClassLoader *load=LH_QtClassLoader::first_; load; load=load->next_)
    {
        Q_ASSERT( load->info_ != NULL );
        lh_class *p = load->info_();
        if( p )
        {
            LH_QtInstance::build_calltable( &p->table, load->factory_ );
            ++ count;
        }
    }
    if( manual_list_ )
    {
        for( int i=0; i<manual_list_->size(); ++i )
        {
            lh_class *p = manual_list_->at(i).info();
            if( p )
            {
                LH_QtInstance::build_calltable( &p->table, manual_list_->at(i).factory() );
                ++ count;
            }
        }
    }

    // free the old list, if any
    if( classlist_ )
    {
        free( classlist_ );
        classlist_ = 0;
    }

    // allocate list and fill it
    if( count > 0 )
    {
        int n = 0;
        classlist_ = (lh_class**) malloc( sizeof(lh_class*) * (count+1) );
        for( LH_QtClassLoader *load=LH_QtClassLoader::first_; load; load=load->next_)
        {
            if( n<count && load->info_() )
                classlist_[n++] = load->info_();
        }
        if( manual_list_ )
        {
            for( int i=0; i<manual_list_->size(); ++i )
                if( n<count && manual_list_->at(i).info() )
                    classlist_[n++] = manual_list_->at(i).info();
        }
        Q_ASSERT( n==count );
        classlist_[n] = NULL;
    }

    return classlist_;
}

void lh_add_class( lh_class *p, lh_class_factory_t f )
{
    if( manual_list_ == NULL ) manual_list_ = new QList<lh_layout_class>();
    for( int i=0; i<manual_list_->size(); ++i )
        if( manual_list_->at(i).info() == p ) return;
    manual_list_->append(lh_layout_class(p,f));
    LH_QtObject::lh_callback( 0, lh_cb_class_refresh, 0 );
}

void lh_remove_class( lh_class *p )
{
    if( manual_list_ == NULL ) return;
    for( int i=0; i<manual_list_->size(); ++i )
        if( manual_list_->at(i).info() == p )
            manual_list_->removeAt(i), i=0;
    LH_QtObject::lh_callback( 0, lh_cb_class_refresh, 0 );
}

