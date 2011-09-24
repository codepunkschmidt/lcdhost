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

    Q_PROPERTY( QVariant value READ value WRITE setValue STORED false )
    Q_PROPERTY( QVariant minimum READ minimum WRITE setMinimum STORED false )
    Q_PROPERTY( QVariant maximum READ maximum WRITE setMaximum STORED false )
    Q_PROPERTY( QString link READ link STORED false )
    Q_PROPERTY( bool isSource READ isSource STORED false )
    Q_PROPERTY( bool isSink READ isSink STORED false )

public:
    lh_sink( const QString & ident, lh_object * parent = 0 ) :
        lh_linkable( ident, parent )
    {}

    lh_sink( const QString & ident, const QString & path, lh_object * parent = 0 ) :
        lh_linkable( ident, path, false, parent )
    {}
};

#endif // LH_SINK_H
