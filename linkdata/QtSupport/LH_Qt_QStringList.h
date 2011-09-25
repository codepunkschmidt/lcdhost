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

class LH_Qt_QStringList : public LH_QtSetupItem
{
    int index_;
    QStringList list_;

public:
	// Reasonable subtypes:
	//  lh_type_integer_list - Yields a dropdown box
	//  lh_type_integer_listbox - Yields a listbox

    LH_Qt_QStringList( LH_QtObject *parent, QString name, const QStringList & list, int flags = 0, lh_setup_type subtype = lh_type_integer_list  ) :
        LH_QtSetupItem( parent, name, subtype, flags ),
        index_( 0 ),
        list_( list )
    {
        LH_QtSetupItem::setList( list_ );
    }
    
    QStringList & list()
    {
        return list_;
    }

    void setList( const QStringList & newlist )
    {
        list_ = newlist;
        refreshList();
    }

    void refreshList()
    {
        LH_QtSetupItem::setList( list_ );
    }

    int value() const
    {
        return index_;
    }

    int index() const
    {
        return index_;
    }

    QString valueText() const
    {
        if( index_ < 0 || index_ >= list_.size() ) return QString();
        return list_.at( index_ );
    }

    virtual void setup_change()
    {
        emit change( value() );
        LH_QtSetupItem::setup_change();
    }

    void setIndex( int i )
    {
        if( i < -1 ) i = -1;
        if( i >= list_.size() ) i = list_.size() - 1;
        if( index_ != i )
        {
            index_ = i;
            LH_QtSetupItem::setValue( i >= 0 ? list_.at( index_ ) : QString() );
        }
    }

    void setValue( int i )
    {
        setIndex( i );
    }

    void setValue( const QString & str )
    {
        setIndex( list_.indexOf(str) );
    }
};

#endif // LH_QT_QSTRINGLIST_H
