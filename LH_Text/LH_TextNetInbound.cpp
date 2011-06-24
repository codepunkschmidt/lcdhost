/**
  \file     LH_TextNetInbound.cpp
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

#include <QDebug>
#include "LH_TextNumber.h"

class LH_TextNetInbound : public LH_TextNumber
{
public:
    LH_TextNetInbound(): LH_TextNumber()
    {
        setup_bits_->setFlag( LH_FLAG_HIDDEN, false );
    }

    static lh_class *classInfo()
    {
        static lh_class classInfo =
        {
            sizeof(lh_class),
            "System/Network/Inbound",
            "SystemNetworkInboundText",
            "Inbound Bandwidth Usage (Text)",
            20,10,
            lh_object_calltable_NULL,
            lh_instance_calltable_NULL
        };
        return &classInfo;
    }

    void prerender()
    {
        LH_TextNumber::prerender();
        if( showUnits() )
        {
            if( setup_bits_->value() ) setText( text().append( "bit/s" ) );
            else setText( text().append( "B/s" ) );
        }
    }

    int notify(int code, void *param)
    {
        if( !code || code&LH_NOTE_NET )
        {
            bool needrender = setValue( state()->net_cur_in ) | setMax( state()->net_max_in ); // bitwise OR - we need both to execute!
            if( needrender ) callback(lh_cb_render,NULL);
        }
        return LH_TextNumber::notify(code,param) | LH_NOTE_SECOND;
    }

};

LH_PLUGIN_CLASS(LH_TextNetInbound)
