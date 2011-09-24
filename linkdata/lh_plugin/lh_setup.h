#ifndef LH_SETUP_H
#define LH_SETUP_H

#include "lh_linkable.h"

/**
  Provides a linkable, saved lh_setup item with optional UI.
  */
class lh_setup : public lh_linkable
{
    Q_OBJECT
    Q_PROPERTY( QVariant value READ value WRITE setValue )
    Q_PROPERTY( QVariant minimum READ minimum WRITE setMinimum )
    Q_PROPERTY( QVariant maximum READ maximum WRITE setMaximum )
    Q_PROPERTY( QString link READ link WRITE setLink )
    Q_PROPERTY( bool isSource READ isSource WRITE setSource )
    Q_PROPERTY( bool isSink READ isSink STORED false )
    Q_PROPERTY( QVariantList list READ list WRITE setList STORED false )

    lh_userinterface ui_;
    QVariantList list_;

public:
    explicit lh_setup( const QString & ident, lh_userinterface ui = lh_ui_none, lh_object *parent = 0 ) :
        lh_linkable( ident, parent ),
        ui_( ui )
    { }

    explicit lh_setup( const QString & ident, const lh_val & val, lh_userinterface ui = lh_ui_none, lh_object *parent = 0 ) :
        lh_linkable( ident, parent ),
        ui_( ui )
    {
        setValue( val );
    }

    explicit lh_setup( const QString & ident, const lh_val & val, const lh_min & min, const lh_max & max, lh_userinterface ui = lh_ui_none, lh_object *parent = 0 ) :
        lh_linkable( ident, parent ),
        ui_( ui )
    {
        setValue( val );
        setMinimum( min );
        setMaximum( max );
    }

    const lh_userinterface ui() const { return ui_; }
    const QVariantList & list() const { return list_; }

public slots:
    void setList( const QVariantList & vl );
};

#endif // LH_SETUP_H
