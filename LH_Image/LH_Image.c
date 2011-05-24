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
static lh_buildinfo bi = LH_STD_BUILDINFO;

void *lh_id;
lh_callback_t lh_callback;

/**************************************************************************
** Object Image
**************************************************************************/

/**
  This structure is what we'll be using as an 'instance' of ourselves.
  */
typedef struct _lh_image_s
{
    char filename[256];
    lh_blob *blob;
    const lh_systemstate *state;
    lh_setup_item setup_filename;
    lh_setup_item *setup_array[2];
} lh_image;

/**
  image_new() allocates a new instance of this class and returns it.
  */
static void * image_new( const lh_class *cls )
{
    lh_image *img;
    Q_UNUSED(cls);

    img = malloc( sizeof(lh_image) );
    if( img )
    {
        memset( img, 0, sizeof(img) );
        img->blob = NULL;
        img->state = 0;
        img->setup_filename.name = "Filename";
        img->setup_filename.help = NULL;
        img->setup_filename.type = lh_type_string_filename;
        img->setup_filename.param.size = sizeof(img->filename);
        img->setup_filename.data.s = img->filename;
        img->setup_filename.flags = 0;
        img->setup_array[0] = & img->setup_filename;
        img->setup_array[1] = NULL;
        return img;
    }

    return NULL;
}

static const char *image_init( void *obj, const char *name, const lh_systemstate *state )
{
    lh_image *img = obj;
    Q_UNUSED(name);
    img->state = state;
    return 0;
}


/**
  Since we store the setup array inside our struct, and do
  the setup when creating it, just return the array.

  This function is called right after the instance is created,
  and the returned data is expected to be valid until the instance
  is destroyed.
  */
static lh_setup_item ** image_setup_data(void *obj)
{
    lh_image *img = obj;
    return img->setup_array;
}

/**
  React to changes in setup immediately. We don't need to check which
  item is changed since we only have one. To illustrate self-unloading,
  if the filename equals 'unload', we request unloading.
  */
static void image_setup_change(void*obj,lh_setup_item *i)
{
    lh_image *img = obj;
    if( lh_callback )
    {
        if( !strcmp( i->data.s, "unload" ) )
        {
            strcpy( i->data.s, "was-unloaded" );
            lh_callback( lh_id, img, lh_cb_setup_refresh, i );
            lh_callback( lh_id, img, lh_cb_unload, "Filename was 'unload'." );
        }
        else
        {
            if( img->blob )
            {
                free( img->blob );
                img->blob = NULL;
            }
            lh_callback( lh_id, img, lh_cb_render, NULL );
        }
    }
    return;
}

/**
  Not using input.
  */
static void image_input(void*obj,lh_setup_item *i,int f,int v)
{
    Q_UNUSED(obj);
    Q_UNUSED(i);
    Q_UNUSED(f);
    Q_UNUSED(v);
    return;
}

/**
  We don't use polling, so we always return zero.
  */
static int image_polling(void*obj)
{
    Q_UNUSED(obj);
    return 0;
}

/**
  We don't use notifications, so we always return zero.
  */
static int image_notify(void*obj,int note,void *param)
{
    Q_UNUSED(obj);
    Q_UNUSED(note);
    Q_UNUSED(param);
    return LH_NOTE_NONE;
}

/**
  This might be a good place to check if the image file
  has been updated.
  */
static void image_prerender(void*obj)
{
    Q_UNUSED(obj);
    return;
}

/**
  We're so primitive, we can't even tell what size picture
  we'll be returning. No worries. Negative size means
  'I don't know', and LCDHost will get the size from the
  image data.
  */
static int image_width(void*obj,int h)
{
    Q_UNUSED(obj);
    Q_UNUSED(h);
    return -1;
}

static int image_height(void*obj,int w)
{
    Q_UNUSED(obj);
    Q_UNUSED(w);
    return -1;
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
static const lh_blob * image_render_blob(void*obj,int w,int h)
{
    char *fullname;
    size_t n;
    lh_image *img = obj;

    Q_UNUSED(w);
    Q_UNUSED(h);

    if( img->blob == NULL )
    {
        if( !is_absolute(img->filename) && img->state->dir_layout )
        {
            n = strlen( img->state->dir_layout ) + strlen( img->filename ) + 1;
            fullname = malloc( n );
            if( fullname )
            {
                strcpy( fullname, img->state->dir_layout );
                strcat( fullname, img->filename );
                /* convert it from UTF-8 to local 8 bit */
                lh_callback( lh_id, img, lh_cb_utf8_to_local8bit, fullname );
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
  Free resources.
  */
static void image_delete(void*obj)
{
    lh_image *img = obj;
    if( img->blob ) free( img->blob );
    memset( img, 0, sizeof(lh_image) );
    free( img );
    return;
}

/**************************************************************************
** LCDHost data
**************************************************************************/

static lh_class class_image =
{
    sizeof(lh_class),
    "Static",
    "StaticImage",
    "Image",
    -1,-1,
    {
        sizeof(lh_instance_calltable),
        {
            sizeof(lh_object_calltable),
            image_init,
            image_setup_data,
            0, /* not using setup_resize, the filename isn't dynamically allocated */
            image_setup_change,
            image_input,
            image_polling,
            image_notify,
            0 /* not using term */
        },
        image_new,
        image_prerender,
        image_width,
        image_height,
        image_render_blob,
        0, /* not using render_qimage */
        image_delete
    }
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

EXPORT void *lh_create( lh_callback_t callback, void *id )
{
    lh_id = id;
    lh_callback = callback;
    return &bi;
}

EXPORT void lh_destroy( void *ref )
{
    Q_UNUSED(ref);
    return;
}

EXPORT const char * lh_name(void *ref)
{
    Q_UNUSED(ref);
    return "Image";
}

EXPORT const char * lh_shortdesc(void*ref)
{
    Q_UNUSED(ref);
    return "Display images files";
}

#ifndef STRINGIZE
# define STRINGIZE_(x) #x
# define STRINGIZE(x) STRINGIZE_(x)
#endif

EXPORT const char * lh_author(void*ref)
{
    Q_UNUSED(ref);
    return "Johan \"SirReal\" Lindh";
}

EXPORT const char * lh_homepage(void*ref)
{
    Q_UNUSED(ref);
    return "<a href=\"http://www.linkdata.se/software/lcdhost\">Link Data Stockholm</a>";
}

EXPORT const char * lh_longdesc(void*ref)
{
    Q_UNUSED(ref);
    return
    	"Show the contents of image files on screen. This plugin comes with "
    	"source code illustrating how a plugin written in C might look.";
}

EXPORT const lh_blob *lh_logo(void*ref)
{
    Q_UNUSED(ref);
    return logo_blob;
}

EXPORT const lh_class ** lh_class_list(void*ref)
{
    Q_UNUSED(ref);
    static const lh_class *lh_classes[] =
    {
        & class_image,
        NULL
    };
    return lh_classes;
}

EXPORT const char * lh_load(void *ref )
{
    Q_UNUSED(ref);
    return NULL; /* indicate success */
}

EXPORT void lh_unload(void*ref)
{
    Q_UNUSED(ref);
    return;
}

/* eof */
