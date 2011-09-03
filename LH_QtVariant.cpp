
#include "LH_QtVariant.h"
#include "LH_QtObject.h"

LH_QtVariant::LH_QtVariant( lh_variant& lhv, LH_QtObject *parent ) :
    QObject(parent),
    LH_Variant(),
    lhv_(lhv)
{
    memset(&lhv_,0,sizeof(lh_variant));
}

void LH_QtVariant::setFormat( lh_format fmt )
{
    if( lhv_.fmt == fmt ) return;
    read(); // make sure we are up-to-date
    lhv_.fmt = fmt;
    write(); // converts data if needed
    read();
}

void LH_QtVariant::read()
{
    LH_Variant::read(lhv_);
}

void LH_QtVariant::write() const
{
    LH_Variant::write(lhv_);
}
