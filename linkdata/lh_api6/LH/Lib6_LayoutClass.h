#ifndef LH_LIB6_LAYOUTCLASS_H
#define LH_LIB6_LAYOUTCLASS_H

#include <LH/Lib6.h>
#include <LH/Lib6_Object.h>

namespace LH {
namespace Lib6 {

class LayoutClass : public Object
{
    Q_OBJECT
    Q_PROPERTY( QString title READ title STORED false )
    Q_PROPERTY( QString path READ path STORED false )

    Q_INTERFACES( LH::Lib6::Object )

    QString title_;
    QString path_;

public:
    LayoutClass( Object & parent,
                 const QString & objectname,
                 const QString & title = QString(),
                 const QString & path = QString() ) :
        Object( parent, objectname ),
        title_( title ),
        path_( path )
    {}

    const QString & title() const { return title_; }
    const QString & path() const { return path_; }

    virtual LayoutItem * create() { return 0; }
    virtual void destroy( LayoutItem * );
};

} // namespace Lib6
} // namespace LH

Q_DECLARE_INTERFACE( LH::Lib6::LayoutClass, "se.linkdata.lh_layoutclass/6.0" )

#endif // LH_LIB6_LAYOUTCLASS_H
