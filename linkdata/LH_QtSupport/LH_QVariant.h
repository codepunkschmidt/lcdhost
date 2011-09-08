/**
  \file     LH_QVariant.h
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009-2011, Johan Lindh

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

#ifndef LH_QVARIANT_H
#define LH_QVARIANT_H

#include <QVariant>
#include <QMetaType>
#include "lh_plugin.h"

Q_DECLARE_METATYPE(lh_input)

/**
  Extends the normal QVariant to be aware of LCDHost data types
  and how they're converted to and from QString and lh_variant.
  */

class LH_QVariant : public QVariant
{
public:
    LH_QVariant() :
        QVariant()
    { }

    LH_QVariant( const LH_QVariant& other ) :
        QVariant(other)
    { }

    LH_QVariant( const QVariant& val ) :
        QVariant(val)
    { }

    LH_QVariant( const lh_variant& lhv ) :
        QVariant()
    {
        read(lhv);
    }

    void fromString( const QString& s );
    QString toString() const;

    bool read( const lh_variant& lhv );
    void write( lh_variant& lhv ) const;

    static const char *lh_formatname( lh_format fmt );
    static QVariant::Type variantType( lh_format fmt );
};

#endif // LH_QVARIANT_H
