#ifndef LH_LUA_H
#define LH_LUA_H

#include <QObject>
#include <QFileInfo>
#include "../LH_QtObject.h"

#define LUADIR "lua"

extern "C" {
#ifdef LUA_STATIC
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
#else
# include "lua_dyn.h"
# define LUA_PREFIX LuaFunctions.
#endif
}

class LH_Lua : public LH_QtObject
{
    Q_OBJECT

    lh_object obj_;
    lua_State *L;
    QString luadir_;

    void scanForFiles();
    void loadLuaFile( QFileInfo fi );

public:
    explicit LH_Lua(QObject *parent = 0);
    ~LH_Lua();

    lua_State *luaState() { return L; }
};

#endif // LH_LUA_H
