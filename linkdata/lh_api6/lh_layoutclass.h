#ifndef LH_API6_LH_LAYOUTCLASS_H
#define LH_API6_LH_LAYOUTCLASS_H

#include "lh_object.h"

namespace lh {
namespace api6 {

class layoutitem;

class layoutclass : public object
{
    Q_OBJECT
    Q_PROPERTY( QString title READ title STORED false )
    Q_PROPERTY( QString path READ path STORED false )

    Q_INTERFACES( lh::api6::object )

    QString title_;
    QString path_;

public:
    layoutclass( object & parent,
                 const QString & objectname,
                 const QString & title = QString(),
                 const QString & path = QString() ) :
        object( parent, objectname ),
        title_( title ),
        path_( path )
    {}

    const QString & title() const { return title_; }
    const QString & path() const { return path_; }

    virtual layoutitem * create() { return 0; }
    virtual void destroy( layoutitem * );
};

} // namespace api6
} // namespace lh

Q_DECLARE_INTERFACE( lh::api6::layoutclass, "se.linkdata.lh_layoutclass/6.0" )

#endif // LH_API6_LH_LAYOUTCLASS_H
