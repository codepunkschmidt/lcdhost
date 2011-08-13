#include "LH_QtInputDevice.h"
#include "LH_QtPlugin.h"

#define RECAST(o) reinterpret_cast<LH_QtInputDevice*>(o)

static const char* obj_open(lh_input_device*obj)
{
    return RECAST(obj->obj.ref)->open();
}

static const char* obj_input_name(lh_input_device*obj,int item)
{
    return RECAST(obj->obj.ref)->input_name(item);
}

static void obj_emulate(lh_input_device*obj,int item,int flags,int value )
{
    return RECAST(obj->obj.ref)->emulate(item,flags,value);
}

static const char* obj_close(lh_input_device*obj)
{
    return RECAST(obj->obj.ref)->close();
}

LH_QtInputDevice::LH_QtInputDevice( const char *ident, int flags ) :
    LH_QtObject( &lh_dev_.obj, ident, LH_QtPlugin::instance() )
{
    lh_dev_.size = sizeof(lh_input_device);
    lh_dev_.flags = flags;
    lh_dev_.obj_open = obj_open;
    lh_dev_.obj_input_name = obj_input_name;
    lh_dev_.obj_emulate = obj_emulate;
    lh_dev_.obj_close = obj_close;

    parent()->callback( lh_cb_input_create, &lh_dev_ );
    return;
}

LH_QtInputDevice::~LH_QtInputDevice()
{
    callback( lh_cb_destroy );
    memset( &lh_dev_, 0, sizeof(lh_dev_) );
    return;
}
