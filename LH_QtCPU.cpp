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
#include "../lh_plugin.h"

LH_QtCPU::LH_QtCPU( LH_QtInstance *parent )
{
    parent_ = parent;
    setup_smoothing_ = new LH_Qt_QSlider(parent,"Smoothing",3,1,10);
    load_.clear();
    return;
}

LH_QtCPU::~LH_QtCPU()
{
    while( !load_.isEmpty() )
        delete[] load_.dequeue();
    return;
}

int LH_QtCPU::count()
{
    return state()->cpu_count;
}

int LH_QtCPU::notify(int n, void *p)
{
    Q_UNUSED(p);

    if( n & LH_NOTE_CPU )
    {
        lh_cpudata *data;

        while( load_.size() >= samples() )
            delete[] load_.dequeue();

        data = new lh_cpudata[ state()->cpu_count ];
        if( data )
        {
            memcpy( data, state()->cpu_cores, sizeof(lh_cpudata) * state()->cpu_count );
            while( load_.size() >= samples() )
            {
                lh_cpudata *olddata = load_.dequeue();
                Q_ASSERT( olddata );
                if( olddata ) delete[] olddata;
            }
            load_.enqueue( data );
            parent_->requestRender();
        }
    }
    return LH_NOTE_CPU;
}

int LH_QtCPU::load( lh_cpudata *from, lh_cpudata *to )
{
    int retv;
    if( to->total <= from->total ) return 0;
    if( to->system < from->system ) return 0;
    if( to->user < from->user ) return 0;
    Q_ASSERT( from->total < to->total );
    retv = (((from->system - to->system)+(from->user - to->user)) * 10000) / (from->total - to->total);
    if( retv < 0 ) retv = 0;
    if( retv > 10000 ) retv = 10000;
    return retv;
}

int LH_QtCPU::coreload(int n)
{
    if( n<0 || n>=count() ) return 0;
    if( count() < 1 || load_.size() < 2 ) return 0;
    return load( & load_.first()[n], & load_.last()[n] );
}

int LH_QtCPU::averageload()
{
    int retv = 0;
    if( count() < 1 || load_.size() < 2 ) return 0;
    for( int i = 0; i<count(); i++ )
    {
        retv += load( & load_.first()[i], & load_.last()[i] );
    }
    retv /= count();
    Q_ASSERT( retv >= 0 );
    Q_ASSERT( retv <= 10000 );
    return retv;
}

