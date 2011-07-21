/**
  \file     LH_QtSetupItem.cpp
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

#include <QDebug>
#include "LH_QtSetupItem.h"

LH_QtSetupItem::LH_QtSetupItem( LH_QtObject *parent, QString id, lh_setup_type type, int flags ) : QObject( parent )
{
    Q_ASSERT( parent != NULL );
#ifndef QT_NO_DEBUG
    if( !parent->isValid() )
        qWarning() << parent->metaObject()->className() << parent->objectName() << "creates setup items before init()";
#endif
    memset( &item_, 0, sizeof(item_) );
    item_.size = sizeof(item_);
    if( flags & LH_FLAG_FIRST ) item_.order = -1;
    if( flags & LH_FLAG_LAST ) item_.order = 1;
    item_.type = type;
    item_.flags = flags;
    setId(id);
}

void LH_QtSetupItem::setup_change()
{
    if( type() == lh_type_string_inputstate || type() == lh_type_string_inputvalue )
    {
        emit input( QString(), item_.param.input.flags, item_.param.input.value );
    }
    else
    {
        emit changed();
    }
    if( item_.flags & LH_FLAG_AUTORENDER ) parent()->requestRender();
    return;
}

void LH_QtSetupItem::setFlag( int f, bool state )
{
    if( state )
    {
        if( (item_.flags & f) == f ) return;
        item_.flags |= f;
    }
    else
    {
        if( !(item_.flags & f) ) return;
        item_.flags &= ~f;
    }
    parent()->callback(lh_cb_setup_refresh, item() );
    return;
}

void LH_QtSetupItem::setOrder( int n )
{
    if( item_.order != n )
    {
        item_.order = n;
        parent()->callback(lh_cb_setup_refresh, item() );
    }
}

void LH_QtSetupItem::setId( QString s )
{
    Q_ASSERT( !s.isEmpty() );
    if( title_array_.isNull() )
    {
        title_array_ = s.toUtf8();
        item_.title = title_array_.data();
        if( title_array_.startsWith('~') )
        {
            title_array_.remove(0,1);
            item_.flags |= LH_FLAG_HIDETITLE;
        }
        if( title_array_.startsWith('^') )
        {
            title_array_.remove(0,1);
            item_.flags |= LH_FLAG_BLANKTITLE;
        }
    }
    if( s.contains('/') )
    {
        qWarning() << parent()->objectName() << "setup item ID contains slashes:" << s;
        s.replace('/','\\');
    }
    setObjectName(s);
    id_array_ = s.toAscii();
    item_.id = id_array_.data();
    return;
}

void LH_QtSetupItem::setTitle(QString s)
{
    if( s.isEmpty() )
    {
        title_array_.clear();
        item_.title = 0;
    }
    else
    {
        title_array_ = s.toUtf8();
        item_.title = title_array_.data();
    }
    parent()->callback( lh_cb_setup_refresh, item() );
    return;
}

void LH_QtSetupItem::setLink(QString s)
{
    if( s.isEmpty() )
    {
        link_array_.clear();
        item_.link = 0;
    }
    else
    {
        link_array_ = s.toUtf8();
        item_.link = link_array_.data();
    }
    parent()->callback( lh_cb_setup_refresh, item() );
    return;
}

void LH_QtSetupItem::setHelp(QString s)
{
    if( s.isEmpty() )
    {
        help_array_.clear();
        item_.help = 0;
    }
    else
    {
        help_array_ = s.toUtf8();
        item_.help = help_array_.data();
    }
    parent()->callback( lh_cb_setup_refresh, item() );
    return;
}

