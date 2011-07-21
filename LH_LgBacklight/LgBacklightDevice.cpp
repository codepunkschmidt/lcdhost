
#include <QDebug>
#include "LgBacklightDevice.h"
#include "LH_LgBacklight.h"

LgBacklightDevice::LgBacklightDevice( const struct hid_device_info *di, LH_LgBacklight *parent )
{
    parent_ = parent;
    name_ = QString::fromWCharArray(di->product_string);
    to_remove_ = false;
    product_id_ = di->product_id;
    hiddev_ = 0;
    path_ = di->path;
    color_ = QColor(Qt::transparent);
    backlightid_ = 7;

    hid_device *dev = hid_open_path( path_.constData() );
    if( dev )
    {
        unsigned char report[5];
        memset( report, 0, sizeof(report) );
        report[0] = backlightid_; // hidapi leading byte
        if( hid_get_feature_report( dev, report, sizeof(report) ) > 0 )
        {
            int r, g, b;
            r = report[1] * 255 / 140;
            if( r > 255 ) r = 255;
            g = report[2];
            b = report[3] * 255 / 150;
            if( b > 255 ) b = 255;
            color_ = QColor( r, g, b );
        }
        hid_close( dev );
    }
}

// automatic intensity correction
void LgBacklightDevice::setColor( QColor c )
{
    color_ = c;
    hid_device *dev = hid_open_path( path_.constData() );
    if( dev )
    {
        unsigned char report[5];
        report[0] = backlightid_;
        report[1] = 140 * color_.red() / 255; // 140
        report[2] = color_.green(); // 255
        report[3] = 150 * color_.blue() / 255; // 150
        report[4] = 0;
        int retv = hid_send_feature_report( dev, report, sizeof(report) );
        if( retv != sizeof(report) )
        {
            qDebug() << "Backlight: failed to set color for" << name() << "only" << retv << "bytes sent";
        }
        hid_close( dev );
    }
    else
        qDebug() << "Backlight: failed to open" << name();
}
