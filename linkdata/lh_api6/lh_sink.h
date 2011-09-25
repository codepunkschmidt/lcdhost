#ifndef LH_SINK_H
#define LH_SINK_H

#include "lh_linkable.h"

namespace lh_api6
{

/**
  sink is a convenience class for creating
  a data sink that need no UI or to be saved.
  */
class sink : public linkable
{
    Q_OBJECT

    Q_PROPERTY( QString pull READ pull STORED false )
    Q_PROPERTY( QVariant value READ value WRITE setValue STORED false )
    Q_PROPERTY( QVariant minimum READ minimum WRITE setMinimum STORED false )
    Q_PROPERTY( QVariant maximum READ maximum WRITE setMaximum STORED false )

public:
    explicit sink(
            object & parent,
            const QString & ident,
            const QString & path,
            const QVariant & val = QVariant(),
            const QVariant & min = QVariant(),
            const QVariant & max = QVariant()
            ) :
        linkable( parent, ident, path, false, val, min, max )
    {
    }
};

} // namespace lh_api6

typedef lh_api6::sink lh_sink;

#endif // LH_SINK_H
