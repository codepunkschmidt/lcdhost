/**
  \file     LH_Qt_QStringList.h
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

#ifndef LH_QT_QSTRINGLIST_H
#define LH_QT_QSTRINGLIST_H

#include <QStringList>
#include "LH_QtSetupItem.h"

/*
  Supports the following setup types:
    lh_type_integer_list
    lh_type_integer_listbox
    lh_type_string_combobox
  */

class LH_Qt_QStringList : public LH_QtSetupItem
{
    QStringList list_;

public:
    LH_Qt_QStringList( LH_QtObject *parent, const char *ident, const QStringList& list, int flags = 0, lh_setup_type subtype = lh_type_integer_list  )
        : LH_QtSetupItem( parent, ident, subtype, flags )
    {
        list_ = list;
        setList( list_.join("\n").toUtf8() );
    }
    
    QStringList& list()
    {
        return list_;
    }

    void refreshList()
    {
        setList( list_.join("\n").toUtf8() );
        refreshMeta();
    }

    int value() const
    {
        if( type() & lh_type_integer )
            return item_.data.i;
        if( type() & lh_type_string )
            return list_.indexOf( str_ );
        return -1;
    }

    QString valueText() const
    {
        if( type() & lh_type_integer )
        {
            if( item_.data.i >= 0 && item_.data.i < list_.size() )
                return list_.at( item_.data.i );
        }
        if( type() & lh_type_string )
            return str_;
        return QString();
    }

    void setValue(const QString& s)
    {
        if( type() & lh_type_integer )
        {
            LH_QtSetupItem::setValue( list_.indexOf(s) );
            return;
        }
        if( type() & lh_type_string )
        {
            LH_QtSetupItem::setValue( s );
            return;
        }
    }

    void setValue(int i)
    {
        if( i < -1 || i >= list_.size() ) i = -1;
        if( type() & lh_type_integer )
        {
            LH_QtSetupItem::setValue( i );
            return;
        }
        if( type() & lh_type_string )
        {
            if( i == -1 ) LH_QtSetupItem::setValue(QString());
            else LH_QtSetupItem::setValue( list_.at(i) );
            return;
        }
    }

};

#endif // LH_QT_QSTRINGLIST_H
