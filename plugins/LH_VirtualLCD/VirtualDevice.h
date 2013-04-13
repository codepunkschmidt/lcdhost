#ifndef VIRTUALDEVICE_H
#define VIRTUALDEVICE_H

#include <LH_QtDevice.h>
#include <LH_Qt_QImage.h>

class VirtualDevice : public LH_QtDevice
{
    Q_OBJECT

protected:
    LH_Qt_QImage *setup_output_;

public:
    VirtualDevice(LH_QtPlugin *drv);

    const char* open();
    const char* render_argb32(int,int,const void*) { return NULL; }
    const char* render_mono(int,int,const void*) { return NULL; }
    int buttons() { return 0; }
    const char* get_backlight(lh_device_backlight*) { return NULL; }
    const char* set_backlight(lh_device_backlight*) { return NULL; }
    const char* close();
    const char* render_qimage(QImage *img);
};

#endif // VIRTUALDEVICE_H
