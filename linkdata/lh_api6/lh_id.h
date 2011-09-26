#ifndef LH_ID_H
#define LH_ID_H

#include <QSharedData>
#include <QExplicitlySharedDataPointer>

class QObject;
class QString;

namespace lh_api6
{

class id_data : public QSharedData
{
public:
    virtual ~id_data() {}
    virtual void lh_new( QObject * child ) = 0;
    virtual void lh_delete( QObject * self ) = 0;
    virtual void lh_request_polling() const = 0;
    virtual void lh_request_render() const = 0;
    virtual void lh_request_reload( const QString & reason ) const = 0;
};

typedef QExplicitlySharedDataPointer<id_data> id_ptr;

} // namespace lh_api6

typedef lh_api6::id_ptr lh_id;

#endif // LH_ID_H
