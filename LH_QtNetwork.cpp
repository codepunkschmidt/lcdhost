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

LH_QtNetwork::LH_QtNetwork( LH_QtInstance *parent ) : QObject(parent)
{
    setup_smoothing_ = new LH_Qt_QSlider(parent,"Smoothing",3,1,10,LH_FLAG_AUTORENDER);
    link_net_in_rate_ = new LH_Qt_int(parent,"NetInRate",0,LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);
    link_net_in_rate_->setLink("/system/net/in/rate");
    connect(link_net_in_rate_, SIGNAL(change(qint64)), this, SLOT(addInRate(qint64)) );
    link_net_in_max_ = new LH_Qt_int(parent,"NetInMax",0,LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);
    link_net_in_max_->setLink("/system/net/in/max");
    link_net_out_rate_ = new LH_Qt_int(parent,"NetOutRate",0,LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);
    link_net_out_rate_->setLink("/system/net/out/rate");
    connect(link_net_out_rate_, SIGNAL(change(qint64)), this, SLOT(addOutRate(qint64)) );
    link_net_out_max_ = new LH_Qt_int(parent,"NetOutMax",0,LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);
    link_net_out_max_->setLink("/system/net/out/max");
    inrate_.clear();
    outrate_.clear();
    return;
}

LH_QtNetwork::~LH_QtNetwork()
{
    return;
}

void LH_QtNetwork::addInRate(qint64 n)
{
    while( inrate_.size() >= samples() ) inrate_.dequeue();
    inrate_.enqueue(n);
    parent()->requestRender();
}

void LH_QtNetwork::addOutRate(qint64 n)
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
    if( link_net_in_max_->value() ) return inRate() * 1000 / link_net_in_max_->value();
    return 0;
}

int LH_QtNetwork::outPermille() const
{
    if( link_net_out_max_->value() ) return outRate() * 1000 / link_net_out_max_->value();
    return 0;
}

int LH_QtNetwork::tpPermille() const
{
    qint64 div = link_net_out_max_->value() + link_net_in_max_->value();
    if( div ) return (inRate() + outRate()) * 1000 / div;
    return 0;
}
