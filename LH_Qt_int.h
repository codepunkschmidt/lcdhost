/**
  \file     LH_Qt_int.h
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

#ifndef LH_QT_INT_H
#define LH_QT_INT_H

#include "LH_QtSetupItem.h"

class LH_Qt_int : public LH_QtSetupItem
{
public:
    LH_Qt_int( LH_QtObject *parent, const char *ident, qint64 value, qint64 min, qint64 max, int flags = 0, lh_setup_type subtype = lh_type_integer )
        : LH_QtSetupItem( parent, ident, subtype, flags )
    {
        Q_ASSERT( ((flags&LH_FLAG_MINMAX)==LH_FLAG_MINMAX) ? min <= max : true );
        item_.data.i = value;
        item_.param.i.min = min;
        item_.param.i.max = max;
    }

    LH_Qt_int( LH_QtObject *parent, const char *ident, qint64 value, int flags = 0, lh_setup_type subtype = lh_type_integer )
        : LH_QtSetupItem( parent, ident, subtype, flags )
    {
        item_.data.i = value;
    }

    virtual void setup_change()
    {
        emit change( item_.data.i );
        LH_QtSetupItem::setup_change();
    }

    qint64 value() const { return item_.data.i; }
    qint64 min() const { return item_.param.i.min; }
    qint64 max() const { return item_.param.i.max; }
};

#endif // LH_QT_INT_H
