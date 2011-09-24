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

LH_QtNetwork::LH_QtNetwork( LH_QtInstance *parent ) :
    QObject(parent),
    link_net_in_(parent,
                 "NetIn",
                 lh::val(0,lh_link_sink),
                 lh::min(0,lh_link_sink),
                 lh::max(0,lh_link_sink),
                 lh::link("/system/network/in")
                 ),
    link_net_out_(parent,
                  "NetOut",
                  lh::val(0,lh_link_sink),
                  lh::min(0,lh_link_sink),
                  lh::max(0,lh_link_sink),
                  lh::link("/system/network/out")
                  ),
    setup_smoothing_(parent,
                     "Smoothing",
                     lh::val(3),
                     lh::min(1),
                     lh::max(10),
                     lh_ui_slider|lh_meta_autorender
                     )
{
    connect( &link_net_in_, SIGNAL(intChanged(int)), this, SLOT(addInRate(int)) );
    connect( &link_net_out_, SIGNAL(intChanged(int)), this, SLOT(addOutRate(int)) );
    inrate_.clear();
    outrate_.clear();
    return;
}

LH_QtNetwork::~LH_QtNetwork()
{
    return;
}

void LH_QtNetwork::addInRate(int n)
{
    while( inrate_.size() >= samples() ) inrate_.dequeue();
    inrate_.enqueue(n);
    parent()->requestRender();
}

void LH_QtNetwork::addOutRate(int n)
{
    while( outrate_.size() >= samples() ) outrate_.dequeue();
    outrate_.enqueue(n);
    parent()->requestRender();
}

#if 0
int LH_QtNetwork::notify(int n, void *p)
{
    Q_UNUSED(p);

    if( n & LH_NOTE_NET )
    {
        while( data_.size() >= samples() ) delete data_.dequeue();
        lh_netdata *data = new lh_netdata;
        if( data )
        {
            memcpy( data, & state()->net_data, sizeof(lh_netdata) );
            while( data_.size() >= samples() ) delete data_.dequeue();
            data_.enqueue( data );
            parent_->requestRender();
        }
    }
    return LH_NOTE_NET;
}
#endif

qint64 LH_QtNetwork::inRate() const
{
    qint64 retv = 0;
    if( !inrate_.isEmpty() )
    {
        foreach( qint64 n, inrate_ ) retv += n;
        retv /= inrate_.size();
    }
    return retv;
}

qint64 LH_QtNetwork::outRate() const
{
    qint64 retv = 0;
    if( !outrate_.isEmpty() )
    {
        foreach( qint64 n, outrate_ ) retv += n;
        retv /= outrate_.size();
    }
    return retv;
}

int LH_QtNetwork::inPermille() const
{
    if( !link_net_in_.maximum().isNull() )
        return inRate() * 1000 / link_net_in_.maximum().toLongLong();
    return 0;
}

int LH_QtNetwork::outPermille() const
{
    if( !link_net_out_.maximum().isNull() )
        return outRate() * 1000 / link_net_out_.maximum().toLongLong();
    return 0;
}

int LH_QtNetwork::tpPermille() const
{
    qint64 div = link_net_out_.maximum().toLongLong() + link_net_in_.maximum().toLongLong();
    if( div ) return (inRate() + outRate()) * 1000 / div;
    return 0;
}
