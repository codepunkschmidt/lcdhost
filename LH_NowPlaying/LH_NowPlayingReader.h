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
        artworkCachePath_ = LH_QtPlugin::instance()->dir_data();
        Q_ASSERT( artworkCachePath_.endsWith('/') );
        return;
    }

    TrackInfo info() { return info_; }
    bool playerFound() { return playerFound_; }
    QString artworkFileName() { return cachedArtwork_.fileName; }
    void clearArtwork();

    void refresh();
    void run();
    void timerEvent(QTimerEvent *);

signals:
    void changed();
    void artworkChanged();
};

extern LH_NowPlayingReader* currentTrack;

#endif
