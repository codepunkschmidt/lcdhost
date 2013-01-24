#ifndef LG160X43DEVICE_H
#define LG160X43DEVICE_H

#include "LH_QtDevice.h"

class LH_HidDevice;

class Lg160x43Device : public LH_QtDevice
{
    Q_OBJECT

    LH_HidDevice *hd_;
    unsigned char output_report_id_;

public:
    Lg160x43Device(LH_HidDevice *hi, int output_report_id, LH_QtPlugin *drv = 0);

    const char* open() { return NULL; }
    const char* render_argb32(int,int,const void*) { return NULL; }
    const char* render_mono(int,int,const void*) { return NULL; }
    int buttons() { return 0; }
    const char* get_backlight(lh_device_backlight*) { return NULL; }
    const char* set_backlight(lh_device_backlight*) { return NULL; }
    const char* close() { return NULL; }

    const char* render_qimage(QImage *img);

public slots:
    void onlineChanged(bool b);
};

#endif // LG160X43DEVICE_H
