#ifndef LH_API6_LH_ID_H
#define LH_API6_LH_ID_H

#include <QSharedData>
#include <QExplicitlySharedDataPointer>

namespace lh {
namespace api6 {

class object;

class id_data : public QSharedData
{
public:
    virtual ~id_data() {}
    virtual void lh_new( object & ) = 0;
    virtual void lh_delete() = 0;
    virtual void lh_bind( object & ) = 0;
    virtual void lh_init_result( const QString & ) = 0;
    virtual void lh_request_polling() const = 0;
    virtual void lh_request_render() const = 0;
    virtual void lh_request_reload( const QString & reason ) const = 0;
};

typedef QExplicitlySharedDataPointer<id_data> id_ptr;

} // namespace api6
} // namespace lh

#endif // LH_API6_LH_ID_H
