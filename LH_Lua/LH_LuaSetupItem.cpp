#include "LH_LuaSetupItem.h"

void LH_LuaSetupItem::setup_change()
{
    LH_QtSetupItem::setup_change();
    parent()->setup_change( this );
}
