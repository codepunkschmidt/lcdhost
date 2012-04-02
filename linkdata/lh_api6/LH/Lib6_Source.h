#ifndef LH_LIB6_SOURCE_H
#define LH_LIB6_SOURCE_H

#include <LH/Lib6.h>
#include <LH/Lib6_Linkable.h>

namespace LH {
namespace Lib6 {

/**
  source is a convenience class for creating
  a data source that need no UI or to be saved.
  */
class Source : public Linkable
{
    Q_OBJECT
    Q_INTERFACES( LH::Lib6::Object LH::Lib6::Linkable )

public:
    explicit Source(
            Object & parent,
            const char * ident,
            const QString & path,
            const QVariant & val = QVariant(),
            const QVariant & min = QVariant(),
            const QVariant & max = QVariant()
            ) :
        Linkable( parent, ident, path, true, val, min, max )
    {
    }
};

} // namespace Lib6
} // namespace LH

Q_DECLARE_INTERFACE( LH::Lib6::Source, "se.linkdata.LibSource/6.0" )

#endif // LH_LIB6_SOURCE_H
