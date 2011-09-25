#ifndef LH_SOURCE_H
#define LH_SOURCE_H

#include "lh_linkable.h"


namespace lh_api6
{

/**
  source is a convenience class for creating
  a data source that need no UI or to be saved.
  */
class source : public linkable
{
    Q_OBJECT

    Q_PROPERTY( QString push READ push STORED false )
    Q_PROPERTY( QVariant value READ value WRITE setValue STORED false )
    Q_PROPERTY( QVariant minimum READ minimum WRITE setMinimum STORED false )
    Q_PROPERTY( QVariant maximum READ maximum WRITE setMaximum STORED false )

public:
    explicit source(
            object & parent,
            const QString & ident,
            const QString & path,
            const QVariant & val = QVariant(),
            const QVariant & min = QVariant(),
            const QVariant & max = QVariant()
            ) :
        linkable( parent, ident, path, true, val, min, max )
    {
    }
};

} // namespace lh_api6

typedef lh_api6::source lh_source;

#endif // LH_SOURCE_H
