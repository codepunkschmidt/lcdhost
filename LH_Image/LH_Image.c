/**
  \file     LH_Image.c
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009 Johan Lindh

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <assert.h>

#include "LH_Image.h"
#include "logo_blob.h"
#include "../lh_plugin.h"

#ifndef Q_UNUSED
# if defined(Q_CC_INTEL) && !defined(Q_OS_WIN) || defined(Q_CC_RVCT)
template <typename T>
inline void qUnused(T &x) { (void)x; }
#   define Q_UNUSED(x) qUnused(x);
# else
#   define Q_UNUSED(x) (void)x;
# endif
#endif

LH_SIGNATURE();

char __lcdhostplugin_xml[] =
"<?xml version=\"1.0\"?>"
"<lcdhostplugin>"
  "<id>Image</id>"
  "<rev>" STRINGIZE(REVISION) "</rev>"
  "<api>" STRINGIZE(LH_API_MAJOR) "." STRINGIZE(LH_API_MINOR) "</api>"
  "<ver>" "r" STRINGIZE(REVISION) "</ver>"
  "<versionurl>http://www.linkdata.se/lcdhost/version.php?arch=$ARCH</versionurl>"
  "<author>Johan \"SirReal\" Lindh</author>"
  "<homepageurl><a href=\"http://www.linkdata.se/software/lcdhost\">Link Data Stockholm</a></homepageurl>"
  "<logourl></logourl>"
  "<shortdesc>"
    "Display static images."
  "</shortdesc>"
  "<longdesc>"
    "Show the contents of image files on screen. This plugin comes with "
    "source code illustrating how a plugin written in C might look."
  "</longdesc>"
"</lcdhostplugin>";

/**************************************************************************
** Object Image
**************************************************************************/

/**
  This structure is what we'll be using as a layout item
  of our single layout class.
  */
typedef struct _lh_image_s
{
    lh_layout_item item_;
    char filename[256];
    lh_blob *blob;
    lh_setup_item setup_filename;
} lh_image;

/**
  React to changes in setup immediately. We don't need to check which
  item is changed since we only have one. To illustrate self-unloading,
  if the filename equals 'unload', we request unloading.
  */
static void image_value_changed( lh_setup_item *obj )
{
    lh_image *img = (void*) obj->obj.ref;
    if( img )
    {
        if( !strcmp( (char*)obj->data.value.data.b.p, "unload" ) )
        {
            strcpy( (char*)obj->data.value.data.b.p, "was-unloaded" );
            obj->obj.cb( obj->obj.cb_id, lh_cb_setup_refresh_value, 0 );
            obj->obj.cb( obj->obj.cb_id, lh_cb_unload, "Filename was 'unload'." );
        }
        else
        {
            if( img->blob )
            {
                free( img->blob );
                img->blob = NULL;
            }
            img->item_.obj.cb( img->item_.obj.cb_id, lh_cb_render, 0 );
        }
    }
    return;
}

/**
  On init, report our single setup item.
  */
static const char *image_init(lh_object *obj)
{
    lh_image *img = (void*) obj;

    img->setup_filename.obj.size = sizeof(lh_object);
    img->setup_filename.obj.ref = img;
    img->setup_filename.size = sizeof(lh_setup_item);
    strcpy( img->setup_filename.obj.ident, "Filename");
    img->setup_filename.meta.type = lh_type_string_filename;
    img->setup_filename.meta.flags = lh_meta_default;
    img->setup_filename.data.value.fmt = lh_format_string;
    img->setup_filename.data.value.data.b.p = img->filename;
    img->setup_filename.data.value.data.b.n = sizeof(img->filename);
    img->setup_filename.obj_value_changed = image_value_changed;
    img->item_.obj.cb( img->item_.obj.cb_id, lh_cb_setup_create, &img->setup_filename );

    return 0;
}

/**
  If the image loading has failed, we'll retry every second
  */
static int image_notify( lh_object *obj, int note, void *param )
{
    lh_image *img = (void*) obj;
    Q_UNUSED(param);
    if( img && ((!note) || (note&LH_NOTE_SECOND)) )
    {
        if( img->blob == 0 )
            img->item_.obj.cb( img->item_.obj.cb_id, lh_cb_render, 0 );
    }
    return LH_NOTE_SECOND;
}

static int is_absolute( const char *filename )
{
    const char *p;
    if( !filename || !*filename ) return 0;
    p = filename;
    while( *p && isspace(*p) ) p ++;
    if( *p == '\\' || *p == '/' ) return 1;
    if( isalpha(*p) && *(p+1) == ':' ) return 1;
    return 0;
}

