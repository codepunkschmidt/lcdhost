/**
  \file     LH_TextNumber.cpp
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2010 Johan Lindh

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

#include "LH_TextNumber.h"

const char *LH_TextNumber::userInit()
{
    if( const char *err = LH_Text::userInit() ) return err;
    bytes_ = false;
    setup_text_->setFlag( LH_FLAG_READONLY, true );
    setup_value_ = new LH_Qt_double(this,"Value",0,LH_FLAG_AUTORENDER);
    setup_value_->setHelp("Link a data source to this value using the button on the left.");
    setup_bits_ = new LH_Qt_bool(this,"Bits instead of bytes",false,LH_FLAG_AUTORENDER|LH_FLAG_FIRST|LH_FLAG_HIDDEN);
    setup_bits_->setHelp("<p>If this is selected, the value will be shown in bits rather than bytes.</p>");
    setup_showsuffix_ = new LH_Qt_bool(this,"Show multiplier",true,LH_FLAG_AUTORENDER|LH_FLAG_FIRST);
    setup_showunits_ = new LH_Qt_bool(this,"Show units",true,LH_FLAG_AUTORENDER|LH_FLAG_FIRST);
    setup_scale_ = new LH_Qt_QStringList(this,"Scale",
                        QStringList("Automatic")<<"Percentage"<<"No scaling"<<"Kilo"<<"Mega"<<"Giga"<<"Tera",
                        LH_FLAG_AUTORENDER|LH_FLAG_FIRST );
    return 0;
}

bool LH_TextNumber::makeText()
{
    double scale = 1.0;
    if( setup_bits_->value() ) scale = 8.0;
    return setNum(
                setup_value_->value()*scale,
                setup_scale_->value(),
                setup_showsuffix_->value(),
                setup_value_->max()*scale,
                bytes_);
}
