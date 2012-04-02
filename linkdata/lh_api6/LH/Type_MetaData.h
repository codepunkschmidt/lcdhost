#ifndef LH_TYPE_METADATA_H
#define LH_TYPE_METADATA_H

#include <LH/Type.h>

namespace LH {
namespace Type {

class MetaData
{
public:
    typedef bool (* compare_fn)( const void * p1, const void * p2 );
    typedef bool (* convertTo_fn)( const void * p, int t, void * v );
    typedef bool (* convertFrom_fn)( void * p, int t, const void * v );
    typedef bool (* canConvertTo_fn)( int t );
    typedef bool (* canConvertFrom_fn)( int t );

    static const MetaData * find( int typeId );
    static bool compare( int typeId1, const void * p1, int typeId2, const void * p2 );
    static bool convert( int fromTypeId, const void * p, int toTypeId, void * v );
    static bool canConvert( int fromTypeId, int toTypeId );

    explicit MetaData(
            const int typeId,
            compare_fn compare,
            convertTo_fn convertTo,
            convertFrom_fn convertFrom,
            canConvertTo_fn canConvertTo,
            canConvertFrom_fn canConvertFrom
            );

    const int typeId() const { return typeId_; }
    bool compare( const void * p1, const void * p2 ) const { return compare_( p1, p2 ); }
    bool convertTo( const void * p, int t, void * v ) const { return convertTo_( p, t, v ); }
    bool convertFrom( void * p, int t, const void * v ) const { return convertFrom_( p, t, v ); }
    bool canConvertTo( int t ) const { return canConvertTo_( t ); }
    bool canConvertFrom( int t ) const { return canConvertFrom_( t ); }

private:
    const int typeId_;
    const compare_fn compare_;
    const convertTo_fn convertTo_;
    const convertFrom_fn convertFrom_;
    const canConvertTo_fn canConvertTo_;
    const canConvertFrom_fn canConvertFrom_;
    const MetaData * const next_;
};

} // namespace Type
} // namespace LH

#endif // LH_TYPE_METADATA_H
