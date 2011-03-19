/**
  \file     LH_QtPlugin_Graph.h
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

#ifndef LH_QTPLUGIN_GRAPH_H
#define LH_QTPLUGIN_GRAPH_H

#include "../LH_QtPlugin.h"

class LH_QtPlugin_Graph : public LH_QtPlugin
{
    Q_OBJECT

public:
    LH_QtPlugin_Graph() : LH_QtPlugin() {}

    const char * lh_name() { return "Graph (v1.21)"; }
    const char * lh_shortdesc() { return "Plots graphs of data items over time"; }
    const char * lh_author() { return "Andy \"Triscopic\" Bridges"; }
    const char * lh_homepage() { return "<a href=\"http://www.codeleap.co.uk\">CodeLeap</a>"; }
    const lh_buildinfo * lh_get_buildinfo() {
        static lh_buildinfo buildinfo =
        {
            LH_BUILDINFO_SIG,
            sizeof(lh_buildinfo),
            REVISION,
            "http://codeleap.co.uk/lcdhost/lh_graph/version.php",
            "1.21"
        };
        return &buildinfo;
    }
    const char * lh_longdesc()
    {
        return  "<p>Plots graphs of data items such as CPU, memory and network load. "
                "Graphs can span 5 seconds to 2 hours by taking up to 600 samples at intervals of up to 12 seconds.</p>";
    }
    const char *lh_load();
    void lh_unload();
};

extern LH_QtPlugin_Graph thePlugin;

#endif // LH_QTPLUGIN_GRAPH_H
