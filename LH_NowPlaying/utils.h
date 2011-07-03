#ifndef UTILS_H
#define UTILS_H

#define PLAYER_STATUS_INVALID INT_MIN
#define PLAYER_STATUS_CLOSED -1
#define PLAYER_STATUS_STOPPED 0
#define PLAYER_STATUS_PAUSED 1
#define PLAYER_STATUS_PLAYING 2

#define STRLEN 100

#include <QtGlobal>
#include <QDateTime>
#include <QString>

enum artworkCacheMode
{
    amNone,
    amArtistAndAlbumName,
    amFileFolder,
    amTrack
};

struct artworkDescription
{
    artworkCacheMode cacheMode;
    QString artist;
    QString album;
    QString track;
    QString sourcefolder;
    QString fileName;
};

struct TrackInfo
{
        QString track;
        QString artist;
        QString album;
        QString player;
        int status;
        int totalSecs;
        int currentSecs;
        QString file;
        QDateTime updatedAt;
};


#ifdef Q_WS_WIN
#include <windows.h>
QString LH_GetWindowTitle(HWND hWnd);
#endif

#endif // UTILS_H
