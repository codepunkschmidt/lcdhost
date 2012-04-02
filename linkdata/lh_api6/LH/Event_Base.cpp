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


#include <LH/Event_Base.h>

#include <QHash>
#include <QMutex>
#include <QDebug>

namespace LH {
namespace Event {

QEvent::Type typeHash( const char ** p_classname, QEvent::Type lookup )
{
    static QMutex mutex_;
    static QHash<QByteArray,QEvent::Type> hash_;
    static const QByteArray unknown_("<unknown>");

    mutex_.lock();

    if( p_classname )
    {
        if( lookup == QEvent::None )
        {
            QHash<QByteArray,QEvent::Type>::iterator it;
            it = hash_.find( QByteArray::fromRawData( *p_classname, strlen( *p_classname ) ) );
            if( it == hash_.end() )
                it = hash_.insert( QByteArray( *p_classname ),
                                   (QEvent::Type) QEvent::registerEventType()
                                   );
            lookup = it.value();
        }
        else
        {
            const QByteArray classname( hash_.key( lookup ) );
            if( classname.isNull() )
                *p_classname = unknown_.constData();
            else
                *p_classname = classname.constData();
        }
    }
#ifndef QT_NO_DEBUG
    else
    {
        qDebug() << "LH::Event::typeHash():" << hash_.size() << "registered events";
        foreach( const QByteArray & key, hash_.keys() )
            qDebug() << "  " << key.constData() << "==" << hash_.value(key);
    }
#endif

    mutex_.unlock();

    return lookup;
}

} // namespace Event
} // namespace LH
