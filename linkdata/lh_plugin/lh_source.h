#ifndef LH_SOURCE_H
#define LH_SOURCE_H

#include "lh_linkable.h"

/**
  lh_source is a convenience class for creating
  a data source that need no UI or to be saved.
  */
class lh_source : public lh_linkable
{
    Q_OBJECT

    Q_PROPERTY( QVariant value READ value WRITE setValue STORED false )
    Q_PROPERTY( QVariant minimum READ minimum WRITE setMinimum STORED false )
    Q_PROPERTY( QVariant maximum READ maximum WRITE setMaximum STORED false )
    Q_PROPERTY( QString link READ link STORED false )
    Q_PROPERTY( bool isSource READ isSource STORED false )
    Q_PROPERTY( bool isSink READ isSink STORED false )

public:
    lh_source( const QString & ident, const QVariant & val, const QString & path, lh_object * parent = 0 ) :
        lh_linkable( ident, path, true, parent )
    {
        setValue( val );
    }

    lh_source( const QString & ident, const QVariant & val, const QVariant & min, const QVariant & max, const QString & path, lh_object * parent = 0 ) :
        lh_linkable( ident, path, true, parent )
    {
        setValue( val );
        setMinimum( min );
        setMaximum( max );
    }

signals:

public slots:

};

#endif // LH_SOURCE_H
