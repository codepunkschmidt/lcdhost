/**
  \file     LH_QtPlugin_Lua.h
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

#ifndef LH_QTPLUGIN_LUA_H
#define LH_QTPLUGIN_LUA_H

#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QTime>
#include <QLibrary>

#include "../LH_QtPlugin.h"

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

class LH_QtPlugin_Lua : public LH_QtPlugin
{
    Q_OBJECT

    lua_State *L;
    // QFileSystemWatcher *watcher_;
    QString luadir_;
    QTime dirmodified_; // we need this to avoid races
    bool needscan_;

    void scanForFiles();
    void loadLuaFile( QFileInfo fi );

public:
    LH_QtPlugin_Lua() : LH_QtPlugin(), L(NULL), needscan_(false) {}

    const char * lh_name() { return "Lua"; }
    const char * lh_shortdesc() { return "Lua scripting"; }
    const char * lh_author() { return "Johan \"SirReal\" Lindh"; }
    const char * lh_homepage() { return "<a href=\"http://www.linkdata.se/software/lcdhost\">Link Data Stockholm</a>"; }
    const char * lh_longdesc()
    {
        return "";
    }

    const char *lh_load();
    int lh_notify( int code, void *);
    void lh_unload();

public slots:
    void directoryChanged(QString);
};

#endif // LH_QTPLUGIN_LUA_H
