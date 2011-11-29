#ifndef LH_API6_LH_ID_H
#define LH_API6_LH_ID_H

#include <QSharedData>
#include <QExplicitlySharedDataPointer>

class QString;
class QVariant;

namespace lh {
namespace api6 {

class object;

class id_data : public QSharedData
{
public:
    virtual ~id_data() {}
    virtual QString toString() = 0;

    virtual void lh_create_child( object & new_child ) = 0;
    virtual void lh_destroy() = 0;
    virtual bool lh_bind( object & self ) = 0;
    virtual bool lh_set_property( const char * name, const QVariant & value ) = 0;
    virtual bool lh_init_result( bool ok ) = 0;
    virtual void lh_ping_reply() = 0;
    virtual void lh_request_polling() const = 0;
    virtual void lh_request_render() const = 0;
    virtual void lh_request_reload( const QString & reason ) const = 0;
};

typedef QExplicitlySharedDataPointer<id_data> id_ptr;

} // namespace api6
} // namespace lh

#endif // LH_API6_LH_ID_H
