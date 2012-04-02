
#include <LH/Type_MetaData.h>
#include <QMetaType>
#include <QAtomicPointer>

namespace LH {
namespace Type {

static QAtomicPointer<const MetaData> lh_first_info_;

const MetaData * MetaData::find( const int typeId )
{
    if( typeId < int(QMetaType::User) ) return 0;
    const MetaData * mt = lh_first_info_;
    while( mt && mt->typeId_ != typeId ) mt = mt->next_;
    return mt;
}

bool MetaData::compare( int typeId1, const void * p1, int typeId2, const void * p2 )
{
    if( typeId1 < (int) QMetaType::User && typeId2 < (int) QMetaType::User )
        return false;
#if 0
    const MetaData * mt = lh_first_info_;
    while( mt )
    {
        if( typeId1 == mt->typeId_ && mt->canConvertTo( toTypeId ) )
            return mt->convertTo( p, toTypeId, v, ok );
        if( toTypeId == mt->typeId_ && mt->canConvertFrom( typeId1 ) )
            return mt->convertFrom( v, fromTypeId, p, ok );
        mt = mt->next_;
    }

    qDebug() << "MetaData::compare("
             << QMetaType::typeName(typeId1)
             << ","
             << QMetaType::typeName(typeId2)
             << ") failed"
                ;
#endif
    return false;
}

bool MetaData::convert( int fromTypeId, const void * p, int toTypeId, void * v )
{
    if( fromTypeId < (int) QMetaType::User && toTypeId < (int) QMetaType::User )
        return false;

    const MetaData * mt = lh_first_info_;
    while( mt )
    {
        if( fromTypeId == mt->typeId_ && mt->canConvertTo( toTypeId ) )
            return mt->convertTo( p, toTypeId, v );
        if( toTypeId == mt->typeId_ && mt->canConvertFrom( fromTypeId ) )
            return mt->convertFrom( v, fromTypeId, p );
        mt = mt->next_;
    }

    if( fromTypeId >= (int) QMetaType::User )
        qCritical("%s needs 'bool convertTo( %s & ) const' or 'operator %s() const'",
                  QMetaType::typeName(fromTypeId),
                  QMetaType::typeName(toTypeId),
                  QMetaType::typeName(toTypeId) );
    else
        qCritical("%s needs 'bool convertFrom( const %s & )' or '%s & operator =( const %s & )'",
                  QMetaType::typeName(fromTypeId),
                  QMetaType::typeName(toTypeId),
                  QMetaType::typeName(fromTypeId),
                  QMetaType::typeName(toTypeId) );
    return false;
}

bool MetaData::canConvert( int fromTypeId, int toTypeId )
{
    if( fromTypeId < (int) QMetaType::User && toTypeId < (int) QMetaType::User )
        return false;

    const MetaData * mt = lh_first_info_;
    while( mt )
    {
        if( fromTypeId == mt->typeId_ && mt->canConvertTo( toTypeId ) ) return true;
        if( toTypeId == mt->typeId_ && mt->canConvertFrom( fromTypeId ) ) return true;
        mt = mt->next_;
    }

    return false;
}

MetaData::MetaData(const int typeId,
                   compare_fn compare,
                   convertTo_fn convertTo,
                   convertFrom_fn convertFrom,
                   canConvertTo_fn canConvertTo,
                   canConvertFrom_fn canConvertFrom ) :
    typeId_( typeId ),
    compare_( compare ),
    convertTo_( convertTo ),
    convertFrom_( convertFrom ),
    canConvertTo_( canConvertTo ),
    canConvertFrom_( canConvertFrom ),
    next_( lh_first_info_.fetchAndStoreAcquire( this ) )
{}

} // namespace Type
} // namespace LH

