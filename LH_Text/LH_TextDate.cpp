/**
  \file     LH_TextDate.cpp
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009-2010 Johan Lindh

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
#include <QDate>
#include <QFont>
#include <QFontMetrics>

#include "LH_Text.h"

class LH_TextDate : public LH_Text
{
protected:
    LH_Qt_QString *setup_format_;

public:
    const char *userInit()
    {
        if( const char *err = LH_Text::userInit() ) return err;
        setup_format_ = new LH_Qt_QString(this,"<a href=\"http://doc.trolltech.com/4.6/qdate.html#toString\">Format</a>","yyyy-MM-dd dddd");
        setup_format_->setHelp("<p>Format to use. Common formats include:</p>"
                               "<ul>"
                               "<li>yyyy-MM-dd dddd</li>"
                               "<li>MM/dd/yyyy</li>"
                               "<li>ddd MMM d yyyy</li>"
                               "</ul>");
        setup_text_->setTitle("Current date");
        setup_text_->setFlag(LH_FLAG_READONLY,true);
        return 0;
    }

    /**
      We'll use the notify LH_NOTE_SECOND to make sure we're up-to-date,
      but we might also have elected to use polling() with a long interval:

        \code
        QTime::currentTime().msecsTo( QTime( 23, 59, 59, 999 ) ) + 10
        \endcode

      In any case, we only redo the rendering if the text actually changed.
      */
    int notify(int n,void* p)
    {
        Q_UNUSED(p);
        if( !n || n&LH_NOTE_SECOND )
        {
            QDateTime now = QDateTime::currentDateTime();
            if( now.time().msec() > 500 ) now = now.addSecs(1);
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
            "SystemDateText",
            "Date (Text)",
            -1, -1,
            
            
        };

        if( classInfo.width == -1 )
        {
            QFont font;
            QFontMetrics fm( font );
            classInfo.height = fm.height();
            classInfo.width = fm.width("9999-99-99 weekday");
        }

        return &classInfo;
    }
};

LH_PLUGIN_CLASS(LH_TextDate)
