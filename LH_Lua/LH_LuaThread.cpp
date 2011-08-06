#include "LH_LuaThread.h"
#include "LH_Lua.h"

void LH_LuaThread::run()
{
    lua_ = new LH_Lua();
    exec();
    delete lua_;
    lua_ = 0;
}
