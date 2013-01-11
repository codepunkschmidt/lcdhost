/**
  \file     LH_QtNetwork.cpp
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

#include "LH_QtNetwork.h"

LH_QtNetwork::LH_QtNetwork( LH_QtObject *parent )
{
    parent_ = parent;
    setup_smoothing_ = new LH_Qt_QSlider(parent,"Smoothing",3,1,10,LH_FLAG_AUTORENDER);
    data_.clear();
    return;
}

LH_QtNetwork::~LH_QtNetwork()
{
    while( !data_.isEmpty() )
        delete data_.takeFirst();
    return;
}

int LH_QtNetwork::notify(int n, void *p)
{
    Q_UNUSED(p);
    Q_ASSERT( parent_ != NULL );

    if( n & LH_NOTE_NET )
    {
        while( data_.size() >= samples() ) delete data_.dequeue();
        lh_netdata *data = new lh_netdata;
        if( data )
        {
            memcpy( data, & state()->net_data, sizeof(lh_netdata) );
            while( data_.size() >= samples() ) delete data_.dequeue();
            data_.enqueue( data );
            if(!parent_->inherits(LH_QtPlugin::staticMetaObject.className()))
                parent_->requestRender();
        }
    }
    return LH_NOTE_NET;
}

qint64 LH_QtNetwork::inRate() const
{
    if( data_.size() < 2 ) return 0;
    if( data_.last()->device != data_.first()->device ) return 0;
    if( data_.last()->in < data_.first()->in ) return 0;
    qint64 timedelta = data_.last()->when - data_.first()->when;
    if( timedelta > 0 ) return (data_.last()->in - data_.first()->in) * Q_INT64_C(1000) / timedelta;
    return 0;
}

qint64 LH_QtNetwork::outRate() const
{
    if( data_.size() < 2 ) return 0;
    if( data_.last()->device != data_.first()->device ) return 0;
    if( data_.last()->out < data_.first()->out ) return 0;
    qint64 timedelta = data_.last()->when - data_.first()->when;
    if( timedelta > 0 ) return (data_.last()->out - data_.first()->out) * Q_INT64_C(1000) / timedelta;
    return 0;
}

int LH_QtNetwork::inPermille() const
{
    if( state()->net_max_in ) return inRate() * 1000 / state()->net_max_in;
    return 0;
}

int LH_QtNetwork::outPermille() const
{
    if( state()->net_max_out ) return outRate() * 1000 / state()->net_max_out;
    return 0;
}

int LH_QtNetwork::tpPermille() const
{
    qint64 div = state()->net_max_in + state()->net_max_out;
    if( div ) return (inRate() + outRate()) * 1000 / div;
    return 0;
}
