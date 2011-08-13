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

protected:
    QByteArray title_array_;

public:
#ifndef QT_NO_DEBUG
    bool clean_init_;
#endif
    LH_QtObject( lh_object *p, QObject *parent = 0);
    LH_QtObject( lh_object *p, const char *ident, QObject *parent = 0);
    ~LH_QtObject();

    bool isValid() const { return p_obj_ && (p_obj_->size == sizeof(lh_object)) && p_obj_->cb && p_obj_->cb_id; }
    lh_object *obj() const { return p_obj_; }
    const char *ident() const { return p_obj_->ident; }

    void setTitle(const char *s = 0);
    void setTitle( const QString& );
    QString title() const { return QString::fromUtf8(p_obj_->title); }

    void callback( lh_callbackcode_t code, void *param = 0 ) const
    {
        if( p_obj_->cb && p_obj_->cb_id )
            p_obj_->cb( p_obj_->cb_id, code, param );
    }

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
    virtual int notify( int, void * ) { return 0; }

    // Deprecated. Move userTerm() code to destructor.
    virtual int userTerm() { Q_ASSERT(0); return 0; }

    // Convenience wrappers
    void show() const { int b = 0; callback( lh_cb_sethidden, (void*)&b ); }
    void hide() const { int b = 1; callback( lh_cb_sethidden, (void*)&b ); }
    void setVisible( bool b ) const { int notb = !b; callback( lh_cb_sethidden, (void*)&notb ); }

    virtual QString dir_layout() const;
    QString dir_binaries() const;
    QString dir_plugins() const;
    QString dir_data() const;

public slots:
    void requestRender() const { callback( lh_cb_render, NULL ); }
    void requestPolling() const { callback( lh_cb_polling, NULL ); }

signals:
    void initialized(); // emitted once userInit() completes
};

#endif // LH_QTOBJECT_H
