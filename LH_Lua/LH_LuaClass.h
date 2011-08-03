/**
  \file     LH_LuaClass.h
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009-2011, Johan Lindh

  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of Link Data Stockholm nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
  */

#ifndef LH_LUACLASS_H
#define LH_LUACLASS_H

#include <QList>
#include <QString>
#include <QFileInfo>
#include "LH_QtPlugin_Lua.h"

#define LUA_MAGIC "wf7bFprtNhNGS6XKjmrc"

extern "C" {
#ifdef LUA_STATIC
# include "lua.h"
# include "lauxlib.h"
#else
# include "lua_dyn.h"
# define LUA_PREFIX LuaFunctions.
extern lua_All_functions LuaFunctions;
#endif
}

/**
  To link the Lua instances with the C++ instances, we'll use a Lua table
  containing a light userdata key and a reference to the Lua instance.
  A 'light userdata' in Lua is simply a pointer, copy-by-value and not subject
  to garbage collection. The table is stored in lcdhost.instance
  */

class LH_LuaClass : public LH_QtObject
{
    Q_OBJECT

    lua_State *L;
    QFileInfo fi_;
    QString filename_;
    QByteArray ident_array_;
    QByteArray name_array_;
    QByteArray path_array_;
    lh_layout_class classinfo_;

    LH_LuaClass( LH_QtPlugin_Lua *parent, QFileInfo fi, QString filename );

    static QList<LH_LuaClass*> list_;

public:
    ~LH_LuaClass();
    const char *userInit();

    LH_QtPlugin_Lua *parent() const { return static_cast<LH_QtPlugin_Lua *>(LH_QtObject::parent()); }
    lua_State *luaState() { return L; }
    lh_layout_class *classInfo() { return &classinfo_; }
    QString filename() const { return filename_; }

    QByteArray id() const { return ident_array_; }
    void lua_pushmodule();

    const QFileInfo& fileInfo() const { return fi_; }

    static QString load( LH_QtPlugin_Lua *parent, QFileInfo fi );
};

#endif // LH_LUACLASS_H
