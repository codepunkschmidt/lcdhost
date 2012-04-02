#ifndef LH_LIB6_LAYOUTITEM_H
#define LH_LIB6_LAYOUTITEM_H

#include <LH/Lib6.h>
#include <LH/Lib6_Object.h>
#include <LH/Lib6_LayoutClass.h>

namespace LH {
namespace Lib6 {

class LayoutItem : public Object
{
    Q_OBJECT
    Q_PROPERTY( QString title READ title WRITE setTitle STORED true )

    Q_INTERFACES( LH::Lib6::Object )

    QString title_;

public:
    LayoutItem( LayoutClass & parent ) : Object( parent ) {}

    LayoutClass * parent() const { return static_cast<LayoutClass *>(Object::parent()); }
    const QString & title() const { return title_; }
    void setTitle( const QString & s ) { title_ = s; }
};

} // namespace Lib6
} // namespace LH

Q_DECLARE_INTERFACE( LH::Lib6::LayoutItem, "se.linkdata.lh_layoutitem/6.0" )

#endif // LH_LIB6_LAYOUTITEM_H
