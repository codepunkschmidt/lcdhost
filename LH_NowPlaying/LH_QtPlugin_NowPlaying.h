/**
  \file     LH_QtPlugin_Text.h
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009 Johan Lindh

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

#ifndef LH_QTPLUGIN_NOWPLAYING_H
#define LH_QTPLUGIN_NOWPLAYING_H

#include "../LH_QtPlugin.h"

class LH_QtPlugin_NowPlaying : public LH_QtPlugin
{
public:
    const char * lh_name() { return "Now Playing (v1.04)"; }
    const char * lh_shortdesc() { return "Displays the currently playing track from Spotify, iTunes or Winamp."; }
    const char * lh_author() { return "Andy \"Triscopic\" Bridges"; }
    const char * lh_homepage() { return "<a href=\"http://www.codeleap.co.uk\">CodeLeap</a> / <a href=\"http://www.linkdata.se/forum/lcdhost-plugins/(release)-lh_nowplaying/\">LCDHost: LH_NowPlaying Thread</a>"; }
    const char * lh_longdesc()
    {
        return "This plugin provides a text item that can display the currently playing track in one of several popular "
               "3rd-party music players, namely <a href=\"http://www.spotify.com/\">Spotify</a>, <a href=\"http://www.apple.com/itunes/\">iTunes</a> or <a href=\"http://www.winamp.com/\">Winamp</a>.";
    }
    const lh_buildinfo * lh_version( int av )
    {
        Q_UNUSED(av);
        static lh_buildinfo buildinfo =
        {
            LH_BUILDINFO_SIG,
            sizeof(lh_buildinfo),
            REVISION,
            LH_API_VERSION,
            "1.00",
            "http://codeleap.co.uk/lcdhost/lh_nowplaying/ver.xml"
        };
        return &buildinfo;
    }
};

extern LH_QtPlugin_NowPlaying thePlugin;

#endif // LH_QTPLUGIN_NOWPLAYING_H
