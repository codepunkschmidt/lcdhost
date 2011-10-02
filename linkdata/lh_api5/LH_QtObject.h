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

#include "lh_api6/lh_api6.h"
#include "lh_api5/lh_plugin.h"

class LH_QtPlugin;

/**
  Base class for Qt-based LCDHost plugin objects,
  such as plugins, layout class instances or devices.
  */
class LH_QtObject : public lh_object
{
    Q_OBJECT

    static LH_QtPlugin *plugin_;

    void show() const { }
    void hide() const { }
    void setVisible( bool ) const { }

    void requestRebuild() const {}

public:
    LH_QtObject( LH_QtObject *parent = 0);
    virtual ~LH_QtObject() {}

    LH_QtObject *parent() const { return static_cast<LH_QtObject *>( QObject::parent() ); }

    // These implement the lh_object_callback functions. If you reimplement them, make
    // sure to call the base class copy of them and preserve or modify the return value
    // (if any) to suit.
    virtual int polling();
    virtual int notify( int code, void *param );

    // You can use these two instead of init() and term(), that way you won't need to
    // pass on the parameters that init() takes to the ancestor. init() will call
    // userInit() when it's done and term() will call userTerm() before it does
    // it's work.
    bool init();
    virtual const char *userInit() { return 0; }
    virtual void userTerm() { return; }

    static void set_plugin( LH_QtPlugin *p ) { plugin_ = p; }
    static LH_QtPlugin *plugin() { return plugin_; }

signals:
    void initialized();

public slots:
    void requestPolling() const { lh_request_polling(); }
    void requestRender() const { lh_request_render(); }
};

#endif // LH_QTOBJECT_H
