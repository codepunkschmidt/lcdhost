#include "LH_NowPlayingAlbumArt.h"
#include <QDebug>

LH_PLUGIN_CLASS(LH_NowPlayingAlbumArt)

LH_NowPlayingAlbumArt::LH_NowPlayingAlbumArt()
{
    connect( lh_plugin(), SIGNAL(updated_data()), this, SLOT(refresh_image()) );

    setup_file_ = new LH_Qt_QFileInfo( this, tr("Image"), QFileInfo(), LH_FLAG_AUTORENDER | LH_FLAG_READONLY | LH_FLAG_NOSAVE_DATA);
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
        lh_object_calltable_NULL,
        lh_instance_calltable_NULL
    };

    return &classInfo;
}

QImage *LH_NowPlayingAlbumArt::render_qimage(int w, int h)
{
    if(QImage *img = initImage(w, h))
    {
        if(setup_file_->value().isFile())
        {
            if(!img->load(setup_file_->value().absoluteFilePath()))
                qDebug() << "Cannot load image: " << setup_file_->value().absoluteFilePath();
        }
        return img;
    }
    return 0;
}

void LH_NowPlayingAlbumArt::refresh_image()
{
    if(player!=NULL)
    {
        QFileInfo f(player->GetCoverPath());
        if(setup_file_->value().absoluteFilePath() != f.absoluteFilePath() ||
                player->GetAlbum() != curAlbum_ ||
                player->GetArtist() != curArtist_ ||
                player->GetTitle() != curTitle_
                )
        {
            curAlbum_ = player->GetAlbum();
            curArtist_ = player->GetArtist();
            curTitle_ = player->GetTitle();
            setup_file_->setValue(f);
            this->requestRender();
        }
    }
}
