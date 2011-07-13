/**
  \file     LH_RivaTunerGraph.cpp
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese
    This module is based on original work by Johan Lindh.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.

  */

#include "LH_RivaTunerGraph.h"

lh_class *LH_RivaTunerGraph::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "3rdParty/Monitoring/RivaTuner",
        "RivaTunerGraph",
        "RivaTuner Graph",
        48,48,
        lh_instance_calltable_NULL
    };

    return &classInfo;
}

const char *LH_RivaTunerGraph::userInit()
{
    if( const char *err = LH_MonitoringGraph::userInit() ) return err;
    data_ = new LH_RivaTunerData( this, true );
    connect_clearData( static_cast<LH_RivaTunerData*>(data_)->setup_value_type_ );

    float graphMin=0;
    if (data_->getGraphMin(graphMin)) setMin(graphMin);
    float graphMax=0;
    if (data_->getGraphMax(graphMax)) setMax(graphMax);
    return 0;
}

LH_PLUGIN_CLASS(LH_RivaTunerGraph)
