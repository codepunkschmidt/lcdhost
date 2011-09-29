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
    LH_Qt_QStringList *setup_combined_status_;
    LH_Qt_QStringList *setup_mute_status_spk_;
    LH_Qt_QStringList *setup_mute_status_mic_;

    QImage* getPlaceholder()
    {
        QImage* image = new QImage();
        QString status = setup_connection_status_->valueText();
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

        setup_combined_status_ = new LH_Qt_QStringList(this, "Connection Status", QStringList() << "Not Running" << "Not Connected" << "No Speaker" << "Me Speaking" << "Other Speaking" << "Both Speaking", LH_FLAG_HIDDEN | LH_FLAG_READONLY | LH_FLAG_NOSOURCE | LH_FLAG_NOSAVE | LH_FLAG_AUTORENDER);

        setup_connection_status_ = new LH_Qt_QStringList(this, "Connection Status", QStringList() << "Not Running" << "Not Connected" << "Connected", LH_FLAG_HIDDEN | LH_FLAG_READONLY | LH_FLAG_NOSOURCE | LH_FLAG_NOSAVE | LH_FLAG_AUTORENDER);
        setup_connection_status_->setLink("=/Monitoring/3rdParty/TeamSpeak3/Connection Status");
        connect(setup_connection_status_, SIGNAL(changed()), this, SLOT(updateCombinedStatus()));

        setup_talking_ = new LH_Qt_QString(this, "Talking", "", LH_FLAG_HIDDEN | LH_FLAG_READONLY | LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_AUTORENDER);
        setup_talking_->setLink("=/Monitoring/3rdParty/TeamSpeak3/Talking");
        connect(setup_talking_, SIGNAL(changed()), this, SLOT(updateCombinedStatus()));

        setup_talking_me_ = new LH_Qt_bool(this, "Me Talking", false, LH_FLAG_HIDDEN | LH_FLAG_READONLY | LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_AUTORENDER);
        setup_talking_me_->setLink("=/Monitoring/3rdParty/TeamSpeak3/Me Talking");//, true);
        connect(setup_talking_me_, SIGNAL(changed()), this, SLOT(updateCombinedStatus()));

        setup_nickname_ = new LH_Qt_QString(this, "Nickname", "", LH_FLAG_HIDDEN | LH_FLAG_READONLY | LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_AUTORENDER);
        setup_nickname_->setLink("=/Monitoring/3rdParty/TeamSpeak3/Nickname");
        connect(setup_nickname_, SIGNAL(changed()), this, SLOT(updateCombinedStatus()));
        //setup_connection_status_->refreshData();

        setup_mute_status_spk_ = new LH_Qt_QStringList(this, "Speaker Mute Status", QStringList() << "N/A" << "None" << "Muted" << "Active", LH_FLAG_HIDDEN | LH_FLAG_READONLY | LH_FLAG_NOSOURCE | LH_FLAG_NOSAVE | LH_FLAG_AUTORENDER);
        setup_mute_status_spk_->setLink("=/Monitoring/3rdParty/TeamSpeak3/Speaker Status");

        setup_mute_status_mic_ = new LH_Qt_QStringList(this, "Microphone Mute Status", QStringList() << "N/A" << "None" << "Muted" << "Active", LH_FLAG_HIDDEN | LH_FLAG_READONLY | LH_FLAG_NOSOURCE | LH_FLAG_NOSAVE | LH_FLAG_AUTORENDER);
        setup_mute_status_mic_->setLink("=/Monitoring/3rdParty/TeamSpeak3/Microphone Status");

        add_cf_target(setup_image_file_);
        add_cf_source(setup_combined_status_);
        add_cf_source(setup_mute_status_spk_);
        add_cf_source(setup_mute_status_mic_);

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

public slots:
    void updateCombinedStatus()
    {
        QString status = setup_connection_status_->valueText();
        QString talking = setup_talking_->value();
        setup_combined_status_->setValue(
                    status=="Not Running"?"Not Running":
                    status=="Not Connected"?"Not Connected":
                    talking==""?"No Speaker":
                    setup_talking_me_->value()==false?"Other Speaking":
                    talking==setup_nickname_->value()?"Me Speaking":"Both Speaking"
                    );
        qDebug() << setup_combined_status_->valueText();
    }

};

LH_PLUGIN_CLASS(LH_TS3CombinedStatusImage)

#endif // LH_TS3COMBINEDSTATUSIMAGE_H

