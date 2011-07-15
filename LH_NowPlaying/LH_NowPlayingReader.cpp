#include "LH_NowPlayingReader.h"

LH_NowPlayingReader* currentTrack;

//----------------------------------------------------------------------------------------------------------

bool get_itunes_info(TrackInfo &ti, QString artworkPath, artworkDescription &cachedArtwork, bool &updatedArtwork);
bool get_winamp_info(TrackInfo &ti);
bool get_msn_compat_info(struct TrackInfo &ti);

void close_itunes_connection();

//----------------------------------------------------------------------------------------------------------

LH_NowPlayingReader::~LH_NowPlayingReader() {
    close_itunes_connection();
}

void LH_NowPlayingReader::refresh()
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

bool LH_NowPlayingReader::get_folder_artwork(TrackInfo newInfo, QString artworkCachePath, artworkDescription &cachedArtwork)
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

bool LH_NowPlayingReader::storeInfo(TrackInfo newInfo)
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
