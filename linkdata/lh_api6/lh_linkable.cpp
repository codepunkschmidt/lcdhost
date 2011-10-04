
#include "lh_linkable.h"
#include <QDebug>
#include <QThread>

using namespace lh::api6;

void linkable::setLinkOut( const QString & path )
{
    if( link_out_ != path )
    {
        link_out_ = path;
        emit linkOutChanged( link_out_ );
    }
}

void linkable::setLinkIn( const QString & path )
{
    if( link_in_ != path )
    {
        link_in_ = path;
        emit linkInChanged( link_in_ );
    }
}

void linkable::setLinkFilter( const QString & filter )
{
    if( link_filter_ != filter )
    {
        link_filter_ = filter;
        emit linkFilterChanged( link_filter_ );
    }
}

void linkable::setValue( const QVariant & v )
{
    Q_ASSERT( thread() == QThread::currentThread() );
    value_ = v;
}

void linkable::setMinimum( const QVariant & v )
{
    minimum_ = v;
}

void linkable::setMaximum( const QVariant & v )
{
    maximum_ = v;
}

