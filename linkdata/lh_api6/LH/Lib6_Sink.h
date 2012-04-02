#ifndef LH_LIB6_SINK_H
#define LH_LIB6_SINK_H

#include <LH/Lib6.h>
#include <LH/Lib6_Linkable.h>

namespace LH {
namespace Lib6 {

/**
  sink is a convenience class for creating
  a data sink that need no UI or to be saved.
  */
class Sink : public Linkable
{
    Q_OBJECT
    Q_INTERFACES( LH::Lib6::Object LH::Lib6::Linkable )

public:
    explicit Sink(
            Object & parent,
            const char * ident,
            const QString & path,
            const QVariant & val = QVariant(),
            const QVariant & min = QVariant(),
            const QVariant & max = QVariant()
            ) :
        Linkable( parent, ident, path, false, val, min, max )
    {}
};

} // namespace Lib6
} // namespace LH

Q_DECLARE_INTERFACE( LH::Lib6::Sink, "se.linkdata.lh_sink/6.0" )

#endif // LH_LIB6_SINK_H
