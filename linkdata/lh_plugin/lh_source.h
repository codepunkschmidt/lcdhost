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

    Q_PROPERTY( QString push READ push STORED false )
    Q_PROPERTY( QVariant value READ value WRITE setValue STORED false )
    Q_PROPERTY( QVariant minimum READ minimum WRITE setMinimum STORED false )
    Q_PROPERTY( QVariant maximum READ maximum WRITE setMaximum STORED false )

public:
    explicit lh_source( lh_object & parent,
               const QString & ident,
               const QString & path,
               const QVariant & val = QVariant(),
               const QVariant & min = QVariant(),
               const QVariant & max = QVariant() ) :
        lh_linkable( parent, ident, path, true, val, min, max )
    {
    }

signals:

public slots:

};

#endif // LH_SOURCE_H
