#include <LH/Lib6_Linkable.h>

#include <QDebug>
#include <QThread>

namespace LH {
namespace Lib6 {

void Linkable::setLinkOut( const QString & path )
{
    if( link_out_ != path )
    {
        link_out_ = path;
        emit linkOutChanged( link_out_ );
    }
}

void Linkable::setLinkIn( const QString & path )
{
    if( link_in_ != path )
    {
        link_in_ = path;
        emit linkInChanged( link_in_ );
    }
}

void Linkable::setLinkFilter( const QString & filter )
{
    if( link_filter_ != filter )
    {
        link_filter_ = filter;
        emit linkFilterChanged( link_filter_ );
    }
}

void Linkable::setValue( const QVariant & v )
{
    Q_ASSERT( thread() == QThread::currentThread() );
    value_ = v;
}

void Linkable::setMinimum( const QVariant & v )
{
    minimum_ = v;
}

void Linkable::setMaximum( const QVariant & v )
{
    maximum_ = v;
}

} // namespace Lib6
} // namespace LH
