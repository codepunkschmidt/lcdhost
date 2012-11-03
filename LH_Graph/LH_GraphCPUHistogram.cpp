/**
  \file     LH_GraphCPUHistogram.cpp
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
#include "QDebug"
#include "LH_QtCPU.h"

#include "LH_QtPlugin_Graph.h"

class LH_GraphCPUHistogram : public LH_Graph
{
    LH_QtCPU cpu_;
    int *valCount;
    qreal *valCache;
    qreal *lastVal;
    bool initialized;

    void initialize(int cpuCount){
        if (cpuCount != 0) {
            initialized = true;
            if(linesCount() != cpuCount)
            {
                clearLines();
                for( int i=0; i<cpu_.count(); i++ )
                    addLine("CPU/Core #"+QString::number(i));
            }

            valCount = new int[cpuCount];
            valCache = new qreal[cpuCount];
            lastVal = new qreal[cpuCount];

            for(int i =0; i<cpuCount; i++ )
            {
                valCount[i] = 0;
                valCache[i] = 0;
                lastVal[i] = 0;
            }
        }
    }

public:
    explicit LH_GraphCPUHistogram() : LH_Graph(gdmHybrid,cpu_histogram_), cpu_(this)
    {
        initialized = false;

        setMin(0.0);
        setMax(100.0, BoundGrowthFixed);
        setYUnit("%");

        cpu_.smoothingHidden(true);
    }

    virtual const char *userInit()
    {
        initialize(cpu_.count());
        return 0;
    }

    ~LH_GraphCPUHistogram()
    {
        delete valCount;
        delete valCache;
        delete lastVal;
    }

    static lh_class *classInfo()
    {
        static lh_class classInfo =
        {
            sizeof(lh_class),
            "System/CPU",
            "SystemCPUHistogramGraph",
            "Core Load (Graph)",
            48,48,
            lh_object_calltable_NULL,
            lh_instance_calltable_NULL
        };

        return &classInfo;
    }

    int notify(int n, void *p)
    {
        if (dataMode() != gdmExternallyManaged)
        {
            if (cpu_.count()!=0)
            {
                if (!initialized) initialize(cpu_.count());

                for(int i =0; i<cpu_.count(); i++ )
                {
                    if(n&LH_NOTE_CPU)
                    {
                        valCache[i]+=cpu_.coreload(i)/100;
                        valCount[i]+=1;
                    }
                    if(n&LH_NOTE_SECOND)
                    {
                        if (valCount[i]!=0)
                        {
                            lastVal[i] = valCache[i]/valCount[i];
                            addValue(lastVal[i], i);
                        }
                        valCache[i] = 0;
                        valCount[i] = 0;
                    }
                }
            }
        }
        else
            callback(lh_cb_render,NULL);
        return LH_Graph::notify(n,p) | cpu_.notify(n,p) | LH_NOTE_SECOND;
    }

    QImage *render_qimage( int w, int h )
    {
        if( LH_Graph::render_qimage(w,h) == NULL ) return NULL;
        drawAll();
        return image_;
    }
};

LH_PLUGIN_CLASS(LH_GraphCPUHistogram)
