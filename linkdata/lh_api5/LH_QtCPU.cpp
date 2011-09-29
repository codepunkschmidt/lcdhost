/**
  \file     LH_QtCPU.cpp
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

#include "LH_QtCPU.h"

LH_QtCPU::LH_QtCPU( LH_QtInstance *parent ) :
    QObject( parent ),
    setup_smoothing_( *parent, "Smoothing", lh_setup::ui_slider, 3,1,10 ),
    sink_coreload_( *parent, "Coreloads", "/system/cpu/coreload" )
{
    connect( &sink_coreload_, SIGNAL(valueChanged(QVariant)), this, SLOT(coreloadChanged(QVariant)) );
    return;
}

LH_QtCPU::~LH_QtCPU()
{
    load_.clear();
    return;
}

int LH_QtCPU::count()
{
    return sink_coreload_.value().toList().size();
}

void LH_QtCPU::coreloadChanged( const QVariant & v )
{
    if( v.type() == QVariant::List )
    {
        load_.enqueue( v.toList() );
        while( load_.size() >= samples() ) load_.dequeue();
        parent()->requestRender();
    }
}

int LH_QtCPU::coreload( int core )
{
    const QVariantList & from( load_.first() );
    const QVariantList & to( load_.last() );
    if( core < 0 || core >= from.size() || core >= to.size() ) return 0;
    return ( from.at(core).toInt() + to.at(core).toInt() ) / 2;
}

int LH_QtCPU::averageload()
{
    const QVariantList & from( load_.first() );
    const QVariantList & to( load_.last() );
    int retv = 0;
    for( int core = 0; core < from.size(); ++ core )
        retv += ( from.at(core).toInt() + to.at(core).toInt() ) / 2;
    retv /= from.size();
    return retv;
}

