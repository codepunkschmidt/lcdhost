#ifndef LH_TS3CONNECTIONIMAGE_H
#define LH_TS3CONNECTIONIMAGE_H

#include "../LH_Monitoring/LH_QImage/LH_QImage.h"
#include "../LH_Qt_QStringList.h"

class LH_TS3ConnectionImage : public LH_QImage
{
protected:
    LH_Qt_QStringList *setup_connection_status_;

    QImage* getPlaceholder()
    {
        QImage* image = new QImage();
        QString status = setup_connection_status_->value();
        image->load(QString(":/images/%1.png")
                    .arg(status=="Not Running"?"notrunning":(status=="Not Connected"?"unconnected":"active"))
                    );
        return image;
    }
public:
    const char *userInit()
    {
        if( const char *err = LH_QImage::userInit() ) return err;

        setup_show_placeholder_->setTitle("Use default images");

        setup_connection_status_ = new LH_Qt_QStringList(this, "Connection Status", QStringList() << "Not Running" << "Not Connected" << "Connected", LH_FLAG_NOSOURCE | LH_FLAG_NOSAVE | LH_FLAG_AUTORENDER);
        setup_connection_status_->setLink("Monitoring/3rdParty/TeamSpeak3/Connection Status");
        setup_connection_status_->refreshData();

        return 0;
    }


    static lh_class *classInfo()
    {
        static lh_class classInfo =
        {
            sizeof(lh_class),
            "3rdParty/TeamSpeak 3",
            "TS3ConnectionImage",
            "TS3 Connection Status (Image)",
            16,16
        };
        return &classInfo;
    }


};

LH_PLUGIN_CLASS(LH_TS3ConnectionImage)

#endif // LH_TS3CONNECTIONIMAGE_H
