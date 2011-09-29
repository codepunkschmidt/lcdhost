#ifndef LH_API6_LH_EVENT_H
#define LH_API6_LH_EVENT_H

#include <QEvent>
#include <QString>
#include <QVariant>
#include "lh_api6/lh_id.h"

namespace lh {
namespace api6 {
namespace event {

/*
  Registering event types using QEvent::registerEventType()
  doesn't match up across dynamic library boundaries.
  */
typedef enum
{
    first_type = QEvent::MaxUser - (QEvent::User*2),
    type_init,
    type_setproperty,
    last_type
} type;

const char * name( type t );

class init : public QEvent
{
    id_ptr id_;
    QString childname_;
public:
    static const Type type = (Type) type_init;
    init( const id_ptr & id, const QString & childname = QString() ) :
        QEvent( type ),
        id_( id ),
        childname_( childname )
    {}
    const id_ptr & id() const { return id_; }
    const QString & childName() const { return childname_; }
};

class setproperty : public QEvent
{
    const char *name_;
    QVariant value_;
public:
    static const Type type = (Type) type_setproperty;
    setproperty( const char * name, const QVariant & value ) :
        QEvent( type ),
        name_( name ),
        value_( value )
    {}
    const char * name() const { return name_; }
    const QVariant & value() const { return value_; }
};

} // namespace event
} // namespace api6
} // namespace lh

#endif // LH_API6_LH_EVENT_H
