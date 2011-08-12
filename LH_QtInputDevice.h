#ifndef LH_QTINPUTDEVICE_H
#define LH_QTINPUTDEVICE_H

#include "LH_QtObject.h"

class LH_QtInputDevice : public LH_QtObject
{
    Q_OBJECT
    lh_input_device lh_dev_;

public:
    explicit LH_QtInputDevice( const char *ident, int flags, QObject *parent = 0 );
    ~LH_QtInputDevice();

    lh_input_device *lh_dev() { return &lh_dev_; }

    virtual const char* open() { return NULL; }
    virtual const char* input_name(int /*item*/) { return NULL; }
    virtual void emulate(int /*item*/,int /*flags*/,int /*value*/) { return; }
    virtual const char* close() { return NULL; }
};

#endif // LH_QTINPUTDEVICE_H
