#ifndef LH_LINKABLE_H
#define LH_LINKABLE_H

#include "lh_object.h"

class lh_val : public QVariant
{
public: lh_val( const QVariant & v ) : QVariant( v ) { }
};

class lh_min : public QVariant
{
public: lh_min( const QVariant & v ) : QVariant( v ) { }
};

class lh_max : public QVariant
{
public: lh_max( const QVariant & v ) : QVariant( v ) { }
};

/**
  lh_linkable provides the basic data linking
  functionality, and storage for the value, minimum and
  maximum properties. Those three are the ones transferred
  when data linking. Note that this object doesn't save
  itself between sessions, nor does it provide an UI.
  */
class lh_linkable : public lh_object
{
    Q_OBJECT

    bool is_source_;
    QString link_;
    QVariant value_;
    QVariant minimum_;
    QVariant maximum_;

public:
    lh_linkable( const QString & ident, lh_object * parent = 0 ) :
        lh_object( ident, parent ),
        is_source_( false )
    {}

    lh_linkable( const QString & ident, const QString & path, bool is_source, lh_object * parent = 0 ) :
        lh_object( ident, parent ),
        is_source_( is_source ),
        link_( path )
    {}

    const QVariant & value() const { return value_; }
    const QVariant & minimum() const { return minimum_; }
    const QVariant & maximum() const { return maximum_; }
    const QString & link() const { return link_; }
    bool isSource() const { return !link_.isEmpty() && is_source_; }
    bool isSink() const { return !link_.isEmpty() && !is_source_; }

public slots:
    void setValue( const QVariant & v );
    void setMinimum( const QVariant & v );
    void setMaximum( const QVariant & v );
    void setLink( const QString & path, bool is_source = false );
    void setSource( bool b ) { setLink( link_, b ); }
    void setSource( const QString & path ) { setLink( path, true ); }
    void setSink( const QString & path ) { setLink( path, false ); }
};

#endif // LH_LINKABLE_H
