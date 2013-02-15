/**
  \file     LH_QtPlugin.h
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

#ifndef LH_QTPLUGIN_H
#define LH_QTPLUGIN_H

#include <QtGlobal>
#include <QObject>
#include <QStringList>

#include "LH_QtObject.h"

#ifndef EXPORT
# define EXPORT extern "C" Q_DECL_EXPORT
#endif


/**
  Automatic registration of LCDHost layout classes written using this
  framework. You can also manually register/deregister classes using
  the lh_add_class() and lh_remove_class() functions in LH_QtPlugin.
  */
typedef lh_class *(*lh_class_info_t)();
typedef void *(*lh_class_factory_t)(const lh_class *);
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
  Base class for Qt-based LCDHost shared libraries.
  As of alpha 17, the LH_QtPlugin object is no longer a global
  C++ object, rather it is created dynamically in lh_create()
  and destroyed in lh_destroy(). These two functions are
  defined when you use the LH_PLUGIN(classname) macro.
  */
class LH_QtPlugin : public LH_QtObject
{
    Q_OBJECT
    lh_object_calltable objtable_;

public:
    LH_QtPlugin();
    virtual ~LH_QtPlugin() {}
    virtual const lh_class **class_list();
    const lh_object_calltable * objtable() { return & objtable_; }
};

/**
  This macro creates the required exported functions
  for your LH_QtPlugin descendant.
  */
#define LH_PLUGIN(classname) \
    EXPORT void *lh_create() \
    { \
        classname *the_plugin = 0; \
        the_plugin = new classname; \
        LH_QtObject::set_plugin( the_plugin ); \
        return reinterpret_cast<void*>(the_plugin); \
    } \
    EXPORT void lh_destroy( void *ref ) { delete reinterpret_cast<classname*>(ref); }

extern void lh_add_class( lh_class *p, lh_class_factory_t f );
extern void lh_remove_class( lh_class *p );

#endif // LH_QTPLUGIN_H
