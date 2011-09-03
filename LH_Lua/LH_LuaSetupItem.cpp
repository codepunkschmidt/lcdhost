#include "LH_LuaSetupItem.h"

LH_LuaSetupItem::LH_LuaSetupItem( LH_LuaInstance *parent, const char *id )
    : LH_QtSetupItem( parent, id, lh_type_none, lh_meta_default )
{
    connect( this, SIGNAL(valueChanged(LH_QtSetupItem)), parent, SLOT(valueChanged(LH_QtSetupItem)) );
    connect( this, SIGNAL(minimumChanged(LH_QtSetupItem)), parent, SLOT(minimumChanged(LH_QtSetupItem)) );
    connect( this, SIGNAL(maximumChanged(LH_QtSetupItem)), parent, SLOT(maximumChanged(LH_QtSetupItem)) );
    connect( this, SIGNAL(otherChanged(LH_QtSetupItem)), parent, SLOT(otherChanged(LH_QtSetupItem)) );
}
