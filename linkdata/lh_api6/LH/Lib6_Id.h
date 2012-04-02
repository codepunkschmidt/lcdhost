#ifndef LH_LIB6_ID_H
#define LH_LIB6_ID_H

#include <LH/Lib6.h>
#include <QSharedData>
#include <QExplicitlySharedDataPointer>

class QString;
class QVariant;

namespace LH {
namespace Lib6 {

class IdData : public QSharedData
{
public:
    virtual ~IdData() {}
    virtual QString toString() = 0;

    virtual void lh_create_child( Object & new_child ) = 0;
    virtual void lh_destroy() = 0;
    virtual bool lh_bind( Object & self ) = 0;
    virtual bool lh_set_property( const char * name, const QVariant & value ) = 0;
    virtual bool lh_init_result( bool ok ) = 0;
    virtual void lh_ping_reply() = 0;
    virtual void lh_request_polling() const = 0;
    virtual void lh_request_render() const = 0;
    virtual void lh_request_reload( const QString & reason ) const = 0;
};

} // namespace Lib6
} // namespace LH

#endif // LH_LIB6_ID_H
