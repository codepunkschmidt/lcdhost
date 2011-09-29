#ifndef LH_API6_LH_LINKABLE_H
#define LH_API6_LH_LINKABLE_H

#include <QVariant>
#include <QString>
#include "lh_api6/lh_object.h"

namespace lh {
namespace api6 {

/**
  linkable provides the basic data linking
  functionality, and storage for the value, minimum and
  maximum properties. Those three are the ones transferred
  when data linking. Note that this object doesn't save
  itself between sessions, nor does it provide an UI.
  */
class linkable : public object
{
    Q_OBJECT
    Q_INTERFACES( lh::api6::object )

    QString push_;
    QString pull_;
    QVariant value_;
    QVariant minimum_;
    QVariant maximum_;

public:
    linkable( object & parent,
              const char * ident,
              const QString & path = QString(),
              bool is_source = false,
              const QVariant & val = QVariant(),
              const QVariant & min = QVariant(),
              const QVariant & max = QVariant()
              ) :
        object( parent, ident ),
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

} // namespace api6
} // namespace lh

Q_DECLARE_INTERFACE( lh::api6::linkable, "se.linkdata.lh_linkable/6.0" )

#endif // LH_API6_LH_LINKABLE_H
