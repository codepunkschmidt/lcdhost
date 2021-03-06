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

#include <QtDebug>
#include "LH_QtCPU.h"

LH_QtCPU::LH_QtCPU( LH_QtInstance *parent ) :
    QObject( parent ),
    link_coreloads_( parent, "Core loads", lh::val(QVariantList(),lh_link_sink), lh::link("/system/cpu/coreloads") ),
    setup_smoothing_( parent, "Smoothing", lh::val(3), lh::min(1), lh::max(10), lh_meta_default|lh_ui_slider )
{
    connect( &link_coreloads_, SIGNAL(valueChanged()), this, SLOT(gotData()) );
    load_.clear();
    return;
}

LH_QtCPU::~LH_QtCPU()
{
    while( !load_.isEmpty() ) delete[] load_.takeFirst();
    return;
}

void LH_QtCPU::gotData()
{
    while( load_.size() > setup_smoothing_.value().toInt() ) delete[] load_.takeFirst();
    int *newloads = new int[count()];
    for( int i = 0; i<count(); i++ )
    {
        newloads[i] = link_coreloads_.value().toList().at(i).toInt();
        Q_ASSERT( newloads[i] >= link_coreloads_.minimum().toInt() );
        Q_ASSERT( newloads[i] <= link_coreloads_.maximum().toInt() );
    }
    load_.append(newloads);
    parent()->requestRender();
}

int LH_QtCPU::coreload(int n)
{
    int retv = 0;
    if( load_.size() < 1 ) return 0;
    if( n<0 || n>=count() ) return 0;
    for( int i=0; i<load_.size(); ++i )
        retv += load_.at(i)[n];
    retv /= load_.size();
    Q_ASSERT( retv >= link_coreloads_.minimum().toInt() );
    Q_ASSERT( retv <= link_coreloads_.maximum().toInt() );
    return retv;
}

int LH_QtCPU::averageload()
{
    int retv = 0;
    if( load_.size() < 1 ) return 0;
    if( count() < 1 ) return 0;
    for( int i=0; i<count(); i++ )
        for( int j=0; j<load_.size(); ++j )
            retv += load_.at(j)[i];
    retv /= (count() * load_.size());
    Q_ASSERT( retv >= link_coreloads_.minimum().toInt() );
    Q_ASSERT( retv <= link_coreloads_.maximum().toInt() );
    return retv;
}

