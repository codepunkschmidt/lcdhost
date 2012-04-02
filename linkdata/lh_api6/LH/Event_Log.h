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

#ifndef LH_EVENT_EVENTLOG_H
#define LH_EVENT_EVENTLOG_H

#include <LH/Event_Base.h>
#include <LH/Log_Message.h>
#include <QtGlobal>
#include <QString>

namespace LH {
namespace Event {

class Log : public Base
{
    ::LH::Log::Message msg_;

public:
    LH_EVENT( Log )
    Log( const ::LH::Log::Message & msg ) :
        Base( type() ),
        msg_( msg )
    {}
    Log( const QString & msgtext, QtMsgType msgtype = QtDebugMsg ) :
        Base( type() ),
        msg_( ::LH::Log::Message( msgtext, msgtype ) )
    {}

    const ::LH::Log::Message & msg() const { return msg_; }
    QDateTime msgTime() const { return msg_.time(); }
    QtMsgType msgType() const { return msg_.type(); }
    QString msgText() const { return msg_.text(); }
};

} // namespace Event
} // namespace LH

#endif // LH_EVENT_EVENTLOG_H
