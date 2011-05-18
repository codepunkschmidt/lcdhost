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
#include <QFile>
#include <QThread>
#include <QDebug>

#define VERSION 2.10

class LH_NowPlaying_Reader: public QThread
{
    Q_OBJECT
    bool playerFound_;
    QString artworkCachePath_;
    TrackInfo info_;
    artworkDescription cachedArtwork_;
    bool storeInfo(TrackInfo newInfo);
public:
    LH_NowPlaying_Reader(LH_QtPlugin *parent): QThread((QObject*)parent) { artworkCachePath_ = parent->state()->dir_plugins; return; }
    ~LH_NowPlaying_Reader();

    TrackInfo info() { return info_; }
    bool playerFound() { return playerFound_; }
    QString artworkFileName()
    {
        return cachedArtwork_.fileName;
    }
    void clearArtwork()
    {
        /*
        qDebug() << "Clean up requested: " << cachedArtwork_.fileName;
        if(cachedArtwork_.fileName!="" && QFile::exists(cachedArtwork_.fileName))
            qDebug() << "Clean up suceeded: " << QFile::remove(cachedArtwork_.fileName);
        else
            qDebug() << "Clean up skipped";
        */
        if(cachedArtwork_.fileName!="" && QFile::exists(cachedArtwork_.fileName))
        {
            #ifdef Q_OS_WIN
                SetFileAttributes((LPCTSTR)cachedArtwork_.fileName.utf16(), 0);
            #endif
            QFile::remove(cachedArtwork_.fileName);
        }
        cachedArtwork_ = (artworkDescription){amNone, "","","",""};
    }

    void refresh();
    virtual void run() { refresh(); }

signals:
    void changed();
    void artworkChanged();
};



extern LH_NowPlaying_Reader* currentTrack;

