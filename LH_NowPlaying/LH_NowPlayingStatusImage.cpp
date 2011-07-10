/**
  \file     LH_NowPlayingStatusImage.cpp
  \author   Johan Lindh <johan@linkdata.se>
  \author   Andy Bridges <andy@bridgesuk.com>
  \legalese Copyright (c) 2010 Johan Lindh, Andy Bridges

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

  **/

#include "LH_NowPlayingStatusImage.h"

LH_PLUGIN_CLASS(LH_NowPlayingStatusImage)

const char *LH_NowPlayingStatusImage::userInit()
{
    if( const char *err = LH_QImage::userInit() ) return err;

    setup_image_file_->setFlags( LH_FLAG_AUTORENDER | LH_FLAG_READONLY | LH_FLAG_NOSAVE );
    setup_image_file_->setHelp("To set image shown, enable conditional formatting and create rules based on the Playing State.");

    connect( currentTrack, SIGNAL(changed()), this, SLOT(refresh_status()) );

    setup_playing_state_ = new LH_Qt_QString(this, "Playing State", "", LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_NOSAVE | LH_FLAG_READONLY);

    add_cf_target(setup_image_file_);
    add_cf_source(setup_playing_state_);

    return 0;
}

lh_class *LH_NowPlayingStatusImage::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "3rdParty/Music",
        "NowPlayingStatusImage",
        "Now Playing (Status Image)",
        -1, -1,
        lh_instance_calltable_NULL
    };

    return &classInfo;
}

void LH_NowPlayingStatusImage::refresh_status()
{
    QString status_text = "";
    if (currentTrack->playerFound())
    {
        switch(currentTrack->info().status)
        {
        case PLAYER_STATUS_CLOSED:
            status_text = "";
            break;
        case PLAYER_STATUS_STOPPED:
            status_text = "Stopped";
            break;
        case PLAYER_STATUS_PAUSED:
            status_text = "Paused";
            break;
        case PLAYER_STATUS_PLAYING:
            status_text = "Playing";
            break;
        }
    }
    setup_playing_state_->setValue( status_text );
}
