/**
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2012 Johan Lindh

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

#ifndef LH_TYPE_DATA_H
#define LH_TYPE_DATA_H

#include <LH/Type.h>

namespace LH {
namespace Type {


class Data
{
public:
    typedef bool (* equals_fn)( const void * p1, int t, const void * p2 );
    typedef bool (* lessThan_fn)( const void * p1, int t, const void * p2 );
    typedef bool (* castTo_fn)( const void * p, int t, void * v );
    typedef bool (* convertTo_fn)( const void * p, int t, void * v );
    typedef bool (* convertFrom_fn)( void * p, int t, const void * v );
    typedef bool (* canCastTo_fn)( int t );
    typedef bool (* canConvertTo_fn)( int t );
    typedef bool (* canConvertFrom_fn)( int t );

    static bool equals( int t1, const void *p1, int t2, const void *p2 );
    static bool lessThan( int t1, const void *p1, int t2, const void *p2 );
    static bool convert( int fromTypeId, const void * p, int toTypeId, void * v );
    static bool canConvert( int fromTypeId, int toTypeId );

    explicit Data(
            int typeId,
            equals_fn equals,
            lessThan_fn lessThan,
            castTo_fn castTo,
            convertTo_fn convertTo,
            convertFrom_fn convertFrom,
            canCastTo_fn canCastTo,
            canConvertTo_fn canConvertTo,
            canConvertFrom_fn canConvertFrom
            );

    const int typeId() const { return typeId_; }
    bool equals( const void * p1, int t, const void * p2 ) const { return equals_( p1, t, p2 ); }
    bool lessThan( const void * p1, int t, const void * p2 ) const { return lessThan_( p1, t, p2 ); }
    bool castTo( const void * p, int t, void * v ) const { return castTo_( p, t, v ); }
    bool convertTo( const void * p, int t, void * v ) const { return convertTo_( p, t, v ); }
    bool convertFrom( void * p, int t, const void * v ) const { return convertFrom_( p, t, v ); }
    bool canCastTo( int t ) const { return canCastTo_( t ); }
    bool canConvertTo( int t ) const { return canConvertTo_( t ); }
    bool canConvertFrom( int t ) const { return canConvertFrom_( t ); }
    const Data * const next() const { return next_; }

private:
    const int typeId_;
    const equals_fn equals_;
    const lessThan_fn lessThan_;
    const castTo_fn castTo_;
    const convertTo_fn convertTo_;
    const convertFrom_fn convertFrom_;
    const canCastTo_fn canCastTo_;
    const canConvertTo_fn canConvertTo_;
    const canConvertFrom_fn canConvertFrom_;
    const Data * const next_;
};

} // namespace Type
} // namespace LH

#endif // LH_TYPE_DATA_H
