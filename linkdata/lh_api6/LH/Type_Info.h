#ifndef LH_TYPE_INFO_H
#define LH_TYPE_INFO_H

#include <LH/Type.h>

namespace LH {
namespace Type {

template <bool b> struct Bool { static const bool value = b; };

template <typename SELFTYPE, typename OTHERTYPE>
struct Operator
{
    template<typename U, OTHERTYPE (U::*)() const> struct SFINAE {};
    template<typename U> static char Test(SFINAE<U, & U::operator OTHERTYPE >* );
    template<typename U> static int Test(...);
    static const bool value = sizeof(Test<SELFTYPE>(0)) == sizeof(char);
    static inline bool call( const SELFTYPE &, OTHERTYPE *, Bool<false> ) { return false; }
    static inline bool call( const SELFTYPE & m, OTHERTYPE * r, Bool<true> ) { * r = m.operator OTHERTYPE(); return true; }
    static inline bool call( const SELFTYPE & m, OTHERTYPE * r ) { return call( m, r, Bool<value>() ); }
};

template <typename SELFTYPE, typename OTHERTYPE>
struct Assignment
{
    template<typename U, U & (U::*)(const OTHERTYPE &)> struct SFINAE {};
    template<typename U> static char Test(SFINAE<U, & U::operator= >* );
    template<typename U> static int Test(...);
    static const bool value = sizeof(Test<SELFTYPE>(0)) == sizeof(char);
    static inline bool call( SELFTYPE &, const OTHERTYPE *, Bool<false> ) { return false; }
    static inline bool call( SELFTYPE & m, const OTHERTYPE * r, Bool<true> ) { m.operator =( * r ); return true; }
    static inline bool call( SELFTYPE & m, const OTHERTYPE * r ) { return call( m, r, Bool<value>() ); }
};

template <typename SELFTYPE, typename OTHERTYPE>
struct ConvertTo
{
    template<typename U, bool (U::*)(OTHERTYPE &) const> struct SFINAE {};
    template<typename U> static char Test(SFINAE<U, & U::convertTo >* );
    template<typename U> static int Test(...);
    static const bool value = sizeof(Test<SELFTYPE>(0)) == sizeof(char);
    static inline bool call( const SELFTYPE &, OTHERTYPE *, Bool<false> ) { return false; }
    static inline bool call( const SELFTYPE & m, OTHERTYPE * r, Bool<true> ) { return m.convertTo( * r ); }
    static inline bool call( const SELFTYPE & m, OTHERTYPE * r) { return call( m, r, Bool<value>() ); }
};

template <typename SELFTYPE, typename OTHERTYPE>
struct ConvertFrom
{
    template<typename U, bool (U::*)(const OTHERTYPE &)> struct SFINAE {};
    template<typename U> static char Test(SFINAE<U, & U::convertFrom >* );
    template<typename U> static int Test(...);
    static const bool value = sizeof(Test<SELFTYPE>(0)) == sizeof(char);
    static inline bool call( SELFTYPE &, const OTHERTYPE *, Bool<false> ) { return false; }
    static inline bool call( SELFTYPE & m, const OTHERTYPE * r, Bool<true> ) { return m.convertFrom( * r ); }
    static inline bool call( SELFTYPE & m, const OTHERTYPE * r ) { return call( m, r, Bool<value>() ); }
};

template <typename SELFTYPE, typename OTHERTYPE>
struct CanConvertTo
{
    static const bool value = Operator<SELFTYPE,OTHERTYPE>::value || ConvertTo<SELFTYPE,OTHERTYPE>::value;
    static inline bool call( SELFTYPE &, const OTHERTYPE * )
        { return value; }
};

template <typename SELFTYPE, typename OTHERTYPE>
struct CanConvertFrom
{
    static const bool value = Assignment<SELFTYPE,OTHERTYPE>::value || ConvertFrom<SELFTYPE,OTHERTYPE>::value;
    static inline bool call( SELFTYPE &, const OTHERTYPE * ) { return value; }
};

template <typename SELFTYPE, typename OTHERTYPE>
struct TryConvertTo
{
    static inline bool call( const SELFTYPE &, OTHERTYPE *, Bool<false> )
    { return false; }
    static inline bool call( const SELFTYPE & m, OTHERTYPE * r, Bool<true> )
    { return ConvertTo<SELFTYPE,OTHERTYPE>::call(m,r) || Operator<SELFTYPE,OTHERTYPE>::call(m,r); }
    static inline bool call( const SELFTYPE & m, OTHERTYPE * r )
    { return call( m, r, Bool<CanConvertTo<SELFTYPE,OTHERTYPE>::value>() ); }
};

template <typename SELFTYPE, typename OTHERTYPE>
struct TryConvertFrom
{
    static inline bool call( SELFTYPE &, const OTHERTYPE *, Bool<false> )
    { return false; }
    static inline bool call( SELFTYPE & m, const OTHERTYPE * r, Bool<true> )
    { return ConvertFrom<SELFTYPE,OTHERTYPE>::call(m,r) || Assignment<SELFTYPE,OTHERTYPE>::call(m,r); }
    static inline bool call( SELFTYPE & m, const OTHERTYPE * r )
    { return call( m, r, Bool<CanConvertFrom<SELFTYPE,OTHERTYPE>::value>() ); }
};

} // namespace Type
} // namespace LH

#endif // LH_TYPE_INFO_H
