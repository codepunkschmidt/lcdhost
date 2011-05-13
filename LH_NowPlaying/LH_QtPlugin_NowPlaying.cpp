/**
  \file     LH_QtPlugin_NowPlaying.cpp
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

#include "LH_QtPlugin_NowPlaying.h"
#include <QFileInfo>
#include <QDebug>

LH_QtPlugin_NowPlaying thePlugin;
LH_NowPlaying_Reader* currentTrack;

bool get_itunes_info(TrackInfo &ti, QString artworkPath, artworkDescription &cachedArtwork, bool &updatedArtwork);
bool get_winamp_info(TrackInfo &ti);
bool get_msn_compat_info(struct TrackInfo &ti);

bool get_folder_artwork(TrackInfo newInfo, QString artworkCachePath, artworkDescription &cachedArtwork)
{
    if(newInfo.album == cachedArtwork.album || newInfo.artist == cachedArtwork.artist)
        return false;

    QString folderImage = QFileInfo(newInfo.file).absolutePath()+"/folder.jpg";
    if(QFile::exists(folderImage))
        if(QFile::copy(folderImage, artworkCachePath+"/art.jpg"))
        {
            cachedArtwork.album = newInfo.album;
            cachedArtwork.artist = newInfo.artist;
            cachedArtwork.fileName = artworkCachePath+"/art.jpg";
            return true;
        }
    return false;
}

void close_itunes_connection();

LH_NowPlaying_Reader::~LH_NowPlaying_Reader() {
    close_itunes_connection();
    //clearArtwork();
}

void LH_NowPlaying_Reader::refresh()
{
    playerFound_ = false;
    TrackInfo newInfo;
    bool updatedArtwork = false;

    playerFound_ = playerFound_ || get_itunes_info(newInfo, artworkCachePath_, cachedArtwork_, updatedArtwork);
    playerFound_ = playerFound_ || get_winamp_info(newInfo);
    playerFound_ = playerFound_ || get_msn_compat_info(newInfo);

    if(storeInfo(newInfo))
        emit changed();

    if(playerFound_ && !updatedArtwork &&
            (cachedArtwork_.album != newInfo.album || cachedArtwork_.artist != newInfo.artist)
            )
    {
        currentTrack->clearArtwork();
        updatedArtwork = get_folder_artwork(newInfo, artworkCachePath_, cachedArtwork_);
        if(!updatedArtwork)
        {
            cachedArtwork_.album = newInfo.album;
            cachedArtwork_.artist != newInfo.artist;
            updatedArtwork = true;
        }
    }

    if(!playerFound_ && currentTrack->artworkFileName()!="")
    {
        currentTrack->clearArtwork();
        updatedArtwork = true;
    }
    if(updatedArtwork)
        emit artworkChanged();
}

bool LH_NowPlaying_Reader::storeInfo(TrackInfo newInfo)
{
    bool dirty = false;
    dirty = dirty || (info_.album != newInfo.album);
    dirty = dirty || (info_.artist != newInfo.artist);
    dirty = dirty || (info_.currentSecs != newInfo.currentSecs);
    dirty = dirty || (info_.player != newInfo.player);
    dirty = dirty || (info_.status != newInfo.status);
    dirty = dirty || (info_.totalSecs!= newInfo.totalSecs);
    dirty = dirty || (info_.track!= newInfo.track);

    info_ = newInfo;
    return dirty;
}

const char *LH_QtPlugin_NowPlaying::lh_load() {
    t.start();
    currentTrack = new LH_NowPlaying_Reader(this);
    return NULL;
}

void LH_QtPlugin_NowPlaying::lh_unload() {
    //currentTrack->clearArtwork();
    currentTrack->deleteLater();
}

int LH_QtPlugin_NowPlaying::lh_notify(int code, void *param) {
    Q_UNUSED(code);
    Q_UNUSED(param);
    if (t.elapsed()>=500)
    {
        t.restart();
        if(!currentTrack->isRunning())
             currentTrack->run();
    }
    return 0;
}
