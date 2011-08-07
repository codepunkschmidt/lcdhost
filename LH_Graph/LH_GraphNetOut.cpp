/**
  \file     LH_GraphNetOut.cpp
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

#include "LH_Graph.h"

class LH_GraphNetOut : public LH_Graph
{
public:
    const char *userInit()
    {
        if( const char *err = LH_Graph::userInit() ) return err;

        addCustomUnits("kb/s (kilobits per second)", "kb/s", 1024 / 8);
        addCustomUnits("Mb/s (megabits per second)", "Mb/s", 1024 * 1024 / 8);
        addCustomUnits("kB/s (kilobytes per second)", "kB/s", 1024);
        addCustomUnits("MB/s (megabytes per second)", "MB/s", 1024 * 1024);

        setup_linked_values_->setLink("/system/net/out/rate");
        setup_max_->setLink("/system/net/out/max");

        setMin(0.0);
        setMax(1000);
        setYUnit("kb/s");
        return 0;
    }

    static lh_class *classInfo()
    {
        static lh_class classInfo =
        {
            sizeof(lh_class),
            "System/Network/Outbound",
            "SystemNetworkOutboundGraph",
            "Outbound Bandwidth Usage (Graph)",
            48,48
        };

        return &classInfo;
    }

};

LH_PLUGIN_CLASS(LH_GraphNetOut)
