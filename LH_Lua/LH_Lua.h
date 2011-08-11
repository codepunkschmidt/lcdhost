#ifndef LH_LUA_H
#define LH_LUA_H

#include <QObject>
#include <QFileInfo>

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

class LH_Lua : public QObject
{
    Q_OBJECT

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
