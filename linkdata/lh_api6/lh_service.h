#ifndef LH_API6_LH_SERVICE_H
#define LH_API6_LH_SERVICE_H

#include "lh_object.h"

namespace lh {
namespace api6 {

class service : public object
{
    Q_OBJECT
    Q_INTERFACES( lh::api6::object )

public:
    service( const QString & objectname, QObject * parent = 0 ) :
        object( objectname, parent )
    {}
};

} // namespace api6
} // namespace lh

Q_DECLARE_INTERFACE( lh::api6::service, "se.linkdata.lh_service/6.0" )

#endif // LH_API6_LH_SERVICE_H
