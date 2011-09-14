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

/**
  Extends the normal QVariant to be aware of LCDHost data types
  and how they're converted to and from QString, lh_variant and
  XML streams.
  */

#include <QVariant>
#include <QMetaType>
#include "lh_plugin.h"

Q_DECLARE_METATYPE(lh_input)

class QXmlStreamWriter;
class QXmlStreamReader;

/**
  Returns true if a QVariant can be read from the XML stream
  in LCDHost format.
  */
bool operator>( const QXmlStreamReader& reader, const QVariant& obj );

/**
  Reads a QVariant from the XML stream in LCDHost format.
  */
QXmlStreamReader& operator>>( QXmlStreamReader& reader, QVariant& obj );

/**
  Writes a QVariant to an XML stream in LCDHost format.
  */
QXmlStreamWriter& operator<<( QXmlStreamWriter& writer, const QVariant& obj );

/**
  Converts LCDHost formatted string data into QVariant,
  preserving the QVariant data type. If the QVariant
  does not have a data type set, it will be set to
  QVariant::String.

  "LCDHost formatting" means the string data is
  whitespace separated. Embedded whitespace
  is escaped.

  Returns the number of characters used from the
  string, or zero in case of error.
  */
int operator>>( const QString&, QVariant& );

/**
  \sa int operator>>( const QString&, QVariant& )

  This function will remove used characters from the
  beginning of the string and return a reference
  to the string.
  */
QString& operator>>( QString& s, QVariant& v );

/**
  Converts a QVariant into LCDHost formatted string
  data. Appends to the given string and returns a
  reference to it.
  */
QString& operator<<( QString&, const QVariant& );

/**
  Writes a QVariant into a lh_variant structure.
  Returns true if successful.
  */
bool operator>>( const QVariant&, lh_variant& lhv );

/**
  Returns the QVariant::Type used to store the
  lh_format given.
  */
QVariant::Type lh_varianttype( lh_format fmt );

/**
  Reads a lh_variant structure and sets the
  QVariant accordingly. This will change the
  QVariant::Type as needed.
  */
void qVariantSetValue( QVariant&, const lh_variant& );

#endif // LH_QVARIANT_H
