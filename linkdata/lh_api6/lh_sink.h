#ifndef LH_API6_LH_SINK_H
#define LH_API6_LH_SINK_H

#include "lh_linkable.h"

namespace lh {
namespace api6 {

/**
  sink is a convenience class for creating
  a data sink that need no UI or to be saved.
  */
class sink : public linkable
{
    Q_OBJECT
    Q_INTERFACES( lh::api6::object lh::api6::linkable )

    Q_PROPERTY( QString pull READ pull STORED false )
    Q_PROPERTY( QVariant value READ value WRITE setValue STORED false )
    Q_PROPERTY( QVariant minimum READ minimum WRITE setMinimum STORED false )
    Q_PROPERTY( QVariant maximum READ maximum WRITE setMaximum STORED false )

public:
    explicit sink(
            object & parent,
            const char * ident,
            const QString & path,
            const QVariant & val = QVariant(),
            const QVariant & min = QVariant(),
            const QVariant & max = QVariant()
            ) :
        linkable( parent, ident, path, false, val, min, max )
    {
    }
};

} // namespace api6
} // namespace lh

Q_DECLARE_INTERFACE( lh::api6::sink, "se.linkdata.lh_sink/6.0" )

#endif // LH_API6_LH_SINK_H
