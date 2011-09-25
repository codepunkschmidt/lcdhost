#ifndef LH_LINKABLE_H
#define LH_LINKABLE_H

#include "lh_object.h"

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

    QString push_;
    QString pull_;
    QVariant value_;
    QVariant minimum_;
    QVariant maximum_;

public:
    lh_linkable( lh_object & parent,
                 const QString & ident,
                 const QString & path = QString(),
                 bool is_source = false,
                 const QVariant & val = QVariant(),
                 const QVariant & min = QVariant(),
                 const QVariant & max = QVariant()
                 ) :
        lh_object( ident, & parent ),
        push_( is_source ? path : QString() ),
        pull_( is_source ? QString() : path ),
        value_( val ),
        minimum_( min ),
        maximum_( max )
    {
    }

    const QString & push() const { return push_; }
    const QString & pull() const { return pull_; }
    const QVariant & value() const { return value_; }
    const QVariant & minimum() const { return minimum_; }
    const QVariant & maximum() const { return maximum_; }
    bool isSource() const { return !push_.isEmpty(); }
    bool isSink() const { return !pull_.isEmpty(); }

signals:
    void pushChanged( const QString & );
    void pullChanged( const QString & );
    void valueChanged( const QVariant & );
    void minimumChanged( const QVariant & );
    void maximumChanged( const QVariant & );

public slots:
    void setPush( const QString & path = QString() );
    void setPull( const QString & path = QString() );
    void setValue( const QVariant & v );
    void setMinimum( const QVariant & v );
    void setMaximum( const QVariant & v );
};

#endif // LH_LINKABLE_H
