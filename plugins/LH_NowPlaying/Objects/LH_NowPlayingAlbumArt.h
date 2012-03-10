#ifndef LH_NOWPLAYINGALBUMART_H
#define LH_NOWPLAYINGALBUMART_H

#include "LH_QtInstance.h"
#include "LH_Qt_QFileInfo.h"
#include "LH_QtPlugin_NowPlaying.h"

class LH_NowPlayingAlbumArt : public LH_QtInstance
{
    Q_OBJECT

protected:
    LH_Qt_QFileInfo *setup_file_;
    QString curAlbum_;
    QString curArtist_;
    QString curTitle_;

public:
    LH_NowPlayingAlbumArt();

    int polling() { return 0; }
    int notify(int n,void* p)  { Q_UNUSED(n); Q_UNUSED(p); return 0; }
    int width( void*obj,int h ) { Q_UNUSED(obj); Q_UNUSED(h); return -1; }
    int height( void*obj,int h ) { Q_UNUSED(obj); Q_UNUSED(h); return -1; }
    QImage *render_qimage( int w, int h );

    static lh_class *classInfo();

public slots:
    void refresh_image();
};

#endif // LH_NOWPLAYINGALBUMART_H
