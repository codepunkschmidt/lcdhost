#include "lh_setup.h"

void lh_setup::setMeta( int meta )
{
    if( meta_ == meta ) return;
    meta_ = meta;
    emit metaChanged( meta_ );
}

void lh_setup::setOrder( int order )
{

}

void lh_setup::setHelp( const QString & help )
{

}

void lh_setup::setList( const QStringList & sl )
{

}
