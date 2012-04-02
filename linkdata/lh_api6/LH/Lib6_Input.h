#ifndef LH_LIB6_INPUT_H
#define LH_LIB6_INPUT_H

#include <LH/Lib6.h>
#include <QString>

/**
  The input flags are used to describe an input device
  and it's events in general.
  */
typedef enum lh_input_flag_t
{
    lh_input_usage      = 0x0001, /**< input comes from a button (a 'usage' in HID terms) */
    lh_input_value      = 0x0002, /**< input comes from an axis (a 'value' in HID terms) */
    lh_input_pos        = 0x0004, /**< button pressed or positive axis values */
    lh_input_neg        = 0x0008, /**< button released or negative axis values */

    lh_input_button     = lh_input_usage,
    lh_input_axis       = lh_input_value,
    lh_input_pressed    = lh_input_pos,
    lh_input_released   = lh_input_neg,

    lh_input_state_mask = (lh_input_usage|lh_input_value|lh_input_pos|lh_input_neg),

    lh_input_keyboard    = 0x1000, /**< this is a keyboard type device */
    lh_input_mouse       = 0x2000, /**< this is a mouse type device */
    lh_input_joystick    = 0x4000, /**< this is a joystick type device */
    lh_input_other       = 0x8000, /**< this is another of type device */

    lh_input_device_mask = (lh_input_keyboard|lh_input_mouse|lh_input_joystick|lh_input_other),

    lh_input_unused
} lh_input_flag;

namespace LH {
namespace Lib6 {

/**
  Used when the state of an input device (button, slider, stick, whatever) changes.

  \c devicename must be a globally unique id for the device. Suggested format is
  to use the HID codes in four-hexadecimal digits groups separated by colons:

    vendor:product:version:page:usage

  \c item is used to uniquely identify the control for that device. Together
  with ident, forms a string uniquely identifying the input control in the format
  "ident/+item", which is used when storing references to the control in
  setup items. Note that the 'item' value is considered a signed integer, and
  the sign is always encoded ('+' or '-'). In case of a specific device state
  being encoded, '/flags' are appended.

  \c flags are used to describe an input device or input device event
  in general terms. They may be zero.

  \c value is the device item's current value. This member is only
  relevant when this class is used to describe an input device event.
  The range is not normalized (ie, min and max may not be known),
  so you should use the flags to determine axis and button state.
  */
class Input
{
    QString devicename_;
    int item_;
    int flags_;
    int value_;
public:
    Input()
    {}

    Input( const Input & other ) :
        devicename_( other.devicename_ ),
        item_( other.item_ ),
        flags_( other.flags_ ),
        value_( other.value_ )
    {}

    Input( const QString & devname, int item, int flags = 0, int val = 0 ) :
        devicename_(devname),
        item_(item),
        flags_(flags),
        value_(val)
    {}

    const QString & deviceName() const { return devicename_; }
    int item() const { return item_; }
    int flags() const { return flags_; }
    int value() const { return value_; }

    void setDeviceName( const QString & name ) { devicename_ = name; }
    void setItem( int item ) { item_ = item; }
    void setFlags( int flags ) { flags_ = flags; }
    void setValue( int value ) { value_ = value; }
};

} // namespace Lib6
} // namespace LH

#endif // LH_LIB6_INPUT_H
