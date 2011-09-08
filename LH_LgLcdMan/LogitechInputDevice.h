#ifndef LOGITECHINPUTDEVICE_H
#define LOGITECHINPUTDEVICE_H

#include "LH_QtInputDevice.h"

class LogitechInputDevice : public LH_QtInputDevice
{
    Q_OBJECT
    lh_input input_;
    unsigned long buttonState_;
public:
    explicit LogitechInputDevice(const char *ident, int flags, QObject *parent = 0);
    const char* input_name(int item);
    void customEvent(QEvent *);

    int buttons() { return (int) buttonState_; }
    unsigned long buttonState() const { return buttonState_; }
    void setButtonState( unsigned long ul );
};

#endif // LOGITECHINPUTDEVICE_H
