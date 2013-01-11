
#include <QDebug>
#include "LgBacklightDevice.h"
#include "LH_LgBacklight.h"

#include <errno.h>

LgBacklightDevice::LgBacklightDevice( const struct hid_device_info *di, LH_LgBacklight *parent )
{
    parent_ = parent;
    name_ = QString::fromWCharArray(di->product_string);
    // name_ = di->path;
    to_remove_ = true;
    product_id_ = di->product_id;
    path_ = di->path;
    color_ = QColor(Qt::transparent);
    backlightid_ = 7;
    errno = 0;

    hid_device *dev = hid_open_path( path_.constData() );
    if( dev )
    {
        unsigned char report[256];
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
            to_remove_ = false;
            // qDebug() << "LgBacklightDevice:" << path_.constData() << r << g << b;
        }
        else
            qDebug() << "LgBacklightDevice can't get feature report for"
                     << name_ << "at" << path_.constData() << strerror(errno);
        hid_close( dev );
    }
    else
    {
        qDebug() << "LgBacklightDevice can't open"
                 << name_ << "at" << path_.constData() << strerror(errno);
    }
#ifdef Q_OS_LINUX
    if(errno == EACCES)
    {
        qDebug("Try adding the following line to <strong><tt>/etc/udev/rules.d/99-logitech.rules</tt></strong>");
        qDebug("<strong><tt>SUBSYSTEM==\"hidraw\", ATTRS{idVendor}==\"%04x\", "
               "ATTRS{idProduct}==\"%04x\", MODE=\"0666\"</tt></strong>",
               0x046d, product_id_);
    }
#endif
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
            qDebug() << "Backlight::setColor() failed for" << name() << "only" << retv << "bytes sent";
        hid_close( dev );
    }
    else
        qDebug() << "Backlight::setColor() failed to open" << name() << "at" << path_.constData() <<
                    strerror(errno);
}
