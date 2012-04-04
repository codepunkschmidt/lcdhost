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

namespace LH {
namespace Type {

template <bool b> struct Bool { static const bool value = b; };
template < typename T, typename S, S > struct SFINAE {};

#define LH_TYPE_METHOD_TEST( METHOD ) \
    template <typename U> static char Test( SFINAE< U, Signature, & U::METHOD > * ); \
    template <typename U> static int Test(...); \
    static const bool value = sizeof( Test<SELFTYPE>(0) ) == sizeof(char); \
    static inline bool call( SELFTYPE &, OTHERTYPE *, Bool<false> ) { return false; } \
    static inline bool call( SELFTYPE & m, OTHERTYPE * r ) { return call( m, r, Bool<value>() ); }

template < typename SELFTYPE, typename OTHERTYPE >
struct Equals
{
    typedef bool (SELFTYPE::*Signature)( const OTHERTYPE & ) const;
    static inline bool call( SELFTYPE & m, OTHERTYPE * r, Bool<true> ) { return m.operator ==( * r ); }
    LH_TYPE_METHOD_TEST( operator == )
};

template < typename SELFTYPE, typename OTHERTYPE >
struct LessThan
{
    typedef bool (SELFTYPE::*Signature)( const OTHERTYPE & ) const;
    static inline bool call( SELFTYPE & m, OTHERTYPE * r, Bool<true> ) { return m.operator <( * r ); }
    LH_TYPE_METHOD_TEST( operator < )
};

template < typename SELFTYPE, typename OTHERTYPE >
struct Operator
{
    typedef OTHERTYPE (SELFTYPE::*Signature)() const;
    static inline bool call( SELFTYPE & m, OTHERTYPE * r, Bool<true> ) { * r = m.operator OTHERTYPE(); return true; }
    LH_TYPE_METHOD_TEST( operator OTHERTYPE )
};

template < typename SELFTYPE, typename OTHERTYPE >
struct Assignment
{
    typedef SELFTYPE & (SELFTYPE::*Signature)(const OTHERTYPE &);
    static inline bool call( SELFTYPE & m, OTHERTYPE * r, Bool<true> ) { m.operator =( * r ); return true; }
    LH_TYPE_METHOD_TEST( operator = )
};

template < typename SELFTYPE, typename OTHERTYPE >
struct ConvertTo
{
    typedef bool (SELFTYPE::*Signature)(OTHERTYPE &) const;
    static inline bool call( SELFTYPE & m, OTHERTYPE * r, Bool<true> ) { return m.convertTo( * r ); }
    LH_TYPE_METHOD_TEST( convertTo )
};

template < typename SELFTYPE, typename OTHERTYPE >
struct ConvertFrom
{
    typedef bool (SELFTYPE::*Signature)(const OTHERTYPE &);
    static inline bool call( SELFTYPE & m, OTHERTYPE * r, Bool<true> ) { return m.convertFrom( * r ); }
    LH_TYPE_METHOD_TEST( convertFrom )
};

template < typename SELFTYPE, typename OTHERTYPE >
struct CanConvertTo
{
    static const bool value = Operator<SELFTYPE,OTHERTYPE>::value || ConvertTo<SELFTYPE,OTHERTYPE>::value;
    static inline bool call( SELFTYPE &, OTHERTYPE * ) { return value; }
};

template < typename SELFTYPE, typename OTHERTYPE >
struct CanConvertFrom
{
    static const bool value = Assignment<SELFTYPE,OTHERTYPE>::value || ConvertFrom<SELFTYPE,OTHERTYPE>::value;
    static inline bool call( SELFTYPE &, OTHERTYPE * ) { return value; }
};

template < typename SELFTYPE, typename OTHERTYPE >
struct TryConvertTo
{
    static inline bool call( SELFTYPE & m, OTHERTYPE * r )
    { return ConvertTo<SELFTYPE,OTHERTYPE>::call(m,r) || Operator<SELFTYPE,OTHERTYPE>::call(m,r); }
};

template < typename SELFTYPE, typename OTHERTYPE >
struct TryConvertFrom
{
    static inline bool call( SELFTYPE & m, OTHERTYPE * r )
    { return ConvertFrom<SELFTYPE,OTHERTYPE>::call(m,r) || Assignment<SELFTYPE,OTHERTYPE>::call(m,r); }
};

} // namespace Type
} // namespace LH

#endif // LH_TYPE_INFO_H
