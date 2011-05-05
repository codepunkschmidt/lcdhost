/**
  \file     LH_QtPlugin_NowPlaying.h
  \author   Johan Lindh <johan@linkdata.se>
  \author   Andy Bridges <andy@bridgesuk.com>
  \legalese Copyright (c) 2010 Johan Lindh, Andy Bridges

  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of Link Data Stockholm nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.

  **/


#ifndef LH_QTPLUGIN_NOWPLAYING_H
#define LH_QTPLUGIN_NOWPLAYING_H

#include "../LH_QtPlugin.h"
#include "utils.h"

class LH_NowPlaying_Reader: public QObject
{
    Q_OBJECT
    bool playerFound_;
    TrackInfo info_;
    bool storeInfo(TrackInfo newInfo);
public:
    LH_NowPlaying_Reader(LH_QtPlugin *parent): QObject(parent) { return; }

    TrackInfo info() { return info_; }
    bool playerFound() { return playerFound_; }
    void refresh();

signals:
    void changed();
};



extern LH_NowPlaying_Reader* currentTrack;

class LH_QtPlugin_NowPlaying : public LH_QtPlugin
{
    QTime t;

public:
    const char * lh_name() { return "Now Playing (v2.00)"; }
    const char * lh_shortdesc() { return "Displays the currently playing track from various music players."; }
    const char * lh_author() { return "Andy \"Triscopic\" Bridges"; }
    const char * lh_homepage() { return "<a href=\"http://www.codeleap.co.uk\">CodeLeap</a>"; }
    const char * lh_longdesc()
    {
        return "This plugin provides a text item that can display the currently playing track in one of several popular "
               "3rd-party music players: "
               "<ul>"
               "<li><a href=\"http://www.winamp.com/\">Winamp</a>: Full, native support.<br/></li>"
               "<li><a href=\"http://www.apple.com/itunes/\">iTunes</a>: Full, native support.<br/></li>"
               "<li><a href=\"http://www.foobar2000.org/\">foobar2000</a>: Partial support via the Foobar_Winamp_Spam plugin (no specific home page but v0.98 is located <a href=\"http://home.comcast.net/~selyb/\">here</a>)."
               "<br/><br/>N.B. Currently Foobar supports the progress bar but not seperate album/artist/track fields.<br/></li>"
               "<li>Windows Media Player: Partial support via the \"Windows Live Messenger Music Plugin\", found under Plugins &gt; Background.<br/></li>"
               "<li><a href=\"http://www.videolan.org/vlc/\">VLC</a>: Partial support via the \"MSN Now Playing\" interface, found under Preferences &gt; All &gt; Interface &gt; Control Interfaces.<br/></li>"
               "<li><a href=\"http://www.spotify.com/\">Spotify</a>: Partial, native support (still via the \"MSN Now Playing\" api, but Spotify does this by default).<br/></li>"
               "<li>...and any other player that supports the MSN Now Playing interface, such as <a href=\"http://www.last.fm/download/\">Last.fm</a>, <a href=\"http://getopenpandora.appspot.com/\">OpenPandora</a>, <a href=\"http://getsongbird.com/\">Songbird</a>(+<a href=\"http://addons.songbirdnest.com/addon/1204\">LiveTweeter</a>), <a href=\"http://www.zune.net\">Zune</a> and others."
               "<br/><br/>Note #1: WMP, VLC, Spotify and any other player using the MSN interface will not support the progress bar but should support seperate Artist, Album & Track details due to limitations in the MSN protocol."
               "<br/><br/>Note #2: Also, please be aware the MSN method may not function correctly if another application is seeking to receive music details in this way, e.g. MSN / Windows Live Messenger. Whilst *most* music players check for multiple receivers some do not. In this case one application should receive the now playing details, whilst others will not. If this does affect your music player it is a problem with the player, not this plugin.</li>"
               "</ul>";
    }
    const lh_buildinfo * lh_version( int amaj, int amin )
    {
        static lh_buildinfo buildinfo =
        {
            LH_BUILDINFO_SIG,
            sizeof(lh_buildinfo),
            REVISION,
            LH_API_MAJOR,
            LH_API_MINOR,
            "2.00",
            "http://www.linkdata.se/lcdhost/version.php?arch=$ARCH"
        };
        Q_ASSERT( amaj == LH_API_MAJOR );
        Q_ASSERT( amin >= LH_API_MINOR );
        return &buildinfo;
    }

    const char *lh_load();
    void lh_unload();
    int lh_notify(int, void *);
};

extern LH_QtPlugin_NowPlaying thePlugin;

#endif // LH_QTPLUGIN_NOWPLAYING_H
