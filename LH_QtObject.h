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

#include "LH_QtPlugin.h"

/**
  Base class for Qt-based LCDHost shared library created objects,
  such as class instances or devices.
  */
class LH_QtObject : public QObject
{
    Q_OBJECT
    QVector<lh_setup_item*> setup_item_vector_;

public:
    LH_QtObject( const char *name, QObject *parent = 0 ) : QObject(parent) // , LH_QtPlugin *parent = 0 ) : QObject(parent)
    {
        if( name ) setObjectName( QString::fromUtf8(name) );
    }

    virtual ~LH_QtObject() {}

    // LH_QtPlugin *parent() const { return static_cast<LH_QtPlugin *>(QObject::parent()); }

    void callback( lh_callbackcode_t code, void *param ) const { LH_QtPlugin::callback( this, code, param ); }
    const lh_systemstate *state() const { return LH_QtPlugin::state(); }

    virtual lh_setup_item **setup_data();
    virtual void setup_resize( lh_setup_item *item, size_t needed );
    virtual void setup_change( lh_setup_item *item );
    virtual void setup_input( lh_setup_item *item, int flags, int value );

public slots:
    void requestRebuild() const { callback( lh_cb_setup_rebuild, NULL ); } // call after adding or removing setup items!
    void requestRender() const { callback( lh_cb_render, NULL ); }
    void requestPolling() const { callback( lh_cb_polling, NULL ); }
};

#endif // LH_QTOBJECT_H
