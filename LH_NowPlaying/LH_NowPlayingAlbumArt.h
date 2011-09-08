#ifndef LH_NOWPLAYINGALBUMART_H
#define LH_NOWPLAYINGALBUMART_H

#include "LH_QtInstance.h"
#include "LH_Qt_QFileInfo.h"

class LH_NowPlayingAlbumArt : public LH_QtInstance
{
    Q_OBJECT

protected:
    LH_Qt_QFileInfo *setup_file_;

public:
    const char *userInit();

    QImage *render_qimage( int w, int h );

    static lh_class *classInfo();

public slots:
    void refresh_image();
};

#endif // LH_NOWPLAYINGALBUMART_H
