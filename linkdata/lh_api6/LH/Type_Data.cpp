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

#include <LH/Type_Data.h>
#include <QMetaType>
#include <QMutex>

namespace LH {
namespace Type {

static QMutex lh_type_data_mutex_;
static const Data * lh_first_info_ = 0;

static int lh_lock_and_filter( int typeId )
{
    lh_type_data_mutex_.lock();
    if( typeId < QMetaType::User )
    {
        Q_ASSERT( !"LH::Type::Data: type ID < QMetaType::User" );
        return 0;
    }
    const Data * mt = lh_first_info_;
    while( mt )
    {
        if( typeId == mt->typeId())
        {
            Q_ASSERT( !"LH::Type::Data: type ID already registered" );
            return 0;
        }
        mt = mt->next();
    }
    return typeId;
}

bool Data::equals( int typeId1, const void * p1, int typeId2, const void * p2 )
{
    const Data * mt = lh_first_info_;
    while( mt )
    {
        if( typeId1 == mt->typeId_ && mt->equals( p1, typeId2, p2 ) ) return true;
        if( typeId2 == mt->typeId_ && mt->equals( p2, typeId1, p1 ) ) return true;
        mt = mt->next_;
    }
    return false;
}

bool Data::lessThan( int typeId1, const void * p1, int typeId2, const void * p2 )
{
    const Data * mt = lh_first_info_;
    while( mt )
    {
        if( typeId1 == mt->typeId_ && mt->lessThan( p1, typeId2, p2 ) ) return true;
        if( typeId2 == mt->typeId_ && mt->lessThan( p2, typeId1, p1 ) ) return false;
        mt = mt->next_;
    }
    return false;
}

bool Data::convert( int fromTypeId, const void * p, int toTypeId, void * v )
{
    const Data * mt = lh_first_info_;
    bool found = false;
    while( mt )
    {
        if( fromTypeId == mt->typeId() )
        {
            found = true;
            if( mt->canConvertTo( toTypeId ) )
            {
                return mt->convertTo( p, toTypeId, v );
            }
            if( mt->canCastTo( toTypeId ) )
            {
                return mt->castTo( p, toTypeId, v );
            }
        }
        if( toTypeId == mt->typeId() && mt->canConvertFrom( fromTypeId ) )
        {
            found = true;
            return mt->convertFrom( v, fromTypeId, p );
        }
        mt = mt->next_;
    }
    Q_ASSERT( found );
    return false;
}

bool Data::canConvert( int fromTypeId, int toTypeId )
{
    const Data * mt = lh_first_info_;
    while( mt )
    {
        if( fromTypeId == mt->typeId() )
        {
            if( mt->canConvertTo( toTypeId ) ) return true;
            if( mt->canCastTo( toTypeId ) ) return true;
        }
        if( toTypeId == mt->typeId() && mt->canConvertFrom( fromTypeId ) ) return true;
        mt = mt->next_;
    }
    return false;
}

Data::Data(int typeId,
           equals_fn equals, lessThan_fn lessThan,
           castTo_fn castTo, convertTo_fn convertTo, convertFrom_fn convertFrom,
           canCastTo_fn canCastTo, canConvertTo_fn canConvertTo, canConvertFrom_fn canConvertFrom ) :
    typeId_( lh_lock_and_filter( typeId ) ),
    equals_( equals ),
    lessThan_( lessThan ),
    castTo_( castTo ),
    convertTo_( convertTo ),
    convertFrom_( convertFrom ),
    canCastTo_( canCastTo ),
    canConvertTo_( canConvertTo ),
    canConvertFrom_( canConvertFrom ),
    next_( typeId_ ? lh_first_info_ : 0 )
{
    if( typeId_ ) lh_first_info_ = this;
    lh_type_data_mutex_.unlock();
}

} // namespace Type
} // namespace LH

