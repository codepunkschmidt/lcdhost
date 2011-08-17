
#include <QBasicTimer>
#include <QTimerEvent>
#include "../LH_QtPlugin.h"
#include "LH_NowPlayingReader.h"

bool get_itunes_info(TrackInfo &ti, QString artworkPath, artworkDescription &cachedArtwork, bool &updatedArtwork);
bool get_winamp_info(TrackInfo &ti);
bool get_msn_compat_info(struct TrackInfo &ti);
void close_itunes_connection();

LH_NowPlayingReader *LH_NowPlayingReader::instance_ = 0;

LH_NowPlayingReader::LH_NowPlayingReader(QObject *parent) :
    LH_QtObject( &obj_, "LH_NowPlayingReader", parent ),
    playerFound_(false)
{
    memset( &info_, 0, sizeof(info_) );
    memset( &cachedArtwork_, 0, sizeof(cachedArtwork_) );

    // get the data directory. we will save a temporary "art.jpg" file
    // here whilst it is valid. When the album changes or the plugin is
    // unloaded this file is automatically cleaned up.
    artworkCachePath_ = LH_QtPlugin::instance()->dir_data();
    Q_ASSERT( artworkCachePath_.endsWith('/') );
    instance_ = this;
    timer_.start(500,this);
    return;
}

LH_NowPlayingReader::~LH_NowPlayingReader()
{
    instance_ = 0;
    timer_.stop();
    close_itunes_connection();
}

//----------------------------------------------------------------------------------------------------------

void LH_NowPlayingReader::timerEvent(QTimerEvent *event)
{
    if( event->timerId() == timer_.timerId() )
    {
        refresh();
        return;
    }
    QObject::timerEvent(event);
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
            clearArtwork();
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

    if(!playerFound_ && artworkFileName()!="")
    {
        clearArtwork();
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

void LH_NowPlayingReader::clearArtwork()
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
