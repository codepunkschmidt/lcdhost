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

public:
    LH_QtInstance( LH_QtObject *parent = 0 ) : LH_QtObject(parent), image_(0) {}

    virtual void term();

    QImage *image() const { return image_; }
    QImage *initImage(int w, int h);

    virtual void prerender() {}
    virtual int width( int ) { return -1; }
    virtual int height( int ) { return -1; }
    virtual lh_blob *render_blob( int, int ) { return NULL; }
    virtual QImage *render_qimage( int, int ) { return NULL; }

    static void build_instance_calltable( lh_instance_calltable *ct, lh_class_factory_t cf );
    static const lh_class **auto_class_list();

    /** You MUST reimplement this in your classes if you use the class loader and macros below */
    static lh_class *classInfo() { Q_ASSERT(!"classInfo() not reimplemented"); return NULL; }
};

/**
  This macro creates the required functions and object to allow
  automatic registration of layout classes. Note that using this
  macro requires a static classInfo() method that returns a
  statically allocated lh_class structure pointer.
  */
#define LH_PLUGIN_CLASS(classname)  \
    classname *_lh_##classname##_factory(const lh_class *) { return new classname; } \
    lh_class *_lh_##classname##_info() { return classname::classInfo(); } \
    LH_QtClassLoader _lh_##classname##_loader( _lh_##classname##_info, reinterpret_cast<lh_class_factory_t>(_lh_##classname##_factory) );

#endif // LH_QTINSTANCE_H
