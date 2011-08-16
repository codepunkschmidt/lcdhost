/**
  \file     LH_QtInstance.h
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

#ifndef LH_QTINSTANCE_H
#define LH_QTINSTANCE_H

#include <QtGlobal>
#include <QImage>

#include "LH_QtObject.h"

typedef lh_layout_item *(*obj_layout_item_create_t)(lh_layout_class*,lh_callback_t,void*);
typedef void (*obj_layout_item_destroy_t)(lh_layout_class*,lh_layout_item *);
typedef struct lh_class_t
{
    int size; /* sizeof(lh_class) */
    const char *path;
    const char *ident;
    const char *title;
    int width;
    int height;
} lh_class;

/**
  Base class for LCDHost layout classes and layout items using Qt.
  For normal use, the macro LH_PLUGIN_CLASS(classname) will export
  the class from the implementation file (not from the header file!).

  See LH_Lua for an example on how to handle dynamically creating and
  removing classes.
  */
class LH_QtInstance : public LH_QtObject
{
    Q_OBJECT

protected:
    lh_layout_item li_;
    QImage *image_;

public:
    LH_QtInstance( lh_callback_t cb = 0 , void* cb_id = 0 );
    ~LH_QtInstance();

    bool isMonochrome() const { return li_.layout.depth == 1; }

    lh_layout_item *item() { return &li_; }
    QImage *image() const { return image_; }
    QImage *initImage(int w, int h);

    virtual void prerender() {}
    virtual int width( int ) { return -1; }
    virtual int height( int ) { return -1; }
    virtual lh_blob *render_blob( int, int ) { return NULL; }
    virtual QImage *render_qimage( int, int ) { return NULL; }

    virtual QString dir_layout() const
    {
        Q_ASSERT( li_.layout.dir );
        return QString::fromUtf8( li_.layout.dir );
    }

    /* reimplement this static member in your own classes */
    static const lh_class *classInfo()
    {
        Q_ASSERT(!"You must reimplement classInfo()");
        return 0;
    }
};

/**
  Automatic registration of LCDHost layout classes written using this
  framework. You can also manually register/deregister classes using
  the callback.
  */
class LH_QtLayoutClassLoader
{
public:
    static LH_QtLayoutClassLoader *first_;
    LH_QtLayoutClassLoader *next_;
    lh_layout_class cls_;
    LH_QtLayoutClassLoader(
        const lh_class *classinfo,
        obj_layout_item_create_t fc,
        obj_layout_item_destroy_t fd );
};

/**
  This macro creates the required functions and object to allow
  automatic registration of layout classes. Note that using this
  macro requires a static classInfo() method that returns a
  statically allocated lh_class structure pointer. This version
  is for classes that use default constructors without the callback
  information immediately available.
  */
#define LH_PLUGIN_CLASS(classname)  \
    lh_layout_item *_lh_##classname##_layout_item_create(lh_layout_class*,lh_callback_t,void*) \
    { return (new classname())->item(); } \
    void _lh_##classname##_layout_item_destroy(lh_layout_class*,lh_layout_item*li) \
    { delete reinterpret_cast<classname*>(li->obj.ref); } \
    LH_QtLayoutClassLoader _lh_##classname##_loader ( \
        classname::classInfo(), \
        _lh_##classname##_layout_item_create, \
        _lh_##classname##_layout_item_destroy );

/**
  This macro is for plugin layout classes that use the callback
  information constructor.
  */
#define LH_PLUGIN_CLASS_CB(classname)  \
    lh_layout_item *_lh_##classname##_layout_item_create(lh_layout_class*,lh_callback_t cb,void*cb_id) \
    { return (new classname(cb,cb_id))->item(); } \
    void _lh_##classname##_layout_item_destroy(lh_layout_class*,lh_layout_item*li) \
    { delete reinterpret_cast<classname*>(li->obj.ref); } \
    LH_QtLayoutClassLoader _lh_##classname##_loader ( \
        classname::classInfo(), \
        _lh_##classname##_layout_item_create, \
        _lh_##classname##_layout_item_destroy );

#endif // LH_QTINSTANCE_H
