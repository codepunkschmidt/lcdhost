/**
  \file     LH_Qt_float.h
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

#ifndef LH_QT_FLOAT_H
#define LH_QT_FLOAT_H

#include "LH_QtSetupItem.h"

class LH_Qt_double : public LH_QtSetupItem
{
public:
    LH_Qt_double( LH_QtObject *parent, const char *ident, double value, double min, double max, int flags = 0 )
        : LH_QtSetupItem( parent, ident, lh_type_double, flags )
    {
        Q_ASSERT( ((flags&LH_FLAG_MINMAX)==LH_FLAG_MINMAX) ? min <= max : true );
        item_.data.d = value;
        item_.param.d.min = min;
        item_.param.d.max = max;
    }

    LH_Qt_double( LH_QtObject *parent, const char *ident, double value, int flags = 0 )
        : LH_QtSetupItem( parent, ident, lh_type_double, flags )
    {
        item_.data.d = value;
    }

    double value() const { return item_.data.d; }
    double min() const { return item_.param.d.min; }
    double max() const { return item_.param.d.max; }
};

#endif // LH_QT_FLOAT_H
