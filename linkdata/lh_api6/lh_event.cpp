
#include "lh_event.h"

const char * lh::api6::event::name( type t )
{
    switch( t )
    {
    case type_init: return "init";
    case type_setproperty: return "setproperty";
    case first_type:
    case last_type:
        break;
    }
    return "unknown";
}
