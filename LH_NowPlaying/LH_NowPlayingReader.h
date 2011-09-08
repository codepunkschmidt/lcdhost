#ifndef LH_NOWPLAYINGREADER_H
#define LH_NOWPLAYINGREADER_H

#include <QObject>
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QBasicTimer>

#include "LH_QtObject.h"
#include "utils.h"

class LH_NowPlayingReader : public LH_QtObject
{
    Q_OBJECT

    static LH_NowPlayingReader *instance_;

    lh_object obj_;
    bool playerFound_;
    QString artworkCachePath_;
    TrackInfo info_;
    artworkDescription cachedArtwork_;
    QBasicTimer timer_;

    bool storeInfo(TrackInfo newInfo);
    bool get_folder_artwork(TrackInfo newInfo, QString artworkCachePath, artworkDescription &cachedArtwork);

public:
    LH_NowPlayingReader(QObject *parent = 0);
    ~LH_NowPlayingReader();

    TrackInfo info() { return info_; }
    bool playerFound() { return playerFound_; }
    QString artworkFileName() { return cachedArtwork_.fileName; }
    void clearArtwork();

    void refresh();
    void run();
    void timerEvent(QTimerEvent *);

    static LH_NowPlayingReader *instance() { return instance_; }

signals:
    void changed();
    void artworkChanged();
};

#define currentTrack LH_NowPlayingReader::instance()

#endif
