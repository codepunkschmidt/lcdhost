#include "LH_NowPlayingAlbumArt.h"
#include <QDebug>

LH_PLUGIN_CLASS(LH_NowPlayingAlbumArt)

LH_NowPlayingAlbumArt::LH_NowPlayingAlbumArt()
{
    connect( currentTrack, SIGNAL(artworkChanged()), this, SLOT(refresh_image()) );

    setup_file_ = new LH_Qt_QFileInfo( this, ("Image"), QFileInfo(), LH_FLAG_AUTORENDER | LH_FLAG_READONLY | LH_FLAG_NOSAVE);
    if(currentTrack!=NULL)
        refresh_image();
    //setup_file_->setFlag( LH_FLAG_HIDDEN, true );
    return;
}

lh_class *LH_NowPlayingAlbumArt::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "3rdParty/Music",
        "NowPlayingAlbumArt",
        "Now Playing (Album Art)",
        -1, -1,
        
    };

    return &classInfo;
}

QImage *LH_NowPlayingAlbumArt::render_qimage(int w, int h)
{
    delete image_;
    if( setup_file_->value().isFile())
    {
        image_ = new QImage();
        if(!image_->load(setup_file_->value().absoluteFilePath()))
            qDebug() << "Cannot load image: " << setup_file_->value().absoluteFilePath();
    }
    else
        image_ = new QImage(w,h,QImage::Format_Invalid);
    return image_;
}

void LH_NowPlayingAlbumArt::refresh_image()
{
    setup_file_->setValue(QFileInfo(currentTrack->artworkFileName()));
    this->requestRender();
}
