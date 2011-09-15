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
  Returns the QVariant::Type used to store the
  lh_format given.
  */
QVariant::Type lh_qvarianttype( lh_format fmt );

/**
  Reads a lh_variant structure and sets the
  QVariant accordingly. This will change the
  QVariant::Type as needed.
  */
void lh_variant_to_qvariant( const lh_variant&, QVariant& );

/**
  Fills a lh_variant using data from the QVariant.
  The format of the lh_variant is preserved if set,
  and the QVariant data will be converted as needed.

  If the lh_variant's lh_format is lh_format_none,
  this function will make a best-guess on what it
  should be based on the QVariant.
  */
void lh_qvariant_to_variant( const QVariant&, lh_variant& );

/**
  Attempts to construct LCDHost formatted string
  data for the given QVariant and appends it to
  the target string. If the target string is not
  empty, a single space is appended first.

  Returns the number of UNICODE characters added.
  This may be zero if the QVariant was invalid or
  can't be encoded as LCDHost string data.
  */
int lh_qvariant_to_qstring( const QVariant&, QString& );

/**
  Converts LCDHost formatted string data into QVariant,
  preserving the QVariant data type.

  If the QVariant does not have a data type set,
  (QVariant::Invalid) this function will make a
  best-guess on the data type, defaulting
  to QVariant::String.

  Returns the number of characters used from the
  string, or zero in case of error. Note that
  this function greedily consumes whitespace
  if a conversion was possible, and the number
  returned reflects that.
  */
int lh_qstring_to_qvariant( const QString& s, QVariant& );

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
  Overloads qVariantSetValue() so it can use a lh_variant
  reference as a parameter.

  \sa lh_variant_to_qvariant()
  */
inline void qVariantSetValue( QVariant& v, const lh_variant& lhv )
{
    lh_variant_to_qvariant( lhv, v );
}

/**
  Converts a QVariant into LCDHost formatted string
  data. If the QVariant is invalid or can't be
  stored in LCDHost format, this function returns
  a null QString.

  \sa lh_qvariant_to_qstring()
  */
inline QString lh_qstring_from_qvariant( const QVariant& v )
{
    QString s;
    lh_qvariant_to_qstring( v, s );
    return s;
}

/**
  Converts string data in LCDHost format to a QVariant.
  If possible, you should provide the QVariant data type.

  \sa lh_qstring_to_qvariant()
  */
inline QVariant lh_qvariant_from_qstring( const QString& s, QVariant::Type t = QVariant::Invalid )
{
    QVariant v(t);
    lh_qstring_to_qvariant(s,v);
    return v;
}


/**
  Stream LCDHost formatted data from a QString to
  QVariant.

  This function will remove used characters from the
  beginning of the string and return a reference
  to the string. This function greedily consumes
  whitespace.

  \return A reference to the source string.

  \sa lh_qstring_to_qvariant()
  */
inline QString& operator>>( QString& s, QVariant& v )
{
    return s.remove(0,lh_qstring_to_qvariant( s, v ));
}

/**
  Converts a QVariant into LCDHost formatted string
  data. If the QVariant is invalid or can't be
  stored in LCDHost format, this function returns
  with no action taken.

  If the target string is not empty, a single space
  is appended first before converting.

  \return A reference to the target string.

  \sa lh_qvariant_to_qstring()
  */
inline QString& operator<<( QString& s, const QVariant& v )
{
    lh_qvariant_to_qstring( v, s );
    return s;
}

#endif // LH_QVARIANT_H
