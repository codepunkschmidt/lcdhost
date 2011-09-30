
#include "lh_event.h"

char * lh::api6::hex( qptrdiff n, char * buf )
{
    if( buf )
    {
        char *p, *q;
        p = q = buf;
        do
        {
            *q++ = "0123456789abcdef"[n&0xf];
            n >>= 4;
        } while( n );
        *q = 0;
        for(--q; p < q; ++p, --q)
            *p = *p ^ *q,
            *q = *p ^ *q,
            *p = *p ^ *q;
    }
    return buf;
}

QEvent::Type lh::api6::event::toType( type t )
{
    static int first_type = QEvent::registerEventType( QEvent::MaxUser - (QEvent::User*3) );
    return (QEvent::Type) (first_type + t);
}

const char * lh::api6::event::name( QEvent::Type t )
{
    static char buf[ sizeof(qptrdiff)*2 + 1 ];
    switch( fromType(t) )
    {
    case type_ping: return "ping";
    case type_create_child: return "create_child";
    case type_init_child: return "init_child";
    case type_init_success: return "type_init_success";
    case type_init_failure: return "type_init_failure";
    case type_set_property: return "set_property";
    case type_api6_unused: return "api6_unused";
    }
    return lh::api6::hex( t, buf );
}

