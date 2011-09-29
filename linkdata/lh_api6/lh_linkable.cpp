
#include "lh_linkable.h"
#include <QDebug>
#include <QThread>

using namespace lh::api6;

void linkable::setPush( const QString & path )
{

}

void linkable::setPull( const QString & path )
{

}

void linkable::setValue( const QVariant & v )
{
    Q_ASSERT( thread() == QThread::currentThread() );
    value_ = v;
}

void linkable::setMinimum( const QVariant & v )
{

}

void linkable::setMaximum( const QVariant & v )
{

}

