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

#ifndef LH_METATYPE_H
#define LH_METATYPE_H

/**
  To use, replace the call to qRegisterMetaType<T>() with LH::MetaType<T>().
  Optionally, to allow QVariant primitive types to be converted into this
  type, add LH_METATYPE(classname) in the implementation file (.cpp)
  */

#include <LH/Type_Map.h>
#include <LH/Type_MetaData.h>
#include <QVariant>

namespace LH {

template <class T>
class MetaType
{
public:
    explicit MetaType( const char * typeName = 0 )
    {
        static const LH::Type::MetaData metadata_(
                    typeName ? qRegisterMetaType<T>( typeName ) : qRegisterMetaType<T>(),
                    compare, convertTo, convertFrom, canConvertTo, canConvertFrom );
    }

    static bool convertTo( const void * p, int t, void * v )
    {
        return LH::Type::Map<const T,void,LH::Type::TryConvertTo>( * reinterpret_cast<const T *>( p ), t, v );
    }

    static bool convertFrom( void * p, int t, const void * v )
    {
        return LH::Type::Map<T,const void,LH::Type::TryConvertFrom>( * reinterpret_cast<T *>( p ), t, v );
    }

    static bool canConvertTo( int t )
    {
        return LH::Type::Map<const T,void,LH::Type::CanConvertTo>( * reinterpret_cast<const T *>( 0 ), t, 0 );
    }

    static bool canConvertFrom( int t )
    {
        return LH::Type::Map<T,const void,LH::Type::CanConvertFrom>( * reinterpret_cast<T *>( 0 ), t, 0 );
    }

    struct HasCompareMethod
    {
        template<typename U, bool (U::*)(const T &) const> struct SFINAE {};
        template<typename U> static char Test(SFINAE<U, & U::operator== >* );
        template<typename U> static int Test(...);
        static const bool Has = sizeof(Test<T>(0)) == sizeof(char);
    };
    static inline bool compare( const T & m, const T & other, LH::Type::Bool<true> )
    {
        return m.operator==( other );
    }
    static inline bool compare( const T &, const T &, LH::Type::Bool<false> )
    {
        return false;
    }
    static bool compare( const void * p1, const void * p2 )
    {
        return compare( *reinterpret_cast<const T *>(p1),
                        *reinterpret_cast<const T *>(p2),
                        LH::Type::Bool<HasCompareMethod::Has>() );
    }
};

} // namespace LH

#define LH_METATYPE( TYPE ) \
    template <> bool QVariant::canConvert< TYPE >() const { \
        return LH::Type::MetaData::canConvert( this->userType(), qMetaTypeId< TYPE >() ); } \
    template <> TYPE qvariant_cast( const QVariant & v ) { \
        TYPE result; \
        LH::MetaType<TYPE>::convertFrom( & result, v.userType(), v.constData() ); \
        return result; }

#endif // LH_METATYPE_H
