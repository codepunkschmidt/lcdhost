/**
  \file     LH_QtInputDevice.cpp
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

#include "LH_QtInputDevice.h"
#include "LH_QtPlugin.h"

#define RECAST(o) reinterpret_cast<LH_QtInputDevice*>(o)

static const char* obj_open(lh_input_device*obj)
{
    return RECAST(obj->obj.ref)->open();
}

static const char* obj_input_name(lh_input_device*obj,int item)
{
    return RECAST(obj->obj.ref)->input_name(item);
}

static void obj_emulate(lh_input_device*obj,int item,int flags,int value )
{
    return RECAST(obj->obj.ref)->emulate(item,flags,value);
}

static const char* obj_close(lh_input_device*obj)
{
    return RECAST(obj->obj.ref)->close();
}

LH_QtInputDevice::LH_QtInputDevice( const char *ident, int flags, QObject *parent ) :
    LH_QtObject( &lh_dev_.obj, ident, parent )
{
    lh_dev_.size = sizeof(lh_input_device);
    lh_dev_.flags = flags;
    lh_dev_.obj_open = obj_open;
    lh_dev_.obj_input_name = obj_input_name;
    lh_dev_.obj_emulate = obj_emulate;
    lh_dev_.obj_close = obj_close;

    LH_QtPlugin::instance()->callback( lh_cb_input_create, &lh_dev_ );
    return;
}

LH_QtInputDevice::~LH_QtInputDevice()
{
    callback( lh_cb_destroy );
    memset( &lh_dev_, 0, sizeof(lh_dev_) );
    return;
}
