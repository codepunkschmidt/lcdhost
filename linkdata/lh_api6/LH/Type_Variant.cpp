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

#include <LH/Type_Variant.h>

namespace LH {
namespace Type {

bool Variant::equals( const QVariant & a, const QVariant & b )
{
    return ( a == b ) || Data::equals( a.userType(), a.constData(),
                                           b.userType(), b.constData() );
}

bool Variant::lessThan( const QVariant & a, const QVariant & b )
{
    return Data::lessThan( a.userType(), a.constData(),
                               b.userType(), b.constData() );
}

bool Variant::convert( const QVariant & from, QVariant & to )
{
    const QVariant::Type toType = (QVariant::Type) to.userType();
    if( from.canConvert( toType ) )
    {
        to.setValue( from );
        return to.convert( toType );
    }
    return Data::convert(
                from.userType(), from.constData(),
                toType, const_cast<void*>(to.constData()) );
}

bool Variant::canConvert( const QVariant & from, const QVariant & to )
{
    if( from.canConvert( (QVariant::Type) to.userType() ) ) return true;
    return Data::canConvert( from.userType(), to.userType() );
}

} // namespace Type
} // namespace LH
