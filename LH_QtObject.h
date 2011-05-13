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

/**
  Base class for Qt-based LCDHost plugin objects,
  such as plugins, layout class instances or devices.
  */
class LH_QtObject : public QObject
{
    Q_OBJECT

    static lh_callback_t cb_;
    static void *cb_id_;
    static LH_QtPlugin *plugin_;

    QVector<lh_setup_item*> setup_item_vector_;

public:
    LH_QtObject( QObject *parent = 0 ) : QObject(parent) {}
    virtual ~LH_QtObject() {}

    void callback( lh_callbackcode_t code, void *param ) const { if( cb_ ) cb_( cb_id_, this, code, param ); }

    virtual lh_setup_item **setup_data();
    virtual void setup_resize( lh_setup_item *item, size_t needed );
    virtual void setup_change( lh_setup_item *item );
    virtual void setup_input( lh_setup_item *item, int flags, int value );
    virtual int notify( int code, void *param );
    virtual int polling();

    static void set_lh_callback( lh_callback_t cb, void *cb_id  ) { cb_ = cb; cb_id_ = cb_id; }
    static void lh_callback(const void *obj, lh_callbackcode_t code, void *param) { if( cb_ ) cb_( cb_id_, obj, code, param ); }

    static void set_plugin( LH_QtPlugin *p ) { plugin_ = p; }
    static LH_QtPlugin *plugin() { return plugin_; }

    static void build_calltable( lh_object_calltable *ct );

public slots:
    void requestRebuild() const { callback( lh_cb_setup_rebuild, NULL ); } // call after adding or removing setup items!
    void requestRender() const { callback( lh_cb_render, NULL ); }
    void requestPolling() const { callback( lh_cb_polling, NULL ); }
};

#endif // LH_QTOBJECT_H
