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
    QObject( parent ),
    setup_smoothing_( *parent, "Smoothing", lh_setup::ui_slider, 3,1,10 ),
    sink_netin_( *parent, "NetIn", "/system/net/in" ),
    sink_netout_( *parent, "NetOut", "/system/net/out" )
{
    connect( &sink_netin_, SIGNAL(valueChanged(QVariant)), this, SLOT(netInChanged(QVariant)) );
    connect( &sink_netout_, SIGNAL(valueChanged(QVariant)), this, SLOT(netOutChanged(QVariant)) );
    return;
}

LH_QtNetwork::~LH_QtNetwork()
{
    return;
}

void LH_QtNetwork::netInChanged( const QVariant & v )
{
    netin_.enqueue( v.toLongLong() );
    while( netin_.size() >= samples() ) netin_.dequeue();
    parent()->requestRender();
}

void LH_QtNetwork::netOutChanged( const QVariant & v )
{
    netout_.enqueue( v.toLongLong() );
    while( netout_.size() >= samples() ) netout_.dequeue();
    parent()->requestRender();
}

long long LH_QtNetwork::inRate() const
{
    if( netin_.size() < 2 ) return 0;
    return ( netin_.first() + netin_.last() ) / 2;
}

long long LH_QtNetwork::outRate() const
{
    if( netout_.size() < 2 ) return 0;
    return ( netout_.first() + netout_.last() ) / 2;
}

int LH_QtNetwork::inPermille() const
{
    if( long long inmax = sink_netin_.maximum().toLongLong() )
        return inRate() * 1000 / inmax;
    return 0;
}

int LH_QtNetwork::outPermille() const
{
    if( long long outmax = sink_netout_.maximum().toLongLong() )
        return outRate() * 1000 / outmax;
    return 0;
}

int LH_QtNetwork::tpPermille() const
{
    if( long long max = (sink_netin_.maximum().toLongLong() + sink_netout_.maximum().toLongLong()) )
        return (inRate()+outRate()) * 1000 / max;
    return 0;
}
