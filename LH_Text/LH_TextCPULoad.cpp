/**
  \file     LH_TextCPULoad.cpp
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

#include <QFont>
#include <QFontMetrics>
#include <QTime>

#include "LH_Text.h"

class LH_TextCPULoad : public LH_Text
{
public:
    LH_TextCPULoad() : LH_Text()
    {
        setup_text_->setName( "Average CPU load" );
        setup_text_->setFlag( LH_FLAG_READONLY, true );
        return;
    }

    const char *userInit()
    {
        const char *retv = LH_Text::userInit();
        if( !retv )
        {
            setText("?%");
        }
        return retv;
    }

    static lh_class *classInfo()
    {
        static lh_class classInfo =
        {
            sizeof(lh_class),
            "System/CPU",
            "SystemCPUText",
            "Average Load (Text)",
            -1, -1,
            lh_object_calltable_NULL,
            lh_instance_calltable_NULL
        };

        if( classInfo.width == -1 )
        {
            QFont font;
            QFontMetrics fm( font );
            classInfo.height = fm.height();
            classInfo.width = fm.width("100%");
        }

        return &classInfo;
    }

    /**
      In this class, we set the text to render in notify() instead of prerender().
      We do this because rendering can be done a lot more often than we ask for it,
      and we just need to update the load text when the LH_NOTE_SECOND notification
      fires. We use LH_NOTE_SECOND because the average load value lh_systemstate->cpu_load
      updates once a second. We'll also do the work if the value for 'n' is zero, since
      that's the initial call to notify right after the instance has been created.
      */
    int notify(int n,void* p)
    {
        if( !n || n&LH_NOTE_SECOND )
        {
            if( setText( QString::number(state()->cpu_load/100)+"%" ) )
                callback(lh_cb_render,NULL); // only render if the text changed
        }
        return LH_Text::notify(n,p) | LH_NOTE_SECOND;
    }

};

LH_PLUGIN_CLASS(LH_TextCPULoad)
