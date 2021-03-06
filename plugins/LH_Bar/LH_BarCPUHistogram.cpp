/**
  \file     LH_BarCPUHistogram.cpp
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

#include "LH_Bar.h"
#include "LH_QtCPU.h"

class LH_BarCPUHistogram : public LH_Bar
{
    LH_QtCPU cpu_;

public:
    LH_BarCPUHistogram() : LH_Bar(), cpu_(this)
    {
        setMin(0.0);
        setMax(10000.0);
    }

    static lh_class *classInfo()
    {
        static lh_class classInfo =
        {
            sizeof(lh_class),
            "System/CPU",
            "SystemCPUHistogram",
            "Core Load (Bar)",
            48,48,
            lh_object_calltable_NULL,
            lh_instance_calltable_NULL
        };

        return &classInfo;
    }

    int notify(int n, void *p)
    {
        return cpu_.notify(n,p);
    }

    QImage *render_qimage( int w, int h )
    {
        if( LH_Bar::render_qimage(w,h) == NULL ) return NULL;
        qreal *loads = new qreal[ cpu_.count() ];
        if( loads )
        {
            for( int i=0; i<cpu_.count(); i++ )
                loads[i] = cpu_.coreload(i);
            drawList( loads, cpu_.count() );
            delete[] loads;
        }
        return image_;
    }
};

LH_PLUGIN_CLASS(LH_BarCPUHistogram)
