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

#include "LH_QtPlugin.h"
#include "LH_QtInstance.h"

LH_SIGNATURE();

LH_QtPlugin *LH_QtPlugin::instance_ = NULL;

static void *lh_id_ = NULL;
static lh_callback_t lh_callback_ = NULL;
static lh_systemstate *lh_systemstate_ = NULL;

LH_QtClassLoader *LH_QtClassLoader::first_ = NULL;
static QList<lh_layout_class> *manual_list_ = NULL;

void lh_callback( const void *obj, lh_callbackcode_t code, void *param )
{
    if( lh_id_ && lh_callback_ )
        lh_callback_(lh_id_,obj,code,param);
}

lh_systemstate *lcdhost_state()
{
    return lh_systemstate_;
}

/**
  Exported from all LCDHost shared libraries.
*/
EXPORT const lh_buildinfo* lh_version(int amaj, int amin )
{
    Q_ASSERT( LH_QtPlugin::instance() != NULL );
    return LH_QtPlugin::instance()->lh_version( amaj, amin );
}
EXPORT const char * lh_name(void) { return LH_QtPlugin::instance()->lh_name(); }
EXPORT const char * lh_shortdesc(void) { return LH_QtPlugin::instance()->lh_shortdesc(); }
EXPORT const char * lh_author(void) { return LH_QtPlugin::instance()->lh_author(); }
EXPORT const char * lh_homepage(void) { return LH_QtPlugin::instance()->lh_homepage(); }
EXPORT const char * lh_longdesc(void) { return LH_QtPlugin::instance()->lh_longdesc(); }
EXPORT const lh_blob *lh_logo(void) { return LH_QtPlugin::instance()->lh_logo(); }
EXPORT int lh_polling(void) { return LH_QtPlugin::instance()->lh_polling(); }
EXPORT int lh_notify(int code, void *param) { return LH_QtPlugin::instance()->lh_notify(code,param); }

EXPORT const char * lh_load( void *id, lh_callback_t p_callback, lh_systemstate *p_state )
{
    lh_id_ = id;
    lh_callback_ = p_callback;
    lh_systemstate_ = p_state;
    return LH_QtPlugin::instance()->lh_load();
}

EXPORT void lh_unload(void)
{
    LH_QtPlugin::instance()->lh_unload();
    lh_id_ = NULL;
    lh_callback_ = NULL;
    lh_systemstate_ = NULL;
}

/**
  Stubs for the class calltable.
*/
#define RECAST(o) reinterpret_cast<LH_QtInstance*>(o)

static lh_setup_item ** obj_setup_data(void *obj) { return RECAST(obj)->setup_data(); }
static void obj_setup_resize(void *obj, lh_setup_item*item,size_t s) { RECAST(obj)->setup_resize(item,s); }
static void obj_setup_change(void *obj, lh_setup_item*item) { RECAST(obj)->setup_change(item); }
static void obj_setup_input(void *obj, lh_setup_item*item, int flags, int value) { RECAST(obj)->setup_input(item,flags,value); }

static int obj_polling(void *obj) { return RECAST(obj)->polling(); }
static int obj_notify(void *obj,int n,void*p) { return RECAST(obj)->notify(n,p); }
static void obj_prerender(void *obj) { RECAST(obj)->prerender(); }
static int obj_width(void *obj,int h) { return RECAST(obj)->width(h); }
static int obj_height(void *obj,int w) { return RECAST(obj)->height(w); }
static const lh_blob * obj_render_blob(void *obj,int w,int h) { return RECAST(obj)->render_blob(w,h); }
static void * obj_render_qimage(void *obj,int w,int h) { return RECAST(obj)->render_qimage(w,h); }
static void obj_delete(void *obj) { delete RECAST(obj); }

static void checkClass( lh_class *p, lh_class_factory_t f )
{
    if( p )
    {
        p->table.o.size = sizeof(p->table.o);
        p->table.o.obj_setup_data = obj_setup_data;
        p->table.o.obj_setup_resize = obj_setup_resize;
        p->table.o.obj_setup_change = obj_setup_change;
        p->table.o.obj_setup_input = obj_setup_input;
        p->table.o.obj_polling = obj_polling;
        p->table.o.obj_notify = obj_notify;

        p->table.size = sizeof(p->table);
        p->table.obj_new = (void*(*)(const char *,const lh_class*)) f;
        p->table.obj_prerender = obj_prerender;
        p->table.obj_width = obj_width;
        p->table.obj_height = obj_height;
        p->table.obj_render_blob = obj_render_blob;
        p->table.obj_render_qimage = obj_render_qimage;
        p->table.obj_delete = obj_delete;
    }
}

static lh_class **classlist_ = NULL;

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
            checkClass( p, load->factory_ );
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
                checkClass( p, manual_list_->at(i).factory() );
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
    lh_callback( 0, lh_cb_class_refresh, 0 );
}

void lh_remove_class( lh_class *p )
{
    if( manual_list_ == NULL ) return;
    for( int i=0; i<manual_list_->size(); ++i )
        if( manual_list_->at(i).info() == p )
            manual_list_->removeAt(i), i=0;
    lh_callback( 0, lh_cb_class_refresh, 0 );
}

