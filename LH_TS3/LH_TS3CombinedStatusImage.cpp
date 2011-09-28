#ifndef LH_TS3COMBINEDSTATUSIMAGE_H
#define LH_TS3COMBINEDSTATUSIMAGE_H

#include "LH_QImage.h"
#include "LH_Qt_QString.h"
#include "LH_Qt_QStringList.h"

class LH_TS3CombinedStatusImage : public LH_QImage
{
protected:
    LH_Qt_QStringList *setup_connection_status_;
    LH_Qt_QString *setup_talking_;
    LH_Qt_bool *setup_talking_me_;
    LH_Qt_QString *setup_nickname_;

    QImage* getPlaceholder()
    {
        QImage* image = new QImage();
        QString status = setup_connection_status_->valueText();
        QString talking = setup_talking_->value();
        image->load(QString(":/images/LED_%1.png")
                    .arg(
                        status=="Not Running"?"NotRunning":
                        status=="Not Connected"?"NotConnected":
                        talking==""?"NoSpeaker":
                        setup_talking_me_->value()==false?"OtherSpeaking":
                        talking==setup_nickname_->value()?"MeSpeaking":"BothSpeaking"
                        )
                    );
        return image;
    }
public:
    const char *userInit()
    {
        if( const char *err = LH_QImage::userInit() ) return err;

        //setup_show_placeholder_->setTitle("Use default images");

        setup_connection_status_ = new LH_Qt_QStringList(this, "Connection Status", QStringList() << "Not Running" << "Not Connected" << "Connected", LH_FLAG_HIDDEN | LH_FLAG_READONLY | LH_FLAG_NOSOURCE | LH_FLAG_NOSAVE | LH_FLAG_AUTORENDER);
        setup_connection_status_->setLink("=/Monitoring/3rdParty/TeamSpeak3/Connection Status");

        setup_talking_ = new LH_Qt_QString(this, "Talking", "", LH_FLAG_HIDDEN | LH_FLAG_READONLY | LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_AUTORENDER);
        setup_talking_->setLink("=/Monitoring/3rdParty/TeamSpeak3/Talking");

        setup_talking_me_ = new LH_Qt_bool(this, "Me Talking", false, LH_FLAG_HIDDEN | LH_FLAG_READONLY | LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_AUTORENDER);
        setup_talking_me_->setLink("=/Monitoring/3rdParty/TeamSpeak3/Me Talking");//, true);

        setup_nickname_ = new LH_Qt_QString(this, "Nickname", "", LH_FLAG_HIDDEN | LH_FLAG_READONLY | LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_AUTORENDER);
        setup_nickname_->setLink("=/Monitoring/3rdParty/TeamSpeak3/Nickname");
        //setup_connection_status_->refreshData();

        add_cf_target(setup_image_file_);
        add_cf_source(setup_connection_status_);

        return 0;
    }


    static lh_class *classInfo()
    {
        static lh_class classInfo =
        {
            sizeof(lh_class),
            "3rdParty/TeamSpeak 3",
            "TS3CombinedStatusImage",
            "TS3 Combined Status (Image)",
            16,16
        };
        return &classInfo;
    }


};

LH_PLUGIN_CLASS(LH_TS3CombinedStatusImage)

#endif // LH_TS3COMBINEDSTATUSIMAGE_H