class LH_QtPlugin_NowPlaying : public LH_QtPlugin
{
    QTime t;

public:
    const char * lh_name() { return "Now Playing"; }
    const char * lh_shortdesc() { return "Displays the currently playing track from various music players."; }
    const char * lh_author() { return "Andy \"Triscopic\" Bridges"; }
    const char * lh_homepage() { return "<a href=\"http://www.codeleap.co.uk\">CodeLeap</a>"; }
    const char * lh_longdesc()
    {
        return  "This plugin provides displays information about the currently playing track in one of several popular "
                "3rd-party music players: "
                "<style type='text/css'>"
                "  .yes  {background-color:#E0FFE0; padding: 2px 4px 2px 4px; color: darkgreen}"
                "  .no   {background-color:#FFE0E0; padding: 2px 4px 2px 4px; color: red}"
                "  .part {background-color:#F8F8D0; padding: 2px 4px 2px 4px; color: darkorange}"
                "  .api  {background-color:#E0E0FF; padding: 2px 4px 2px 4px}"
                "  .info {background-color:#EBEBEB; padding: 2px 4px 2px 4px}"
                "  .head {background-color:#707070; padding: 2px 4px 2px 4px; color: white;}"
                "</style>"
                "<br/><table>"
                "<tr>"
                "<th class='head'>Player</th>"
                "<th class='head'>API</th>"
                "<th class='head'>Separate Album, <br/>Artist & Track</th>"
                "<th class='head'>Progress Bar & <br/>Played Times</th>"
                "<th class='head'>Album<br/>Artwork</th>"
                "</tr>"

                "<tr>"
                "<td class='info' rowspan='2'><a href=\"http://www.apple.com/itunes/\">iTunes</a></td>"
                "<td align='center' class='api'>iTunes</td>"
                "<td align='center' class='yes'>Yes</td>"
                "<td align='center' class='yes'>Yes</td>"
                "<td align='center' class='yes'>Yes</td>"
                "</tr>"
                "<tr>"
                "<td class='info' colspan='4'>Full, native support. Just run iTunes and it should work immediately.</td>"
                "</tr>"

                "<tr>"
                "<td class='info' rowspan='2'><a href=\"http://www.winamp.com/\">Winamp</a></td>"
                "<td align='center' class='api'>Winamp</td>"
                "<td align='center' class='yes'>Yes</td>"
                "<td align='center' class='yes'>Yes</td>"
                "<td align='center' class='part'>Partial<sup>1</sup></td>"
                "</tr>"
                "<tr>"
                "<td class='info' colspan='4'>Full, native support. Just run Winamp and it should work immediately.</td>"
                "</tr>"

                "<tr>"
                "<td class='info' rowspan='2'><a href=\"http://www.foobar2000.org/\">foobar2000</a></td>"
                "<td align='center' class='api'>Winamp</td>"
                "<td align='center' class='no'>No</td>"
                "<td align='center' class='yes'>Yes</td>"
                "<td align='center' class='no'>No</td>"
                "</tr>"
                "<tr>"
                "<td class='info' colspan='4'>Requires the Foobar_Winamp_Spam plugin (no specific home page but v0.98 is located <a href=\"http://home.comcast.net/~selyb/\">here</a>)</td>"
                "</tr>"

                "<tr>"
                "<td class='info' rowspan='2'><a href='http://windows.microsoft.com/en-US/windows/products/windows-media-player'>Windows Media <br/>Player</a></td>"
                "<td align='center' class='api'>MSN<sup>2</sup></td>"
                "<td align='center' class='yes'>Yes</td>"
                "<td align='center' class='no'>No<sup>3</sup></td>"
                "<td align='center' class='no'>No<sup>3</sup></td>"
                "</tr>"
                "<tr>"
                "<td class='info' colspan='4'>Requires the \"Windows Live Messenger Music Plugin\", found under Plugins &gt; Background.</td>"
                "</tr>"

                "<tr>"
                "<td class='info' rowspan='2'><a href=\"http://www.videolan.org/vlc/\">VLC Player</a></td>"
                "<td align='center' class='api'>MSN<sup>2</sup></td>"
                "<td align='center' class='yes'>Yes</td>"
                "<td align='center' class='no'>No<sup>3</sup></td>"
                "<td align='center' class='no'>No<sup>3</sup></td>"
                "</tr>"
                "<tr>"
                "<td class='info' colspan='4'>Requires the \"MSN Now Playing\" interface to be enabled, found under Preferences &gt; All &gt; Interface &gt; Control Interfaces.</td>"
                "</tr>"

                "<tr>"
                "<td class='info' rowspan='2'><a href=\"http://www.spotify.com/\">Spotify</a></td>"
                "<td align='center' class='api'>MSN<sup>2</sup></td>"
                "<td align='center' class='yes'>Yes</td>"
                "<td align='center' class='no'>No<sup>3</sup></td>"
                "<td align='center' class='no'>No<sup>3</sup></td>"
                "</tr>"
                "<tr>"
                "<td class='info' colspan='4'>Partial, native support. Just run Spotify and it should work immediately.</td>"
                "</tr>"

                "</table>"
                "<br/><br/>"
                "...and any other player that supports the MSN Now Playing interface, such as <a href=\"http://www.last.fm/download/\">Last.fm</a>, <a href=\"http://getopenpandora.appspot.com/\">OpenPandora</a>, <a href=\"http://getsongbird.com/\">Songbird</a>(+<a href=\"http://addons.songbirdnest.com/addon/1204\">LiveTweeter</a>), <a href=\"http://www.zune.net\">Zune</a> and others."
                "<br/><br/><sup>1</sup> Album artwork is only available from Winamp if the music's folder contains &quot;folder.jpg&quot;."
                "<br/><br/><sup>2</sup> Tests suggest the MSN interface does not function under 64-bit Windows."
                "<br/><br/><sup>3</sup> The MSN interface only supports the seperate Artist, Album & Track details, hence players using this system cannot display the progress bar or album art."
                ;
    }
    const lh_buildinfo * lh_version( int amaj, int amin )
    {
        static lh_buildinfo buildinfo = LH_STD_BUILDINFO_WITH_VERSION(VERSION);
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
