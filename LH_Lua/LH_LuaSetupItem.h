#ifndef LH_LUASETUPITEM_H
#define LH_LUASETUPITEM_H

#include "LH_LuaInstance.h"
#include "../LH_QtSetupItem.h"

class LH_LuaSetupItem : public LH_QtSetupItem
{
    Q_OBJECT

public:
    LH_LuaSetupItem( LH_LuaInstance *parent, const char *id );
    LH_LuaInstance *parent() const { return static_cast<LH_LuaInstance *>(LH_QtSetupItem::parent()); }
};

#endif // LH_LUASETUPITEM_H
