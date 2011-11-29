#ifndef LH_API6_LH_WIDGET_H
#define LH_API6_LH_WIDGET_H

#include "lh_object.h"

namespace lh {
namespace api6 {

class item;

class widget : public object
{
    Q_OBJECT
    Q_PROPERTY( QString title READ title STORED false )
    Q_PROPERTY( QString path READ path STORED false )

    Q_INTERFACES( lh::api6::object )

    QString title_;
    QString path_;

public:
    widget( object & parent ) : object( parent ) {}

    QString title() const { return title_; }
    QString path() const { return path_; }

    virtual item * create() { return 0; }
    virtual void destroy( item * );
};

} // namespace api6
} // namespace lh

Q_DECLARE_INTERFACE( lh::api6::widget, "se.linkdata.lh_widget/6.0" )

#endif // LH_API6_LH_WIDGET_H
