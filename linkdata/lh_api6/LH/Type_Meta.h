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
  To use, include this file in your implementation (.cpp file) and
  put LH_DEFINE_METATYPE( typename ) after you've included both your
  type's header file and this file.
  */

#include <LH/Type_Data.h>
#include <LH/Type_Map.h>
#include <QVariant>

namespace LH {
namespace Type {

#define LH_TYPE_METHOD_TEST( NAME ) \
    template <typename V> static char Test( SFINAE< T, Func, & V::NAME > * ); \
    template <typename V> static int Test(...); \
    static const bool value = sizeof( Test<T>(0) ) == sizeof(char); \
    static bool call( Self * self, Other * other ) { return call( self, other, Bool<value>() ); } \
    static bool call( Self *, Other *, Bool<false> ) { return false; }

template <class T>
class Meta
{
    static const Data typedata_;
    T * self_;

public:
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
            * other = self->operator Other();
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

    template < template <typename> class Method, typename DATATYPE >
    static inline bool mapType( T * self, int t, DATATYPE * other )
    {
    #define LH_TYPE_PAIR( OTHERTYPE, NAME ) \
        case QMetaTypeId2< OTHERTYPE >::MetaType : \
            return Method< OTHERTYPE >::call( \
                static_cast<typename Method< OTHERTYPE >::Self *>(self), \
                static_cast<typename Method< OTHERTYPE >::Other *>(other) );
        switch( t )
        {
            LH_TYPE_MAP
            default: return false;
        }
    #undef LH_TYPE_PAIR
    }

    static inline Meta<T> wrap( void * self )
    {
        return Meta<T>( static_cast<T *>( self ) );
    }

    static inline Meta<const T> wrap( const void * self )
    {
        return Meta<const T>( static_cast<const T *>( self ) );
    }

    static inline bool equals( const void * p1, int t, const void * p2  )
    {
        return wrap( p1 ).equals(t,p2);
    }

    static inline bool lessThan( const void * p1, int t, const void * p2 )
    {
        return wrap(p1).lessThan(t,p2);
    }

    static inline bool castTo( const void * p, int t, void * v )
    {
        return wrap(p).castTo(t,v);
    }

    static inline bool convertTo( const void * p, int t, void * v )
    {
        return wrap(p).convertTo(t,v);
    }

    static inline bool convertFrom( void * p, int t, const void * v )
    {
        return wrap(p).convertFrom(t,v);
    }

    static inline bool canCastTo( int t )
    {
        return mapType< CanCastTo, void >(0,t,0);
    }

    static inline bool canConvertTo( int t )
    {
        return mapType< CanConvertTo, void >(0,t,0);
    }

    static inline bool canConvertFrom( int t )
    {
        return mapType< CanConvertFrom, const void >(0,t,0);
    }

    Meta( T * self ) :
        self_( self )
    {}

    bool equals( int t, const void * p ) const
    {
        return mapType< Equals, const void >( self_, t, p );
    }

    bool lessThan( int t, const void * p ) const
    {
        return mapType< LessThan, const void >( self_, t, p );
    }

    bool assign( int t, const void * p ) const
    {
        return mapType< Assignment, const void >( self_, t, p );
    }

    bool castTo( int t, void * p ) const
    {
        return mapType< CastTo, void >( self_, t, p );
    }

    bool convertTo( int t, void * p ) const
    {
        return mapType< ConvertTo, void >( self_, t, p );
    }

    bool convertFrom( int t, const void * p ) const
    {
        return mapType< ConvertFrom, const void >( self_, t, p );
    }
};

template <typename T>
const Data Meta<T>::typedata_(
        qMetaTypeId< T >(), // qRegisterMetaType<T>(),
        equals, lessThan,
        castTo, convertTo, convertFrom,
        canCastTo, canConvertTo, canConvertFrom );

bool equals( const QVariant & a, const QVariant & b );
bool lessThan( const QVariant & a, const QVariant & b );
bool convert( const QVariant & from, QVariant & to );
bool canConvert( const QVariant & from, const QVariant & to );

} // namespace Type
} // namespace LH

#define LH_DEFINE_METATYPE( TYPENAME ) \
    template class LH::Type::Meta< TYPENAME >; \
    template <> bool QVariant::canConvert< TYPENAME >() const { \
        return LH::Type::Data::canConvert( this->userType(), qMetaTypeId< TYPENAME >() ); } \
    template <> TYPENAME qvariant_cast( const QVariant & v ) { \
        TYPENAME result; \
        LH::Type::Meta< TYPENAME >::convertFrom( & result, v.userType(), v.constData() ); \
        return result; }

#endif // LH_TYPE_META_H
