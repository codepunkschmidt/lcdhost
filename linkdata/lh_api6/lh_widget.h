#ifndef LH_API6_LH_WIDGET_H
#define LH_API6_LH_WIDGET_H

#include "lh_object.h"

namespace lh {
namespace api6 {

class widget : public object
{
    Q_OBJECT

public:
    widget( object & parent ) : object( parent ) {}

};

} // namespace api6
} // namespace lh

Q_DECLARE_INTERFACE( lh::api6::widget, "se.linkdata.lh_widget/6.0" )

#endif // LH_API6_LH_WIDGET_H
