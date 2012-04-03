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

#include <LH/Type_MetaData.h>
#include <QAtomicPointer>
#include <QMetaType>
#include <QVariant>

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

bool convert( const QVariant & from, QVariant & to )
{
    const QVariant::Type toType = (QVariant::Type) to.userType();
    if( from.canConvert( toType ) )
    {
        to.setValue( from );
        return to.convert( toType );
    }
    return MetaData::convert( from.userType(), from.constData(),
                              toType, const_cast<void*>(to.constData()) );
}

bool canConvert( const QVariant & from, const QVariant & to )
{
    return from.canConvert( (QVariant::Type) to.userType() ) ||
            MetaData::canConvert( from.userType(), to.userType() );
}

} // namespace Type
} // namespace LH

