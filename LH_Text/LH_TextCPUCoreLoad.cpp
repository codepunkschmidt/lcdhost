/**
  \file     LH_TextCPUCoreLoad.cpp
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
#include "LH_QtCPU.h"
#include "LH_Qt_int.h"

class LH_TextCPUCoreLoad : public LH_Text
{
    LH_Qt_int *setup_core_selector_;
    LH_QtCPU *cpu_;

public:
    const char *userInit()
    {
        if( const char *err = LH_Text::userInit() ) return err;
        cpu_ = new LH_QtCPU(this);
        setup_text_->setTitle( "Average core load" );
        setup_text_->setFlag( LH_FLAG_READONLY, true );
        setup_core_selector_ = new LH_Qt_int( this, "Select core", 1, 1, cpu_->count(), LH_FLAG_FIRST );
        connect( setup_core_selector_, SIGNAL(changed()), this, SLOT(updateValue()) );
        setText("0%");
        return 0;
    }

    static lh_class *classInfo()
    {
        static lh_class classInfo =
        {
            sizeof(lh_class),
            "System/CPU",
            "SystemCoreText",
            "Core Load (Text)",
            -1, -1,
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

    int notify(int n,void* p)
    {
        if( !n || n&LH_NOTE_SECOND ) updateValue();
        return LH_NOTE_SECOND | LH_Text::notify(n,p);
    }

public slots:
    void updateValue()
    {
        if( setText( QString::number(cpu_->coreload(setup_core_selector_->value()-1)/100)+"%" ) )
            callback(lh_cb_render,NULL);
    }

};

LH_PLUGIN_CLASS(LH_TextCPUCoreLoad)
