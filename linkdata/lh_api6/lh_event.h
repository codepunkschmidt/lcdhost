#ifndef LH_API6_LH_EVENT_H
#define LH_API6_LH_EVENT_H

#include <QEvent>

namespace lh {
namespace api6 {
namespace event {

/**
  Registering event types using QEvent::registerEventType()
  doesn't match up across dynamic library boundaries.
  Note that the actual event classes are declared with
  the objects that make use of them.
  */
typedef enum
{
    first_type = QEvent::MaxUser - (QEvent::User*2),
    type_initchild,
    type_setproperty,
    last_type
} type;

/**
  Returns a the event name given a type.
  */
const char * name( type t );

} // namespace event
} // namespace api6
} // namespace lh

#endif // LH_API6_LH_EVENT_H
