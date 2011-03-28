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

#include "lh_plugin.h"

#ifndef EXPORT
# define EXPORT extern "C" Q_DECL_EXPORT
#endif

extern lh_systemstate *lcdhost_state();
extern void lh_callback( const void *obj, lh_callbackcode_t code, void *param );

class LH_QtInstance;

/**
  Automatic registration of LCDHost layout classes written using this
  framework. You can also manually register/deregister classes using
  the addClass() and removeClass() methods of LH_QtPlugin.
  */
typedef lh_class *(*lh_class_info_t)();
typedef LH_QtInstance *(*lh_class_factory_t)(const char *, const lh_class *);
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
  */
class LH_QtPlugin : public QObject
{
    Q_OBJECT
    static LH_QtPlugin *instance_;

public:
    LH_QtPlugin() { Q_ASSERT(instance_ == NULL); instance_ = this; }
    virtual ~LH_QtPlugin() { instance_ = NULL; }

    virtual const char *lh_name() { return NULL; }
    virtual const char *lh_shortdesc() { return NULL; }
    virtual const char *lh_author() { return NULL; }
    virtual const char *lh_homepage() { return NULL; }
    virtual const char *lh_longdesc() { return NULL; }
    virtual const lh_blob *lh_logo() { return NULL; }
    virtual const char *lh_load() { return NULL; }
    virtual void lh_unload() {}
    virtual int lh_polling() { return 0; }
    virtual int lh_notify(int, void *) { return 0; }
    virtual const lh_buildinfo *lh_version( int av ) { Q_UNUSED(av); return NULL; }

    static const lh_systemstate *state() { return lcdhost_state(); }
    static void callback( const void *obj, lh_callbackcode_t code, void *param ) { lh_callback( obj, code, param ); }
    static LH_QtPlugin *instance() { return instance_; }
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
    classname *_lh_##classname##_factory(const char *name, const lh_class *cls) { Q_UNUSED(cls); return new classname(name); } \
    lh_class *_lh_##classname##_info() { return classname::classInfo(); } \
    LH_QtClassLoader _lh_##classname##_loader( _lh_##classname##_info, reinterpret_cast<lh_class_factory_t>(_lh_##classname##_factory) );

#endif // LH_QTPLUGIN_H
