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
#include "LH_Qt_QString.h"

/*
  Supports the following setup types:
    lh_type_string_dropdownbox
    lh_type_string_listbox
    lh_type_string_combobox
  */

class LH_Qt_QStringList : public LH_Qt_QString
{
public:
    LH_Qt_QStringList( LH_QtObject *parent, const char *ident, const QStringList& list, int flags = 0,
                       lh_meta_type subtype = lh_type_string_combobox  )
        : LH_Qt_QString( parent, ident, list.isEmpty()?QString():list.first(), flags, subtype )
    {
        setOther( list );
    }

    // const because modifying will have no effect on the underlying data
    // change the code to modify a copy and then use setList(), or use appendToList() or clearList()
    const QStringList list() const
    {
        return other().toStringList();
    }

    void clearList()
    {
        setOther(QStringList());
    }

    void appendToList( const QString& s )
    {
        QStringList sl(list());
        sl.append(s);
        setOther(sl);
    }

    void setList( const QStringList& sl )
    {
        setOther(sl);
    }

    int index() const
    {
        return list().indexOf( value() );
    }

    void setIndex( int idx )
    {
        LH_Qt_QString::setValue( list().at(idx) );
    }
};

#endif // LH_QT_QSTRINGLIST_H