#include "lh_setup.h"

using namespace lh_api6;

void setup::setMeta( int meta )
{
    if( meta_ == meta ) return;
    meta_ = meta;
    emit metaChanged( meta_ );
}

void setup::setOrder( int order )
{

}

void setup::setHelp( const QString & help )
{

}

void setup::setList( const QStringList & sl )
{

}
