/**
  \file     LH_LuaInstance.h
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

#ifndef LH_LUAINSTANCE_H
#define LH_LUAINSTANCE_H

#include <QImage>
#include <QStack>

#include "../LH_QtPlugin.h"
#include "../LH_QtInstance.h"
#include "LH_LuaVariant.h"
#include "LH_LuaClass.h"

class LH_QtSetupItem;
class LH_LuaSetupItem;

class LH_LuaInstance : public LH_QtInstance
{
    Q_OBJECT

    static QStack<LH_LuaInstance*> *stack_;

    lua_State *L;
    lh_blob *blob_;
    int ref_; // reference to 'self' in the registry

public:
    LH_LuaInstance( LH_LuaClass *alc );
    ~LH_LuaInstance();

    LH_LuaClass *parent() const { return static_cast<LH_LuaClass *>(LH_QtInstance::parent()); }

    void lua_pushself() { lua_rawgeti(L, LUA_REGISTRYINDEX, ref_); }
    bool lua_pushfunction(const char *funcname);

    const char *userInit();
    int polling();
    int notify( int, void* );

    void prerender();
    int width( int );
    int height( int );
    lh_blob *render_blob( int, int );
    QImage *render_qimage( int, int );

    static void push( LH_LuaInstance *inst ) { if( stack_ == 0 ) stack_ = new QStack<LH_LuaInstance*>(); stack_->push(inst); }
    static LH_LuaInstance *top() { return ( stack_ ? stack_->top() : 0); }
    static void pop() { if( stack_ ) stack_->pop(); }

public slots:
    void valueChanged( const LH_QtSetupItem& );
    void minimumChanged( const LH_QtSetupItem& );
    void maximumChanged( const LH_QtSetupItem& );
    void otherChanged( const LH_QtSetupItem& );
};

#endif // LH_LUAINSTANCE_H
