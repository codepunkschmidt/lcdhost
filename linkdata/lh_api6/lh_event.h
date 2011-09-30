#ifndef LH_API6_LH_EVENT_H
#define LH_API6_LH_EVENT_H

#include <QEvent>

namespace lh {
namespace api6 {

/**
  Converts an unsigned integer to a hex
  string using a static char buffer of
  at least sizeof(qptrdiff)*2+1.
  */
char * hex( qptrdiff n, char * buf );

namespace event {

/**
  LCDHost custom events. The enumeration
  values are relative to \c lh_event::toType().
  */
typedef enum
{
    type_ping = 0,
    type_create_child,
    type_init_child,
    type_init_success,
    type_init_failure,
    type_set_property,
    type_api6_unused
} type;

/**
  Returns the base of the LCDHost event type
  enumeration, plus the value given, as a
  QEvent::Type value.
  */
QEvent::Type toType( type t = type_ping );

/**
  Returns the LCDHost type enumeration value
  given a QEvent::Type. The return value may
  not be a valid lh_event::type.
  */
inline type fromType( QEvent::Type t )
{
    return (type) ( t - toType() );
}

/**
  Returns a string describing the event.
  For non-LCDHost event types, returns
  the numerical value as a hex string.
  */
const char * name( QEvent::Type t );

} // namespace event
} // namespace api6
} // namespace lh

#endif // LH_API6_LH_EVENT_H
