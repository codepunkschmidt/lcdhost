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
    LH_Qt_int( LH_QtObject *parent, const char *ident, long long value, long long min, long long max, int flags = 0, lh_meta_type subtype = lh_type_integer )
        : LH_QtSetupItem( parent, ident, subtype, flags )
    {
        setValue(value);
        setMinimum( min );
        setMaximum( max );
    }

    LH_Qt_int( LH_QtObject *parent, const char *ident, long long value = 0, int flags = 0, lh_meta_type subtype = lh_type_integer )
        : LH_QtSetupItem( parent, ident, subtype, flags )
    {
        setValue(value);
    }

    int value() const { return LH_QtSetupItem::value().toInt(); }
    int min() const { return LH_QtSetupItem::minimum().toInt(); }
    int max() const { return LH_QtSetupItem::maximum().toInt(); }
    void setMin( int v ) { setMinimum(v); }
    void setMax( int v ) { setMaximum(v); }
};

#endif // LH_QT_INT_H
