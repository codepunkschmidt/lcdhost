/**
  \file     LH_QtObject.cpp
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

#include "LH_QtObject.h"
#include "LH_QtSetupItem.h"

lh_callback_t LH_QtObject::cb_ = 0;
void *LH_QtObject::cb_id_ = 0;
LH_QtPlugin *LH_QtObject::plugin_ = 0;

static int compareSetupItems( const LH_QtSetupItem *a, const LH_QtSetupItem *b )
{
    return a->order() < b->order();
}

#define RECAST(obj) reinterpret_cast<LH_QtObject*>(obj)
static lh_setup_item **obj_setup_data( void *obj ) { return RECAST(obj)->setup_data(); }
static void obj_setup_resize( void *obj, lh_setup_item *item, size_t needed ) { RECAST(obj)->setup_resize(item,needed); }
static void obj_setup_change( void *obj, lh_setup_item *item ) { return RECAST(obj)->setup_change(item); }
static void obj_setup_input( void *obj, lh_setup_item *item, int flags, int value ) { return RECAST(obj)->setup_input(item,flags,value); }
static int obj_notify( void *obj, int code, void *param ) { return RECAST(obj)->notify(code,param); }
static int obj_polling( void *obj ) { return RECAST(obj)->polling(); }

void LH_QtObject::build_calltable( lh_object_calltable *ct )
{
    if( ct )
    {
        ct->size = sizeof(lh_object_calltable);
        ct->obj_setup_data = obj_setup_data;
        ct->obj_setup_resize = obj_setup_resize;
        ct->obj_setup_change = obj_setup_change;
        ct->obj_setup_input = obj_setup_input;
        ct->obj_notify = obj_notify;
        ct->obj_polling = obj_polling;
    }
    return;
}

lh_setup_item **LH_QtObject::setup_data()
{
    QList<LH_QtSetupItem*> list;
    for( QObjectList::const_iterator i = children().constBegin(); i != children().constEnd(); ++i )
    {
        LH_QtSetupItem *si = qobject_cast<LH_QtSetupItem *>(*i);
        if( si ) list.append(si);
    }
    qStableSort( list.begin(), list.end(), compareSetupItems );

    setup_item_vector_.clear();
    foreach( LH_QtSetupItem *si, list ) setup_item_vector_.append( si->item() );
    setup_item_vector_.append( NULL );

    return setup_item_vector_.data();
}

void LH_QtObject::setup_resize(lh_setup_item *item,size_t needed)
{
    for( QObjectList::const_iterator i = children().constBegin(); i != children().constEnd(); ++i )
    {
        LH_QtSetupItem *si = qobject_cast<LH_QtSetupItem *>(*i);
        if( si && si->item() == item )
        {
            si->setup_resize(needed);
            return;
        }
    }
    Q_ASSERT(0);
    return;
}

void LH_QtObject::setup_change(lh_setup_item *item)
{
    for( QObjectList::const_iterator i = children().constBegin(); i != children().constEnd(); ++i )
    {
        LH_QtSetupItem *si = qobject_cast<LH_QtSetupItem *>(*i);
        if( si && si->item() == item )
        {
            si->setup_change();
            return;
        }
    }
    Q_ASSERT(0);
    return;
}

void LH_QtObject::setup_input(lh_setup_item *item, int flags, int value)
{
    for( QObjectList::const_iterator i = children().constBegin(); i != children().constEnd(); ++i )
    {
        LH_QtSetupItem *si = qobject_cast<LH_QtSetupItem *>(*i);
        if( si && si->item() == item )
        {
            si->setup_input(flags,value);
            return;
        }
    }
    Q_ASSERT(0);
    return;
}

int LH_QtObject::notify( int code, void *param )
{
    Q_UNUSED(code);
    Q_UNUSED(param);
    return 0;
}

int LH_QtObject::polling()
{
    return 0;
}
