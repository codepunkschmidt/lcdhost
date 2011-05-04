/**
  \file     LH_ATITrayToolsImage.cpp
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese Copyright (c) 2010 Andy Bridges
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

#include "LH_ATITrayToolsImage.h"

LH_PLUGIN_CLASS(LH_ATITrayToolsImage)

lh_class *LH_ATITrayToolsImage::classInfo()
{
    static lh_class classinfo =
    {
        sizeof(lh_class),
        "3rdParty/Monitoring/ATITrayTools",
        "ATITrayToolsStatusImage",
        "ATITrayTools Status Image",
        -1, -1,
        lh_instance_calltable_NULL
    };

    return &classinfo;
}

LH_ATITrayToolsImage::LH_ATITrayToolsImage( const char *name, LH_QtPlugin *parent ) : LH_MonitoringImage( name, parent )
{
    data_ = new LH_ATITrayToolsData(this);
    connect_changeType( static_cast<LH_ATITrayToolsData*>(data_)->setup_value_type_ );
}


