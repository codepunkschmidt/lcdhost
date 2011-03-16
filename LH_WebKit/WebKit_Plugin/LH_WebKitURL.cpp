/**
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

#include "LH_WebKitURL.h"

LH_PLUGIN_CLASS(LH_WebKitURL)

lh_class *LH_WebKitURL::classInfo()
{
    static lh_class classinfo =
    {
        sizeof(lh_class),
        "Dynamic",
        "DynamicWebKitURL",
        "WebKit URL",
        96, 32,
        lh_instance_calltable_NULL
    };

    return &classinfo;
}

LH_WebKitURL::LH_WebKitURL(const char *name) : LH_WebKit(name)
{
    setup_url_ = new LH_Qt_QString(this,"URL",QString(),LH_FLAG_FOCUS);
    connect( setup_url_, SIGNAL(changed()), this, SLOT(urlChanged()) );
    setup_url_sanitized_ = new LH_Qt_QString(this,"Sanitized URL",QString(),LH_FLAG_READONLY);
}

void LH_WebKitURL::urlChanged()
{
    url_ = QUrl::fromUserInput( setup_url_->value() );
    if( url_.isValid() )
    {
        setup_url_sanitized_->setValue( url_.toString() );
        sendRequest( url_ );
    }
    else setup_url_sanitized_->setValue( QString() );
    return;
}
