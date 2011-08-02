#ifndef LH_NOWPLAYINGREADER_H
#define LH_NOWPLAYINGREADER_H

#include "../LH_QtPlugin.h"
#include "utils.h"
#include <QThread>
#include <QFile>
#include <QDebug>
#include <QFileInfo>

class LH_NowPlayingReader: public QThread
{
    Q_OBJECT
    bool playerFound_;
    QString artworkCachePath_;
    TrackInfo info_;
    artworkDescription cachedArtwork_;
    bool storeInfo(TrackInfo newInfo);

    bool get_folder_artwork(TrackInfo newInfo, QString artworkCachePath, artworkDescription &cachedArtwork);

public:
    LH_NowPlayingReader(LH_QtPlugin *parent): QThread((QObject*)parent)
    {
        // get the data directory. we will save a temporary "art.jpg" file
        // here whilst it is valid. When the album changes or the plugin is
        // unloaded this file is automatically cleaned up.
        artworkCachePath_ = LH_QtPlugin::dir_data();
        Q_ASSERT( artworkCachePath_.endsWith('/') );
        return;
    }

    ~LH_NowPlayingReader();

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

extern LH_NowPlayingReader* currentTrack;

#endif
