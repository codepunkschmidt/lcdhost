/**
  \file     LH_QtPlugin_Cursor.h
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese Copyright (c) 2010,2011 Andy Bridges
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

 **/

#ifndef LH_QTPLUGIN_CURSOR_H
#define LH_QTPLUGIN_CURSOR_H

#include "../LH_QtPlugin.h"

class LH_QtPlugin_Cursor: public LH_QtPlugin
{
public:
    const char * lh_name() { return "Cursor (v0.31)"; }
    const char * lh_shortdesc() { return "Allows some complex cursor-like and tab-like behaviours"; }
    const char * lh_author() { return "Andy \"Triscopic\" Bridges"; }
    const char * lh_homepage() { return "<a href=\"http://www.codeleap.co.uk\">CodeLeap</a> / <a href=\"http://www.linkdata.se/forum/lcdhost-plugins/(release)-lh_cursor/\">LCDHost: LH_Cursor Thread</a>"; }
    const lh_buildinfo * lh_version( int amaj, int amin )
    {
        static lh_buildinfo buildinfo =
        {
            LH_BUILDINFO_SIG,
            sizeof(lh_buildinfo),
            REVISION,
            LH_API_MAJOR,
            LH_API_MINOR,
            "0.3",
            "http://codeleap.co.uk/lcdhost/lh_cursor/ver.xml"
        };
        Q_ASSERT( amaj == LH_API_MAJOR );
        Q_ASSERT( amin >= LH_API_MINOR );
        return &buildinfo;
    }
    const char * lh_longdesc()
    {
        return "This plugin allows the user to: <ul>"
               "<li>Create menu layouts that can load other layouts when an item is selected by a cursor.</li>"
               "<li>Allows non-menu layouts to load another (menu) layout at the press of a button.</li>"
               "<li>Create tabs or pages in a layout that a user can navigate through.</li>"
               "<li>Create layouts which have \"drill down\" effects, in which panels can be selected to reveal "
               "(or hide) another panel.</li>"
               "</ul>"
               "Note that creating pages and drill downs works by exploiting some behavioural quirks of LCDHost, and is not "
               "as easy to do as other things are; to begin making \"cursor-based\" layouts of any type, it is strongly "
               "recommended that you start by reading the latest version of the <a href='http://forum.linkdata.se/lcdhost-plugins/(release)-lh_cursor/msg703/#msg703'>user guide</a> "
               "and downloading the example layouts provided there."
               ;
    }
};

extern LH_QtPlugin_Cursor thePlugin;

#endif // LH_QTPLUGIN_CURSOR_H

