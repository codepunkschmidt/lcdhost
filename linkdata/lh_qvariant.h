/**
  \file     lh_qvariant.h
  \brief    Extends the normal QVariant to be aware of LCDHost data types.
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

class QXmlStreamWriter;
class QXmlStreamReader;
class QColor;

/**
  Returns the QVariant::Type used to store the
  lh_format_xxx embedded in the lh_property given.
  */
QVariant::Type lh_qvarianttype( lh_meta_format fmt );

/**
  Returns a string representation of a QVariant.

  This is generally the same as using
  QVariant::toString(), but will also handle
  LCDHost specific data types and may produce
  other results for some data types (like
  QColor, for which the standard toString()
  loses alpha channel data).

  Note that QVariant::StringList and
  QVariant::List can not be accurately
  represented with an unescaped string, so
  this function simply returns a null
  string for them.
  */
QString lh_qstring_from_qvariant(const QVariant &v);

/**
  Converts LCDHost formatted string data into
  QVariant, preserving the QVariant data type.

  If the QVariant does not have a data type set,
  (QVariant::Invalid) this function will make a
  best-guess on the data type, defaulting
  to QVariant::String.
  */
void lh_qstring_to_qvariant(const QString& s, QVariant& );

void lh_qstring_to_qcolor( const QString& s, QColor& c );
QString lh_qstring_from_qcolor( const QColor& c );
void lh_qstring_to_lhinput( const QString& s, lh_input& in );
QString lh_qstring_from_lhinput( const lh_input& in );

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

#endif // LH_QVARIANT_H
