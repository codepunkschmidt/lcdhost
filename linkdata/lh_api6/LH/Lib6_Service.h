#ifndef LH_LIB6_SERVICE_H
#define LH_LIB6_SERVICE_H

#include <LH/Lib6.h>
#include <LH/Lib6_Object.h>

namespace LH {
namespace Lib6 {

class Service : public Object
{
    Q_OBJECT
    Q_INTERFACES( LH::Lib6::Object )

public:
    Service( const QString & objectname, QObject * parent = 0 ) :
        Object( objectname, parent )
    {}
};

} // namespace Lib6
} // namespace LH

Q_DECLARE_INTERFACE( LH::Lib6::Service, "se.linkdata.LibService/6.0" )

#endif // LH_LIB6_SERVICE_H
