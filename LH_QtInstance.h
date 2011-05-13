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

#include "LH_QtPlugin.h"
#include "LH_QtObject.h"
#include "LH_QtPlugin.h"

#ifndef EXPORT
# define EXPORT extern "C" Q_DECL_EXPORT
#endif

/**
  Automatic registration of LCDHost layout classes written using this
  framework. You can also manually register/deregister classes using
  the addClass() and removeClass() methods of LH_QtPlugin.
  */
typedef lh_class *(*lh_class_info_t)();
typedef void *(*lh_class_factory_t)();
class LH_QtClassLoader
{
public:
    static LH_QtClassLoader *first_;
    LH_QtClassLoader *next_;
    lh_class_info_t info_;
    lh_class_factory_t factory_;
    LH_QtClassLoader( lh_class_info_t info, lh_class_factory_t factory ) : info_(info), factory_(factory)
    {
        next_ = first_;
        first_ = this;
    }
};

/*
 Support class, keeps info for classes added with lh_add_class()
 (as opposed to automatically added using LH_PLUGIN_CLASS)
 */
class lh_layout_class
{
    lh_class *info_;
    lh_class_factory_t factory_;
public:
    lh_layout_class(lh_class *p,lh_class_factory_t f) : info_(p), factory_(f) {}
    lh_class *info() const { return info_; }
    lh_class_factory_t factory() const { return factory_; }
};

/**
  Base class for LCDHost plugin classes using Qt. For normal use, the macro
  LH_PLUGIN_CLASS(classname) will export the class from the implementation
  file (not from the header file!).

  See LH_Lua for an example on how to handle dynamically creating and
  removing classes.
  */
class LH_QtInstance : public LH_QtObject
{
    Q_OBJECT

protected:
    QImage *image_;
    const lh_systemstate *state_;

public:
    LH_QtInstance( QObject *parent = 0 ) : LH_QtObject(parent), image_(0), state_(0) {}

    virtual const char *init( const lh_systemstate *state, const char *name, const lh_class *cls );
    virtual void term();

    QImage *image() const { return image_; }
    QImage *initImage(int w, int h);
    const lh_systemstate *state() const { return state_; }

    virtual int polling() { return 0; }
    virtual int notify( int, void* ) { return 0; }
    virtual void prerender() {}
    virtual int width( int ) { return -1; }
    virtual int height( int ) { return -1; }
    virtual lh_blob *render_blob( int, int ) { return NULL; }
    virtual QImage *render_qimage( int, int ) { return NULL; }

    static void build_calltable( lh_instance_calltable *ct, lh_class_factory_t cf );

    /** You MUST reimplement this in your classes if you use the class loader and macros below */
    static lh_class *classInfo() { Q_ASSERT(!"classInfo() not reimplemented"); return NULL; }
};

extern void lh_add_class( lh_class *p, lh_class_factory_t f );
extern void lh_remove_class( lh_class *p );

/**
  This macro creates the required functions and object to allow
  automatic registration of layout classes. Note that using this
  macro requires a static classInfo() method that returns a
  statically allocated lh_class structure pointer.
  */
#define LH_PLUGIN_CLASS(classname)  \
    classname *_lh_##classname##_factory() { return new classname; } \
    lh_class *_lh_##classname##_info() { return classname::classInfo(); } \
    LH_QtClassLoader _lh_##classname##_loader( _lh_##classname##_info, reinterpret_cast<lh_class_factory_t>(_lh_##classname##_factory) );

#endif // LH_QTINSTANCE_H
