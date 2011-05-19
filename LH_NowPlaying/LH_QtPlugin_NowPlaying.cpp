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

LH_PLUGIN(LH_QtPlugin_NowPlaying);
lh_buildinfo buildinfo = LH_STD_BUILDINFO_WITH_VERSION(VERSION);


LH_NowPlaying_Reader* currentTrack;

bool get_itunes_info(TrackInfo &ti, QString artworkPath, artworkDescription &cachedArtwork, bool &updatedArtwork);
bool get_winamp_info(TrackInfo &ti);
bool get_msn_compat_info(struct TrackInfo &ti);

bool get_folder_artwork(TrackInfo newInfo, QString artworkCachePath, artworkDescription &cachedArtwork)
{
    QFileInfo newItemFile(newInfo.file);
    QString newItemFilePath = newItemFile.absolutePath();
    QString folderImage = newItemFilePath+"/folder.jpg";
    QString artworkCacheFileName = QString("%0%1art.%2").arg(artworkCachePath).arg(artworkCachePath.endsWith("/")? "" : "/").arg("jpg");
    if(QFile::exists(folderImage))
    {
        if(QFile::exists(artworkCacheFileName))
        {
            #ifdef Q_OS_WIN
                SetFileAttributes((LPCTSTR)artworkCacheFileName.utf16(), 0);
            #endif
            QFile::remove(artworkCacheFileName);
        }
        if(QFile::copy(folderImage, artworkCacheFileName))
        {
            #ifdef Q_OS_WIN
                SetFileAttributes((LPCTSTR)artworkCacheFileName.utf16(), 0);
            #endif
            cachedArtwork.cacheMode = amFileFolder;
            cachedArtwork.album = newInfo.album;
            cachedArtwork.artist = newInfo.artist;
            cachedArtwork.sourcefolder = newItemFilePath;
            cachedArtwork.fileName = artworkCacheFileName;
            return true;
        } else {
            qWarning() << "NowPlaying: Unable to copy album artwork from " << folderImage;
        }
    }
    return false;
}

void close_itunes_connection();

LH_NowPlaying_Reader::~LH_NowPlaying_Reader() {
    close_itunes_connection();
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

    if(playerFound_ && !updatedArtwork)
    {
        QFileInfo newItemFile(newInfo.file);
        QString newItemFilePath = newItemFile.absolutePath();
        bool validArtwork = false;
        switch(cachedArtwork_.cacheMode)
        {
        case amArtistAndAlbumName:
            validArtwork = (cachedArtwork_.album == newInfo.album && cachedArtwork_.artist == newInfo.artist);
            break;
        case amFileFolder:
            validArtwork = (cachedArtwork_.sourcefolder == newItemFilePath);
            break;
        case amTrack:
            validArtwork = (cachedArtwork_.track == newInfo.track && cachedArtwork_.album == newInfo.album && cachedArtwork_.artist == newInfo.artist);
            break;
        case amNone:
            validArtwork = false;
            break;
        }
        if(!validArtwork)
        {
            currentTrack->clearArtwork();
            updatedArtwork = get_folder_artwork(newInfo, artworkCachePath_, cachedArtwork_);
            cachedArtwork_.album = newInfo.album;
            cachedArtwork_.artist = newInfo.artist;
            if(!updatedArtwork)
            {
                cachedArtwork_.sourcefolder = newItemFilePath;
                updatedArtwork = true;
            } else {
                cachedArtwork_.track == newInfo.track;
                cachedArtwork_.cacheMode = amTrack;
            }
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
    currentTrack = new LH_NowPlaying_Reader(this);
    timer_.setInterval(500);
    timer_.start();
    connect(&timer_, SIGNAL(timeout()), this, SLOT(refresh_data()));
    return NULL;
}

void LH_QtPlugin_NowPlaying::lh_unload() {
    //currentTrack->clearArtwork();
    timer_.stop();
    currentTrack->deleteLater();
}

void LH_QtPlugin_NowPlaying::refresh_data() {
    if(!currentTrack->isRunning())
         currentTrack->run();
}
