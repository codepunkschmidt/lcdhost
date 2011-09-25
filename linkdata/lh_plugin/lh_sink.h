#ifndef LH_SINK_H
#define LH_SINK_H

#include "lh_linkable.h"

/**
  lh_sink is a convenience class for creating
  a data sink that need no UI or to be saved.
  */
class lh_sink : public lh_linkable
{
    Q_OBJECT

    Q_PROPERTY( QString pull READ pull STORED false )
    Q_PROPERTY( QVariant value READ value WRITE setValue STORED false )
    Q_PROPERTY( QVariant minimum READ minimum WRITE setMinimum STORED false )
    Q_PROPERTY( QVariant maximum READ maximum WRITE setMaximum STORED false )

public:
    explicit lh_sink( lh_object & parent,
             const QString & ident,
             const QString & path,
             const QVariant & val = QVariant(),
             const QVariant & min = QVariant(),
             const QVariant & max = QVariant()
            ) :
        lh_linkable( parent, ident, path, false, val, min, max )
    {
    }
};

#endif // LH_SINK_H
