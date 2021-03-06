#ifndef LH_TS3SPEAKINGIMAGE_H
#define LH_TS3SPEAKINGIMAGE_H

#include "LH_QImage.h"
#include "LH_Qt_QStringList.h"

class LH_TS3SpeakingImage : public LH_QImage
{
protected:
    LH_Qt_QString *setup_talking_;

    QImage* getPlaceholder()
    {
        QImage* image = new QImage();
        image->load(QString(":/images/%1.png")
                    .arg(setup_talking_->value()==""?"empty":"talking")
                    );
        return image;
    }

public:
    const char *userInit()
    {
        if( const char *err = LH_QImage::userInit() ) return err;

        //setup_show_placeholder_->setTitle("Use default images");

        setup_talking_ = new LH_Qt_QString(this, "Talking", "", LH_FLAG_HIDDEN | LH_FLAG_READONLY | LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_AUTORENDER);
        setup_talking_->setLink("=/3rdParty/TeamSpeak 3/Speaking");
        //setup_talking_->refreshData();

        add_cf_target(setup_image_file_);
        add_cf_source(setup_talking_);

        return 0;
    }

    static lh_class *classInfo()
    {
        static lh_class classInfo =
        {
            sizeof(lh_class),
            "3rdParty/TeamSpeak 3",
            "TS3SpeakingStatusImage",
            "TS3 Speaking Status (Image)",
            16,16,
            lh_object_calltable_NULL,
            lh_instance_calltable_NULL
        };
        return &classInfo;
    }

};

LH_PLUGIN_CLASS(LH_TS3SpeakingImage)

#endif // LH_TS3SPEAKINGIMAGE_H
