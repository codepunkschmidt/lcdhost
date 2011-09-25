#ifndef LH_SETUP_H
#define LH_SETUP_H

#include <QStringList>
#include "lh_linkable.h"

/**
  Provides a linkable, saved lh_setup item with optional UI.
  */
class lh_setup : public lh_linkable
{
    Q_OBJECT

    Q_PROPERTY( lh_ui_type ui READ ui WRITE setUi STORED false )
    Q_PROPERTY( bool isStored READ isStored WRITE setStored STORED false )
    Q_PROPERTY( bool isEnabled READ isEnabled WRITE setEnabled STORED false )
    Q_PROPERTY( bool isVisible READ isVisible WRITE setVisible STORED false )
    Q_PROPERTY( int order READ order WRITE setOrder )
    Q_PROPERTY( QString push READ push WRITE setPush )
    Q_PROPERTY( QString pull READ pull WRITE setPull )
    Q_PROPERTY( QString help READ help WRITE setHelp )
    Q_PROPERTY( QVariant value READ value WRITE setValue )
    Q_PROPERTY( QVariant minimum READ minimum WRITE setMinimum )
    Q_PROPERTY( QVariant maximum READ maximum WRITE setMaximum )
    Q_PROPERTY( QStringList list READ list WRITE setList STORED false )

    int meta_;
    int order_;
    QString help_;
    QStringList list_;

public:
    explicit lh_setup( lh_object & parent,
                       const QString & ident,
                       const int meta,
                       const QVariant & val = QVariant(),
                       const QVariant & min = QVariant(),
                       const QVariant & max = QVariant()
            ) :
        lh_linkable( parent, ident, QString(), false, val, min, max ),
        meta_( meta ),
        order_( 0 )
    {
        if( meta_ & lh_ui_first ) -- order_;
        if( meta_ & lh_ui_last ) ++ order_;
    }

    int meta() const { return meta_; }
    lh_ui_type ui() const { return (lh_ui_type) (meta_ & lh_ui_type_mask); }
    int flags() const { return meta_ & lh_ui_flag_mask; }
    bool flag( const int f ) const { return (flags() & f) ? true : false; }
    bool isStored() const { return flag( lh_ui_stored ); }
    bool isEnabled() const { return flag( lh_ui_enabled ); }
    bool isVisible() const { return flag( lh_ui_visible ); }
    int order() const { return order_; }
    const QString & help() const { return help_; }
    const QStringList & list() const { return list_; }

    void setUi( lh_ui_type ui ) { setMeta( (meta_ & ~lh_ui_type_mask) | ui ); }
    void setFlag( const int f, const bool b ) { setMeta( (meta_&~f) | (b?f:0) ); }
    void setStored( const bool b ) { setFlag( lh_ui_stored, b ); }
    void setEnabled( const bool b ) { setFlag( lh_ui_enabled, b ); }
    void setVisible( const bool b ) { setFlag( lh_ui_visible, b ); }

signals:
    void metaChanged( const int );

public slots:
    void setMeta( int meta );
    void setOrder( int order );
    void setHelp( const QString & help );
    void setList( const QStringList & sl );
};

#endif // LH_SETUP_H
