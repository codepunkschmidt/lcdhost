#ifndef LH_LUATHREAD_H
#define LH_LUATHREAD_H

#include <QThread>

class LH_Lua;

class LH_LuaThread : public QThread
{
    Q_OBJECT
    LH_Lua *lua_;

public:
    explicit LH_LuaThread(QObject *parent = 0) : QThread(parent), lua_(0) {}
    void run();
};

#endif // LH_LUATHREAD_H
