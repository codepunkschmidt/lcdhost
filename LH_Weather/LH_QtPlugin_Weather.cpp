/**
  \file     LH_QtPlugin_Weather.cpp
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

#include "LH_QtPlugin_Weather.h"

LH_PLUGIN(LH_QtPlugin_Weather)

char __lcdhostplugin_xml[] =
"<?xml version=\"1.0\"?>"
"<lcdhostplugin>"
  "<id>Weather</id>"
  "<rev>" STRINGIZE(REVISION) "</rev>"
  "<api>" STRINGIZE(LH_API_MAJOR) "." STRINGIZE(LH_API_MINOR) "</api>"
  "<ver>" STRINGIZE(VERSION) "\nr" STRINGIZE(REVISION) "</ver>"
  "<versionurl>http://www.linkdata.se/lcdhost/version.php?arch=$ARCH</versionurl>"
  "<author>Andy \"Triscopic\" Bridges</author>"
  "<homepageurl><a href=\"http://www.codeleap.co.uk\">CodeLeap</a></homepageurl>"
  "<logourl></logourl>"
  "<shortdesc>"
  "Connects to Yahoo's weather service and displays the results."
  "</shortdesc>"
  "<longdesc>"
"This plugin provides classes that acquire and display weather information.\n\n"
"Weather data is acquired by the Weather Connector component; a layout should only contain one connector. "
"There are two other classes that display weather data, one for text the other for images.\n\n"
"Images are should be placed in a subfolder along with a text file that defines what images to use for "
"each status code for day and night. See the documentation for more details."
"</longdesc>"
"</lcdhostplugin>";
