
#include "LgBacklightDevice.h"
#include "LH_LgBacklight.h"
#include "LH_HidDevice.h"
#include <QDebug>

static const int backlightid_ = 7;

LgBacklightDevice::LgBacklightDevice(LH_HidDevice *hd, QObject *parent) :
    QObject(parent),
    hd_(hd),
    color_(Qt::transparent)
{
    setObjectName(hd_->objectName());
    color_ = getDeviceColor();
}

void LgBacklightDevice::setColor(QColor c)
{
    if(color_ != c && setDeviceColor(c))
        emit colorChanged();
}

QColor LgBacklightDevice::getDeviceColor()
{
    if(hd_->online())
    {
        QByteArray d(hd_->readFeature(backlightid_));
        if(d.size() > 3)
        {
            const unsigned char *report = (const unsigned char *) d.constData();
            int r, g, b;
            r = report[1] * 255 / 140;
            if( r > 255 ) r = 255;
            g = report[2];
            b = report[3] * 255 / 150;
            if( b > 255 ) b = 255;
            return QColor(r, g, b);
        }
    }
    return QColor(Qt::transparent);
}

// automatic intensity correction
bool LgBacklightDevice::setDeviceColor(const QColor &c)
{
    if(hd_->online())
    {
        unsigned char report[5];
        report[0] = backlightid_;
        report[1] = 140 * c.red() / 255; // 140
        report[2] = c.green(); // 255
        report[3] = 150 * c.blue() / 255; // 150
        report[4] = 0;
        if(hd_->writeFeature(QByteArray((const char *)report, 5)) > 0)
            return true;
    }
    return false;
}
