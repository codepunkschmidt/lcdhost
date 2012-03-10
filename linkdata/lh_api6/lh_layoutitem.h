#ifndef LH_API6_LH_LAYOUTITEM_H
#define LH_API6_LH_LAYOUTITEM_H

#include "lh_object.h"
#include "lh_layoutclass.h"

namespace lh {
namespace api6 {

class layoutitem : public object
{
    Q_OBJECT
    Q_PROPERTY( QString title READ title WRITE setTitle STORED true )

    Q_INTERFACES( lh::api6::object )

    QString title_;

public:
    layoutitem( layoutclass & parent ) : object( parent ) {}

    layoutclass * parent() const { return static_cast<layoutclass *>(object::parent()); }
    const QString & title() const { return title_; }
    void setTitle( const QString & s ) { title_ = s; }
};

} // namespace api6
} // namespace lh

Q_DECLARE_INTERFACE( lh::api6::layoutitem, "se.linkdata.lh_layoutitem/6.0" )

#endif // LH_API6_LH_LAYOUTITEM_H
