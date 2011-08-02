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

LH_QtCPU::LH_QtCPU( LH_QtInstance *parent ) : QObject( parent )
{
    link_coreloads_ = new LH_Qt_array_int( parent, "Core loads", 0, LH_FLAG_HIDDEN|LH_FLAG_NOSAVE );
    link_coreloads_->setLink("/system/cpu/coreloads");
    setup_smoothing_ = new LH_Qt_QSlider( parent,"Smoothing",3,1,10);
    load_.clear();
    return;
}

LH_QtCPU::~LH_QtCPU()
{
    while( !load_.isEmpty() ) delete[] load_.takeFirst();
    return;
}

int LH_QtCPU::count()
{
    return link_coreloads_->size();
}

int LH_QtCPU::coreload(int n)
{
    if( n<0 || n>=link_coreloads_->size() ) return 0;
    return link_coreloads_->at(n);
}

int LH_QtCPU::averageload()
{
    int retv = 0;
    if( count() < 1 ) return 0;
    for( int i = 0; i<count(); i++ )
        retv += link_coreloads_->at(i);
    retv /= count();
    Q_ASSERT( retv >= 0 );
    Q_ASSERT( retv <= 10000 );
    return retv;
}

