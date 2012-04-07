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

#ifndef LH_TYPE_INFO_H
#define LH_TYPE_INFO_H

#include <LH/Type.h>
#include <LH/Type_Map.h>
#include <QDebug>

template < typename U, typename T >
struct LH_Cast_Helper
{
    static void castToHelper( U *, const T * );
};

namespace LH {
namespace Type {

#define LH_TYPE_METHOD_TEST( NAME ) \
    template <typename V> static char Test( SFINAE< T, Func, & V::NAME > * ); \
    template <typename V> static int Test(...); \
    static const bool value = sizeof( Test<T>(0) ) == sizeof(char); \
    static bool call( Self * self, Other * other ) { return call( self, other, Bool<value>() ); } \
    static bool call( Self *, Other *, Bool<false> ) { return false; }

template < typename T >
class Wrap
{
    T * self_;

public:
    Wrap( T * self ) :
        self_( self )
    {}

    template < bool b > struct Bool { static const bool value = b; };
    template < typename X, typename Y, Y > struct SFINAE {};

    template < typename U >
    struct Equals
    {
        typedef const T Self;
        typedef const U Other;
        typedef bool (T::*Func)( Other & ) const;
        LH_TYPE_METHOD_TEST( operator == )
        static bool call( Self * self, Other * other, Bool<true> )
        {
            return (*self).operator ==(*other);
        }
    };

    template < typename U >
    struct LessThan
    {
        typedef const T Self;
        typedef const U Other;
        typedef bool (T::*Func)( Other & ) const;
        LH_TYPE_METHOD_TEST( operator < )
        static bool call( Self * self, Other * other, Bool<true> )
        {
            return (*self).operator <( * other );
        }
    };

    template < typename U >
    struct Assignment
    {
        typedef T Self;
        typedef const U Other;
        typedef Self & (T::*Func)( Other & );
        LH_TYPE_METHOD_TEST( operator = )
        static bool call( Self * self, Other * other, Bool<true> )
        {
            return (*self).operator =( * other );
        }
    };

    template < typename U >
    struct CastTo
    {
        typedef const T Self;
        typedef U Other;
        typedef Other (T::*Func)() const;
        LH_TYPE_METHOD_TEST( operator Other )
        static bool call( Self * self, Other * other, Bool<true> )
        {
            // If you get an error here, you forgot LH_META_TYPE( typename )
            // in the implementation (.cpp file) for your meta type.
            * other = self->operator Other();

            // LH_Cast_Helper<Other,Self>::castToHelper( other, self );
            return true;
        }
    };

    template < typename U >
    struct ConvertTo
    {
        typedef const T Self;
        typedef U Other;
        typedef bool (T::*Func)( Other & ) const;
        LH_TYPE_METHOD_TEST( convertTo )
        static bool call( Self * self, Other * other, Bool<true> )
        {
            return (*self).convertTo( * other );
        }
    };

    template < typename U >
    struct ConvertFrom
    {
        typedef T Self;
        typedef const U Other;
        typedef bool (T::*Func)( Other & );
        LH_TYPE_METHOD_TEST( convertFrom )
        static bool call( Self * self, Other * other, Bool<true> )
        {
            return (*self).convertFrom( * other );
        }
    };

    template < typename U >
    struct CanCastTo
    {
        typedef const T Self;
        typedef U Other;
        static bool call(Self*,Other*) { return CastTo<U>::value; }
    };

    template < typename U >
    struct CanConvertTo
    {
        typedef const T Self;
        typedef U Other;
        static bool call(Self*,Other*) { return ConvertTo<U>::value; }
    };

    template < typename U >
    struct CanConvertFrom
    {
        typedef T Self;
        typedef const U Other;
        static bool call(Self*,Other*) { return ConvertFrom<U>::value; }
    };

    bool equals( int t, const void * p ) const
    {
        return type_switch< T, const void, Equals >( self_, t, p );
    }

    bool lessThan( int t, const void * p ) const
    {
        return type_switch< T, const void, LessThan >( self_, t, p );
    }

    bool assign( int t, const void * p ) const
    {
        return type_switch< T, const void, Assignment >( self_, t, p );
    }

    bool castTo( int t, void * p ) const
    {
        return type_switch< T, void, CastTo >( self_, t, p );
    }

    bool convertTo( int t, void * p ) const
    {
        return type_switch< T, void, ConvertTo >( self_, t, p );
    }

    bool convertFrom( int t, const void * p ) const
    {
        return type_switch< T, const void, ConvertFrom >( self_, t, p );
    }

    static bool canCastTo( int t )
    {
        return type_switch< T, void, CanCastTo>(0,t,0);
    }

    static bool canConvertTo( int t )
    {
        return type_switch< T, void, CanConvertTo>(0,t,0);
    }

    static bool canConvertFrom( int t )
    {
        return type_switch< T, const void, CanConvertFrom>(0,t,0);
    }
};

} // namespace Type
} // namespace LH

#endif // LH_TYPE_INFO_H
