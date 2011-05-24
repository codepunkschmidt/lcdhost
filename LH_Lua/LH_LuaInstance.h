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
#include "LH_LuaClass.h"


class LH_LuaInstance;

class lua_setup_item
{
    LH_LuaInstance *parent_;
    int order_;
    QByteArray key_;
    QByteArray name_;
    QByteArray help_;
    QByteArray data_;
    QByteArray paramlist_;
    lh_setup_item item_;

public:
    lua_setup_item(LH_LuaInstance *parent, const char *key) : parent_(parent), order_(0), key_(key)
    {
        memset(&item_,0,sizeof(item_));
    }

    LH_LuaInstance *parent() { return parent_; }
    int order() const { return order_; }
    void setOrder(int n) { order_ = n; }
    const char* key() const { return key_.constData(); }
    lh_setup_item* item() { return &item_; }
    QByteArray& name() { return name_; }
    QByteArray& help() { return help_; }
    QByteArray& data() { return data_; }
    QByteArray& paramlist() { return paramlist_; }

    void setup_resize(size_t needed)
    {
        data_.resize(needed);
        item_.param.size = data_.capacity();
        item_.data.s = data_.data();
        return;
    }
};


class LH_LuaInstance : public LH_QtInstance
{
    Q_OBJECT

    static QStack<LH_LuaInstance*> *stack_;

    const lh_class *cls_;
    lua_State *L;
    LH_LuaClass *alc_;
    lh_blob *blob_;
    int ref_; // reference to 'self' in the registry
    QList<lua_setup_item*> items_;
    QVector<lh_setup_item*> setup_item_vector_;

public:
    LH_LuaInstance( const lh_class *cls, QObject *parent = 0) : LH_QtInstance( parent ), cls_(cls), L(0), alc_(0), blob_(0), ref_(LUA_NOREF) {}

    virtual const char *init(const char *name, const lh_systemstate *state);
    virtual void term();

    void lua_pushself() { lua_rawgeti(L, LUA_REGISTRYINDEX, ref_); }
    bool lua_pushfunction(const char *funcname);

    lh_setup_item **setup_data();
    void setup_resize( lh_setup_item *item, size_t needed );
    void setup_change( lh_setup_item *item );
    void setup_input( lh_setup_item *item, int flags, int value );

    int polling();
    int notify( int, void* );
    void prerender();
    int width( int );
    int height( int );
    lh_blob *render_blob( int, int );
    QImage *render_qimage( int, int );
    QList<lua_setup_item*>& items() { return items_; }

    void update_setup_item_vector();

    static void push( LH_LuaInstance *inst ) { if( stack_ == 0 ) stack_ = new QStack<LH_LuaInstance*>(); stack_->push(inst); }
    static LH_LuaInstance *top() { return ( stack_ ? stack_->top() : 0); }
    static void pop() { if( stack_ ) stack_->pop(); }
};

#endif // LH_LUAINSTANCE_H