/**
  We'll just use the provided lh_binaryfile_to_blob()
  function to allocate and fill a blob with the file
  data. Let LCDHost handle B/W color matching and
  sizing issues that may arise.
  */
static const lh_blob * image_render_blob(lh_layout_item *obj,int w,int h)
{
    char *fullname;
    size_t n;
    lh_image *img = (void*) obj;

    Q_UNUSED(w);
    Q_UNUSED(h);

    if( img->blob == NULL )
    {
        if( !is_absolute(img->filename) )
        {
            const char *dir_layout = img->item_.layout.dir;
            if( dir_layout == 0 ) dir_layout = "./";
            n = strlen( dir_layout ) + strlen( img->filename ) + 1;
            fullname = malloc( n );
            if( fullname )
            {
                strcpy( fullname, dir_layout );
                strcat( fullname, img->filename );
                /* convert it from UTF-8 to local 8 bit */
                img->item_.obj.cb( img->item_.obj.cb_id, lh_cb_utf8_to_local8bit, fullname );
                img->blob = lh_binaryfile_to_blob( fullname );
                free( fullname );
            }
        }
        else
            img->blob = lh_binaryfile_to_blob( img->filename );
    }

    return img->blob ? img->blob : logo_blob;
}

/**
  image_new() allocates a new instance of this class and returns it.
  */
static lh_layout_item* image_new( lh_layout_class *cls )
{
    lh_image *img;
    Q_UNUSED(cls);

    img = malloc( sizeof(lh_image) );
    if( img )
    {
        memset( img, 0, sizeof(lh_image) );
        img->item_.obj.size = sizeof( lh_object );
        img->item_.obj.obj_init = image_init;
        img->item_.obj.obj_notify = image_notify;

        img->item_.size = sizeof(lh_layout_item);
        img->item_.obj_render_blob = image_render_blob;

        img->blob = NULL;
        memset( &img->setup_filename, 0, sizeof(lh_setup_item) );
        return (lh_layout_item*) img;
    }

    return NULL;
}

/**
  Free resources.
  */
static void image_delete( lh_layout_class *cls, lh_layout_item *obj )
{
    Q_UNUSED(cls);
    lh_image *img = (void*) obj;
    if( img->blob ) free( img->blob );
    if( img->setup_filename.obj.cb_id )
        img->setup_filename.obj.cb( img->setup_filename.obj.cb_id, lh_cb_destroy, 0 );
    memset( img, 0, sizeof(lh_image) );
    free( img );
    return;
}

/**************************************************************************
** Our layout class object
**************************************************************************/

static lh_layout_class image_class =
{
    {
        sizeof(lh_object),
        0, /* ref */
        0, /* cb_id */
        0, /* cb */
        0, /* obj_init */
        0, /* obj_polling */
        0, /* obj_notify */
        0, /* obj_realloc */
        "StaticImage", /* ident */
        "Image" /* title */
    },
    sizeof(lh_layout_class),
    "Static", /* path */
    -1,-1, /* default size */
    image_new,
    image_delete
};

static const char *image_plugin_init( lh_object *o )
{
    o->cb( o->cb_id, lh_cb_class_create, &image_class );
    return 0;
}


/**************************************************************************
** Our plugin object
**************************************************************************/

static lh_object image_plugin =
{
    sizeof(lh_object),
    0, /* ref */
    0, /* cb_id */
    0, /* cb */
    image_plugin_init, /* to register our single class */
    0, /* obj_polling */
    0, /* obj_notify */
    0, /* obj_realloc */
    {}, /* ident */
    "Image", /* title */
};

/**************************************************************************
** Exported functions
**
** Note that you'll probably have to modify the EXPORT define to match
** what your compiler wants to see in order to declare these functions
** as exported by the shared library (DLL).
**************************************************************************/

#ifdef Q_DECL_EXPORT
# define EXPORT Q_DECL_EXPORT
#endif

#ifdef _MSC_VER
# define EXPORT __declspec(dllexport)
#endif

#ifndef EXPORT
# define EXPORT
#endif

EXPORT lh_object *lh_create()
{
    return &image_plugin;
}

EXPORT void lh_destroy( lh_object *o )
{
    Q_UNUSED(o);
    if( image_class.obj.cb && image_class.obj.cb_id )
        image_class.obj.cb( image_class.obj.cb_id, lh_cb_destroy, 0 );
    return;
}

/* eof */
