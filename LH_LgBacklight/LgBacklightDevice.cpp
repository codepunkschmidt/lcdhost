
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
    color_ = new LH_Qt_QColor( parent, name_, QColor(Qt::white), LH_FLAG_NOSAVE|LH_FLAG_NOSOURCE );
    parent_->requestRebuild();
}

LgBacklightDevice::~LgBacklightDevice()
{
    delete color_;
    color_ = 0;
    parent_->requestRebuild();
}
