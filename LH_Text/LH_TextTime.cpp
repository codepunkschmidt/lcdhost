/**
  \file     LH_TextTime.cpp
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009 Johan Lindh

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

#include <QtGlobal>
#include <QTime>
#include <QFont>
#include <QFontMetrics>

#include "LH_Text.h"
#include "../LH_Qt_QString.h"

class LH_TextTime : public LH_Text
{
protected:
    LH_Qt_QString *setup_format_;

public:
    const char *userInit()
    {
        if( const char *err = LH_Text::userInit() ) return err;
        setup_format_ = new LH_Qt_QString(this,"<a href=\"http://doc.trolltech.com/4.6/qtime.html#toString\">Format</a>","HH:mm:ss");
        setup_text_->setTitle("Current time");
        setup_text_->setFlag(LH_FLAG_READONLY,true);
        return 0;
    }

    /**
      This being a time class, we want to update the image every second. Therefore we
      listen to the LH_NOTE_SECOND notification. Since the notification might fire a
      few milliseconds before or after the second switchover, we'll select the closest
      second to show. Finally, unless the text actually changed (setText() returns true)
      we don't need to re-render at all.
      */
    int notify(int n,void* p)
    {
        Q_UNUSED(p);

        if( !n || n&LH_NOTE_SECOND )
        {
            QTime now = QTime::currentTime();
            if( now.msec() > 500 ) now = now.addSecs(1);
            // if( !(now.second() % 10) ) for(;;) ; // to test plugins that hang
            if( setText( now.toString( setup_format_->value() ) ) )
                callback(lh_cb_render,NULL);
        }
        return LH_NOTE_SECOND | LH_Text::notify(n,p);
    }

    static lh_class *classInfo()
    {
        static lh_class classInfo =
        {
            sizeof(lh_class),
            "System/Date and time",
            "SystemTimeText",
            "Time (Text)",
            -1, -1,
            lh_object_calltable_NULL,
            lh_instance_calltable_NULL
        };

        if( classInfo.width == -1 )
        {
            QFont font;
            QFontMetrics fm( font );
            classInfo.height = fm.height();
            classInfo.width = fm.width("00:00:00");
            // *( (int*) 0 ) = 0; // test plugin segmentation violations
        }

        return &classInfo;
    }
};

LH_PLUGIN_CLASS(LH_TextTime)
