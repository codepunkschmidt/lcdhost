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

#ifndef LH_TYPE_META_H
#define LH_TYPE_META_H

/**
  To use, replace the call to qRegisterMetaType<T>() with LH::Type::Meta<T>().
  */

#include <LH/Type_Data.h>
#include <LH/Type_Map.h>

namespace LH {
namespace Type {

template <class T>
class Meta
{
public:
    explicit Meta()
    {
        typeId( qRegisterMetaType<T>() );
    }

    explicit Meta( const char * typeName )
    {
        typeId( qRegisterMetaType<T>( typeName ) );
    }

    static const int typeId( const int typeId = 0 )
    {
        static const Data typedata_(
                    typeId, equals, lessThan,
                    convertTo, convertFrom,
                    canConvertTo, canConvertFrom );
        return typedata_.typeId();
    }

    static bool equals( const void * p1, int t, const void * p2 )
    {
        return map<const T,const void,Equals>( * reinterpret_cast<const T *>( p1 ), t, p2 );
    }

    static bool lessThan( const void * p1, int t, const void * p2 )
    {
        return map<const T,const void,LessThan>( * reinterpret_cast<const T *>( p1 ), t, p2 );
    }

    static bool convertTo( const void * p, int t, void * v )
    {
        return map<const T,void,TryConvertTo>( * reinterpret_cast<const T *>( p ), t, v );
    }

    static bool convertFrom( void * p, int t, const void * v )
    {
        return map< T, const void, TryConvertFrom >( * reinterpret_cast<T *>( p ), t, v );
    }

    static bool canConvertTo( int t )
    {
        return map<const T,void,CanConvertTo>( * reinterpret_cast<const T *>( 0 ), t, 0 );
    }

    static bool canConvertFrom( int t )
    {
        return map<T,const void,CanConvertFrom>( * reinterpret_cast<T *>( 0 ), t, 0 );
    }
};

} // namespace Type
} // namespace LH

#endif // LH_TYPE_META_H
