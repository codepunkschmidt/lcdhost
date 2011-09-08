/**
  \file     LH_QtPlugin_Cursor.cpp
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

#include "LH_QtPlugin_Cursor.h"

LH_PLUGIN(LH_QtPlugin_Cursor) =
"<?xml version=\"1.0\"?>"
"<lcdhostplugin>"
  "<id>Cursor</id>"
  "<rev>" STRINGIZE(REVISION) "</rev>"
  "<api>" STRINGIZE(LH_API_MAJOR) "." STRINGIZE(LH_API_MINOR) "</api>"
  "<ver>" STRINGIZE(VERSION) "\nr" STRINGIZE(REVISION) "</ver>"
  "<versionurl>http://www.linkdata.se/lcdhost/version.php?arch=$ARCH</versionurl>"
  "<author>Andy \"Triscopic\" Bridges</author>"
  "<homepageurl><a href=\"http://www.codeleap.co.uk\">CodeLeap</a></homepageurl>"
  "<logourl></logourl>"
  "<shortdesc>"
  "Allows some complex cursor-like and tab-like behaviours"
  "</shortdesc>"
  "<longdesc>"
    "This plugin allows the user to: <ul>"
    "<li>Create menu layouts that can load other layouts when an item is selected by a cursor.</li>"
    "<li>Allows non-menu layouts to load another (menu) layout at the press of a button.</li>"
    "<li>Create tabs or pages in a layout that a user can navigate through.</li>"
    "<li>Create layouts which have \"drill down\" effects, in which panels can be selected to reveal "
    "(or hide) another panel.</li>"
    "<li>Have one layout load another after a certain number of seconds.</li>"
    "</ul>"
    "Note that pages that are added will only be visible when the cursor is on them; when building your layout "
    "make sure you have assigned the page a suitable coordinate and move the cursor to it. New pages start with "
    "a coordintate of 1,1 so might not always be visible when first added (i.e. if the cursor is not at 1,1 "
    "newly added pages will be hidden from view immediately)."
"</longdesc>"
"</lcdhostplugin>";

