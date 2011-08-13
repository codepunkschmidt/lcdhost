#ifndef LOGITECHINPUTDEVICE_H
#define LOGITECHINPUTDEVICE_H

#include "../LH_QtInputDevice.h"

class LogitechInputDevice : public LH_QtInputDevice
{
    Q_OBJECT
public:
    explicit LogitechInputDevice(const char *ident, int flags, QObject *parent = 0);
    const char* input_name(int item);
};

#endif // LOGITECHINPUTDEVICE_H
