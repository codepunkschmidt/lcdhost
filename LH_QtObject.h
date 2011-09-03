/**
  \file     LH_QtObject.h
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

#ifndef LH_QTOBJECT_H
#define LH_QTOBJECT_H

#include <QObject>
#include "lh_plugin.h"

/**
  Base class for Qt-based LCDHost plugin objects,
  such as plugins, layout class instances or devices.
  */
class LH_QtObject : public QObject
{
    Q_OBJECT
    lh_object *p_obj_;
    bool clean_init_;
    QByteArray title_array_;

public:
    LH_QtObject( lh_object *p, const char *ident = 0, QObject *parent = 0 );
    ~LH_QtObject();

    bool isValid() const { return p_obj_ && (p_obj_->size == sizeof(lh_object)) && p_obj_->cb && p_obj_->cb_id; }
    const char *ident() const { return p_obj_->ident; }
    const char *title() const { return p_obj_->title; }

    void callback( lh_callbackcode_t code, void *param = 0 ) const;

    // This gets called by LCDHost when the corresponding UI elements
    // have been created, layout information is available (if this
    // object is a layout item). This function calls userInit().
    const char *init();

    // Not all callbacks will work in all constructors.
    // You should leave the constructor 'clean', and do your
    // initialization in userInit().
    // Make sure to call the base class userInit() first thing.
    virtual const char *userInit();

    // These implement the lh_object functions. If you reimplement them, make sure
    // to call the base class copy of them and preserve or modify the return value.
    virtual int polling() { return 0; }
    virtual int notify( int, void * );

    virtual QString dir_layout() const;
    QString dir_binaries() const;
    QString dir_plugins() const;
    QString dir_data() const;

    static const char *callbackName( lh_callbackcode code );

public slots:
    void setTitle( const char * );
    void setTitle( QString );
    void requestRender() const { callback( lh_cb_render ); }
    void requestPolling() const { callback( lh_cb_polling ); }

signals:
    void titleChanged( const char * );
    void initialized(); // emitted once userInit() completes OK for this and all child objects
};

/**
  Support class for automatic registration of static LCDHost objects.
  */
typedef void (*lh_load_function)(LH_QtObject *);
class LH_QtLoader
{
    static LH_QtLoader *first_;
    LH_QtLoader *next_;
    lh_load_function func_;
public:
    LH_QtLoader( lh_load_function f ) : next_(first_), func_(f) { first_ = this; }
    static void load( LH_QtObject *parent );
};

#endif // LH_QTOBJECT_H
