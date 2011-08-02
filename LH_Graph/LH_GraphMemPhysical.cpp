/**
  \file     LH_GraphMemPhysical.cpp
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
#include "../LH_Qt_int.h"

class LH_GraphMemPhysical : public LH_Graph
{
    double unitBase;
    LH_Qt_int *link_phys_mem_used_;
    LH_Qt_int *link_phys_mem_total_;

public:
    const char *userInit()
    {
        if( const char *err = LH_Graph::userInit() ) return err;
        unitBase = 1024 * 1024 * 1024;
        setMin(0.0);
        setMax(1000.0);
        setYUnit("GB");
        if( const char *err = LH_Graph::userInit() ) return err;
        link_phys_mem_used_ = new LH_Qt_int(this,"LinkPhysMemUsed",0);
        link_phys_mem_used_->setLink("/system/memory/physical/used");
        link_phys_mem_total_ = new LH_Qt_int(this,"LinkPhysMemTotal",0);
        link_phys_mem_total_->setLink("/system/memory/physical/total");
        return 0;
    }

    static lh_class *classInfo()
    {
        static lh_class classInfo =
        {
            sizeof(lh_class),
            "System/Memory/Physical",
            "SystemMemoryPhysicalGraph",
            "Physical memory used (Graph)",
            48,48
        };
        return &classInfo;
    }

    int notify(int n, void *p)
    {
        Q_UNUSED(p);

        if(!n || n&LH_NOTE_SECOND)
        {
            setMax( link_phys_mem_total_->value() / unitBase );
            addValue( link_phys_mem_used_->value() / unitBase );
            callback(lh_cb_render,NULL);
        }
        return LH_NOTE_SECOND;
    }

    QImage *render_qimage( int w, int h )
    {
        if( LH_Graph::render_qimage(w,h) == NULL ) return NULL;
        drawSingle();
        return image_;
    }
};

LH_PLUGIN_CLASS(LH_GraphMemPhysical)
