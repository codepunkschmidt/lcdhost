/**
  \file     LH_Aida64Image.cpp
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

#include "LH_Aida64Image.h"

LH_PLUGIN_CLASS(LH_Aida64Image)

lh_class *LH_Aida64Image::classInfo()
{
    static lh_class classinfo =
    {
        sizeof(lh_class),
        "3rdParty/Monitoring/Aida64",
        "Aida64StatusImage",
        "Aida64 Status Image",
        -1, -1,
        lh_instance_calltable_NULL
    };

    return &classinfo;
}

LH_Aida64Image::LH_Aida64Image()
{
    data_ = new LH_Aida64Data(this, mdmAll);
    connect_changeType( static_cast<LH_Aida64Data*>(data_)->setup_value_type_ );
    connect_updateImage( static_cast<LH_Aida64Data*>(data_)->setup_value_group_ );
    connect_updateImage( static_cast<LH_Aida64Data*>(data_)->setup_value_sensor_ );
}
