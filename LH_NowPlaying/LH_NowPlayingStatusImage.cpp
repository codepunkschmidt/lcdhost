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
    setup_image_file_->setHelp("To set image shown, enable conditional formatting and create rules based on the selected item.");

    connect( currentTrack, SIGNAL(changed()), this, SLOT(refresh_value()) );

    setup_item_ = new LH_Qt_QStringList(this, "Item",
                                        QStringList()
                                        <<"Track Title"
                                        <<"Artist"
                                        <<"Album"
                                        <<"Duration"
                                        <<"Time Played"
                                        <<"Time Remaining"
                                        <<"Status"
                                        <<"Player"
                                        <<"Custom"
                                        <<"File Name",
                                        LH_FLAG_FIRST
                                        );
    setup_item_->setValue(6);
    setup_item_->setHelp("The item to base your rules on.<br/><br/>Note that not all players provide all data items. If you user such a player you may find a custom string a better choice.");
    connect(setup_item_, SIGNAL(changed()), this, SLOT(setup_item_changed()));

    setup_custom_ = new LH_Qt_QString(this, "Custom", "{artist}{artist?: \"}{title}{artist?:\"} {status?[}{status}{status?]}", LH_FLAG_FIRST | LH_FLAG_HIDDEN | LH_FLAG_BLANKTITLE);
    setup_custom_->setHelp("Enter a template string.<br/><br/>"
                           "Templates: <br/><br/>"
                           "<table>"
                           "<tr><td>Track Title:</td><td>{title}</td></tr>"
                           "<tr><td>Artist:</td><td>{artist}</td></tr>"
                           "<tr><td>Album:</td><td>{album}</td></tr>"
                           "<tr><td>Duration:</td><td>{duration}</td></tr>"
                           "<tr><td>Time Played</td><td>{played}</td></tr>"
                           "<tr><td>Time Remaining</td><td>{remaining}</td></tr>"
                           "<tr><td>Status</td><td>{status}</td></tr>"
                           "<tr><td>Player</td><td>{player}</td></tr>"
                           "</table><br/><br/>"
                           "Additionally you can include text conditionally using {<token>?<text>} , <br/>"
                           "e.g.: {album?Album: \"}{album}{album?\"} <br/>"
                           "This displays nothing when no album name is provided; when an album name is available however it will show: <br/>"
                           "Album: \"<album name>\"<br/>"
                           );
    connect(setup_custom_, SIGNAL(changed()), this, SLOT(refresh_value()));


    setup_value_ = new LH_Qt_QString(this, "Select Item Value", "", LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_NOSAVE | LH_FLAG_READONLY | LH_FLAG_FIRST | LH_FLAG_HIDETITLE);

    add_cf_target(setup_image_file_);
    add_cf_source(setup_value_);

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

void LH_NowPlayingStatusImage::setup_item_changed()
{
    setup_custom_->setFlag(LH_FLAG_HIDDEN, setup_item_->value()!=8);
    refresh_value();
}

void LH_NowPlayingStatusImage::refresh_value()
{
    QString template_value = "";
    if (currentTrack->playerFound())
    {
        switch(setup_item_->value())
        {
        case 0:     //Track Title
            template_value = "{title}";
            break;
        case 1:     //Artist
            template_value = "{artist}";
            break;
        case 2:     //Album
            template_value = "{album}";
            break;
        case 3:     //Duration
            template_value = "{duration}";
            break;
        case 4:     //Time Played
            template_value = "{played}";
            break;
        case 5:     //Time Remaining
            template_value = "{remaining}";
            break;
        case 6:     //Status
            template_value = "{status}";
            break;
        case 7:     //Player
            template_value = "{player}";
            break;
        case 9:     //File
            template_value = "{file}";
            break;
        //case 8:     //Custom
        default:
            template_value = setup_custom_->value();
        }

        QString status_text = "";
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

        replace_token(template_value,"title",currentTrack->info().track);
        replace_token(template_value,"artist",currentTrack->info().artist);
        replace_token(template_value,"album",currentTrack->info().album);
        replace_token(template_value,"duration",currentTrack->info().totalSecs,currentTrack->info().totalSecs);
        replace_token(template_value,"played",currentTrack->info().currentSecs,currentTrack->info().totalSecs);
        replace_token(template_value,"remaining",currentTrack->info().totalSecs - currentTrack->info().currentSecs,currentTrack->info().totalSecs);
        replace_token(template_value,"status",status_text);
        replace_token(template_value,"player",currentTrack->info().player);
        replace_token(template_value,"file",currentTrack->info().file);
    }
    setup_value_->setValue( template_value );
}

void LH_NowPlayingStatusImage::replace_token(QString &str, QString token, QString val)
{
    QRegExp re1(QString("\\{%1\\}").arg(token));
    str = str.replace(re1, val);
    QRegExp re2(QString("\\{%1\\?([^}]*)\\}").arg(token));
    if(val.trimmed()!="")
        str = str.replace(re2, "\\1");
    else
        str = str.replace(re2, "");
}

void LH_NowPlayingStatusImage::replace_token(QString &str, QString token, int seconds, int totalSeconds)
{
    QTime t = QTime(0,0,0).addSecs(seconds);
    replace_token(str,token,(totalSeconds==0? "" : t.toString((t.hour()>0? "hh:mm:ss" : "mm:ss"))));
}
