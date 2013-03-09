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
#include <QVector>
#include "lh_plugin.h"

class LH_QtPlugin;

extern "C" Q_DECL_EXPORT LH_QtPlugin *lh_plugin();

/**
  Base class for Qt-based LCDHost plugin objects,
  such as plugins, layout class instances or devices.
  */
class LH_QtObject : public QObject
{
    Q_OBJECT

    // static LH_QtPlugin *plugin_;

    lh_callback_t cb_;
    int cb_id_;
    const lh_systemstate *state_;
    QVector<lh_setup_item*> setup_item_vector_;

public:
    // static void set_plugin( LH_QtPlugin *p ) { plugin_ = p; }
    // static LH_QtPlugin *plugin() { return lh_plugin(); }
    static void build_object_calltable( lh_object_calltable *ct );

    LH_QtObject( LH_QtObject *parent = 0);
    virtual ~LH_QtObject() {}

    LH_QtObject *parent() const { return static_cast<LH_QtObject *>( QObject::parent() ); }

    void callback( lh_callbackcode_t code, void *param ) const;

    // These implement the lh_object_callback functions. If you reimplement them, make
    // sure to call the base class copy of them and preserve or modify the return value
    // (if any) to suit.
    virtual int polling();
    virtual int notify( int code, void *param );

    virtual const char *init( lh_callback_t cb, int cb_id, const char *name, const lh_systemstate* state );
    virtual lh_setup_item **setup_data();
    virtual void setup_resize( lh_setup_item *item, size_t needed );
    virtual void setup_change( lh_setup_item *item );
    virtual void setup_input( lh_setup_item *item, int flags, int value );
    virtual const lh_class **class_list();
    virtual void term();

    // You can use these two instead of init() and term(), that way you won't need to
    // pass on the parameters that init() takes to the ancestor. init() will call
    // userInit() when it's done and term() will call userTerm() before it does
    // it's work.
    virtual const char *userInit() { return 0; }
    virtual void userTerm() { return; }

    // Convenience wrappers
    void show() const { int b = 0; callback( lh_cb_sethidden, (void*)&b ); }
    void hide() const { int b = 1; callback( lh_cb_sethidden, (void*)&b ); }
    void setVisible( bool b ) const { int notb = !b; callback( lh_cb_sethidden, (void*)&notb ); }

    const lh_systemstate* state() const { return state_; }


signals:
    void initialized();

public slots:
    void requestRebuild() const { callback( lh_cb_setup_rebuild, NULL ); } // call after adding or removing setup items!
    void requestRender() const { callback( lh_cb_render, NULL ); }
    void requestPolling() const { callback( lh_cb_polling, NULL ); }
};

#endif // LH_QTOBJECT_H
