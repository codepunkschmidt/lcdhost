/**
  \file     LH_NowPlayingText.cpp
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

#include "LH_NowPlayingText.h"

LH_PLUGIN_CLASS(LH_NowPlayingText)

LH_NowPlayingText::LH_NowPlayingText()
{
    connect( currentTrack, SIGNAL(changed()), this, SLOT(refresh_text()) );

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
    setup_item_->setValue(8);
    setup_item_->setHelp("The item to display<br/><br/>Note that not all players provide all data items. If you user such a player you may find a custom string a better choice.");
    connect(setup_item_, SIGNAL(changed()), this, SLOT(setup_item_changed()));

    setup_custom_ = new LH_Qt_QString(this, "^Custom", "{artist}{artist?: \"}{title}{artist?:\"} {status?[}{status}{status?]}", LH_FLAG_FIRST);
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
    connect(setup_custom_, SIGNAL(changed()), this, SLOT(refresh_text()));

    setup_hide_playing_state_ = new LH_Qt_bool(this, "Hide \"Playing\" State", false, LH_FLAG_FIRST);
    setup_hide_playing_state_->setHelp("This changes the behaviour of the \"State\" option, causing it not to display the \"Playing\" value.");
    connect(setup_hide_playing_state_, SIGNAL(changed()), this, SLOT(refresh_text()));

    setup_text_->setName( "Now Playing" );
    setup_text_->setFlag( LH_FLAG_READONLY, true );
    setup_text_->setFlag( LH_FLAG_HIDDEN, true );
    setup_text_->setFlag( LH_FLAG_NOSAVE, true );
    setText( "  " );
    return;
}

lh_class *LH_NowPlayingText::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "3rdParty/Music",
        "NowPlayingText",
        "Now Playing (Text)",
        -1, -1,
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

void LH_NowPlayingText::setup_item_changed()
{
    setup_custom_->setFlag(LH_FLAG_HIDDEN, setup_item_->value()==7);
    refresh_text();
}

void LH_NowPlayingText::refresh_text()
{
    if (currentTrack->playerFound())
    {
        QString template_value = "";
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
            status_text = (setup_hide_playing_state_->value()? "" : "Playing");
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

        if( setText( template_value ) )  callback(lh_cb_render,NULL);
    } else {
        // No player found
        if( setText( " " ) )  callback(lh_cb_render,NULL);
    }
}

void LH_NowPlayingText::replace_token(QString &str, QString token, QString val)
{
    QRegExp re1(QString("\\{%1\\}").arg(token));
    str = str.replace(re1, val);
    QRegExp re2(QString("\\{%1\\?([^}]*)\\}").arg(token));
    if(val.trimmed()!="")
        str = str.replace(re2, "\\1");
    else
        str = str.replace(re2, "");
}

void LH_NowPlayingText::replace_token(QString &str, QString token, int seconds, int totalSeconds)
{
    QTime t = QTime(0,0,0).addSecs(seconds);
    replace_token(str,token,(totalSeconds==0? "" : t.toString((t.hour()>0? "hh:mm:ss" : "mm:ss"))));
}

/*

bool getiTunes(QString& qTitle, QString& qArtist)
{
#ifdef Q_WS_WIN
    HWND hwnd = FindWindowA("iTunes",NULL);
    if (hwnd != 0)
    {
        CoInitializeEx(NULL, COINIT_MULTITHREADED);

        IiTunes *iITunes = 0;
        IITTrack *currentTrack = 0;

        HRESULT hRes = ::CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID *)&iITunes);
        if(hRes == S_OK && iITunes)
        {
            //BSTR* bStrmTitle;
            //HRESULT hResStrm = iITunes->get_CurrentStreamTitle((BSTR *)&bStrmTitle);
            //if(hResStrm == S_OK && iITunes)
            //{
            //    qTitle = QString::fromWCharArray( (LPCTSTR)bStrmTitle );

            //    currentTrack->Release();
            //    iITunes->Release();
            //    return true;
            //}

            if(iITunes->get_CurrentTrack(&currentTrack) == S_OK)
            {

                BSTR* bTitle;
                currentTrack->get_Name((BSTR *)&bTitle);
                qTitle = QString::fromWCharArray( (LPCTSTR)bTitle );

                BSTR* bArtist;
                currentTrack->get_Artist((BSTR *)&bArtist);
                qArtist = QString::fromWCharArray( (LPCTSTR)bArtist );

                //IITArtworkCollection *artColl;
                //if(currentTrack->get_Artwork(&artColl) == S_OK)
                //{
                //    long count;
                //    IITArtwork *art;
                //    artworkCollection->get_Count(&count);
                //    if(count > 0 && artColl->get_Item(1,&art) == S_OK)
                //    {
                //        //http://rainmeter.googlecode.com/svn-history/r542/trunk/Plugins/PluginiTunes/iTunesPlugin.cpp
                //    }
                //}

                currentTrack->Release();
                iITunes->Release();
                return true;
            } else
                iITunes->Release();
        }
        CoUninitialize();
    }
#else
    Q_UNUSED(qTitle);
    Q_UNUSED(qArtist);
#endif
    return false;
}

bool getSpotify(QString& qTitle, QString& qArtist)
{
#ifdef Q_WS_WIN
    //~~~ Spotify ~~~
    HWND hwnd = FindWindowA("SpotifyMainWindow",NULL);
    if (hwnd != 0)
    {
        qArtist = "";
        qTitle = "";
        int nSize = 100;
        CHAR cWindowTitle[nSize];
        //int result =
        GetWindowTextA(hwnd,cWindowTitle,nSize);
        //Q_UNUSED(result)
        QString qWindowTitle = QString(cWindowTitle);
        if(qWindowTitle.length() < 10)
            qTitle = "[Spotify Paused]";
        else
        {
            qWindowTitle = qWindowTitle.right( qWindowTitle.length() - 10 );
            int i;
            for(i=0; i<qWindowTitle.length(); i++)
                if (int(qWindowTitle.at(i).toAscii()) == -106)
                {
                    qArtist = qWindowTitle.left(i).trimmed();
                    qTitle = qWindowTitle.right(qWindowTitle.length()-1 - i).trimmed();
                    break;
                }
        }
        return true;
    }
#else
    Q_UNUSED(qTitle);
    Q_UNUSED(qArtist);
#endif
    return false;
}

bool getiTunes(QString& qTitle, QString& qArtist)
{
#ifdef Q_WS_WIN
    HWND hwnd = FindWindowA("iTunes",NULL);
    if (hwnd != 0)
    {
        CoInitializeEx(NULL, COINIT_MULTITHREADED);

        IiTunes *iITunes = 0;
        IITTrack *currentTrack = 0;

        HRESULT hRes = ::CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID *)&iITunes);
        if(hRes == S_OK && iITunes)
        {
            //BSTR* bStrmTitle;
            //HRESULT hResStrm = iITunes->get_CurrentStreamTitle((BSTR *)&bStrmTitle);
            //if(hResStrm == S_OK && iITunes)
            //{
            //    qTitle = QString::fromWCharArray( (LPCTSTR)bStrmTitle );

            //    currentTrack->Release();
            //    iITunes->Release();
            //    return true;
            //}

            if(iITunes->get_CurrentTrack(&currentTrack) == S_OK)
            {

                BSTR* bTitle;
                currentTrack->get_Name((BSTR *)&bTitle);
                qTitle = QString::fromWCharArray( (LPCTSTR)bTitle );

                BSTR* bArtist;
                currentTrack->get_Artist((BSTR *)&bArtist);
                qArtist = QString::fromWCharArray( (LPCTSTR)bArtist );

                //IITArtworkCollection *artColl;
                //if(currentTrack->get_Artwork(&artColl) == S_OK)
                //{
                //    long count;
                //    IITArtwork *art;
                //    artworkCollection->get_Count(&count);
                //    if(count > 0 && artColl->get_Item(1,&art) == S_OK)
                //    {
                //        //http://rainmeter.googlecode.com/svn-history/r542/trunk/Plugins/PluginiTunes/iTunesPlugin.cpp
                //    }
                //}

                currentTrack->Release();
                iITunes->Release();
                return true;
            } else
                iITunes->Release();
        }
        CoUninitialize();
    }
#else
    Q_UNUSED(qTitle);
    Q_UNUSED(qArtist);
#endif
    return false;
}

bool getWinamp(QString& qTitle, QString& qArtist)
{
#ifdef Q_WS_WIN
    //IPC_GET_PLAYING_TITLE
    Q_UNUSED(qArtist);
    HWND hwnd = FindWindowA("Winamp v1.x",NULL);
    if (hwnd != 0)
    {
        qArtist = "";
        qTitle = "";
        int nSize = 100;
        CHAR cWindowTitle[nSize];
        GetWindowTextA(hwnd,cWindowTitle,nSize);
        QString qWindowTitle = QString(cWindowTitle);

        int playingState = SendMessage(hwnd,WM_WA_IPC,0,IPC_ISPLAYING);
        QRegExp rx("(^[0-9]*\\.\\s)|(?:\\s-\\sWinamp(?:\\s\\[Paused\\])*$)");
        switch(playingState)
        {
        case 3: // paused
        case 1: // playing
            qTitle = qWindowTitle.remove(rx);
            return true;
        case 0: // not playing
            return false;
        }

        //wchar_t *wcTitle = (wchar_t*) SendMessage(hwnd,WM_WA_IPC,0,IPC_GET_PLAYING_TITLE);
        //qTitle = qTitle + QString::fromWCharArray(wcTitle);
        //return true;
    }
#else
    Q_UNUSED(qTitle);
    Q_UNUSED(qArtist);
#endif
    return false;
}

bool getWMP(QString& qTitle, QString& qArtist)
{
#ifdef Q_WS_WIN
    // is there a clue in
    //          http://code.google.com/p/onlylyric/source/browse/trunk/components/winPlayer.cpp?r=2


    HWND hwnd = FindWindowA("WMP Skin Host",NULL);
    if (hwnd != 0)
    {
        CoInitialize(0);

        IWMPCore *spWMPCore;
        IWMPMedia *currentTrack;

        HRESULT hRes = ::CoCreateInstance(CLSID_WindowsMediaPlayer, NULL, CLSCTX_INPROC_SERVER, IID_IWMPCore, (void **)&spWMPCore);
        if(hRes == S_OK && spWMPCore)
        {
            if(spWMPCore->get_currentMedia(&currentTrack) == S_OK)
            {
                BSTR* bTitle;
                currentTrack->get_name((BSTR *)&bTitle);
                qTitle = QString::fromWCharArray( (LPCTSTR)bTitle );

                BSTR* bArtist;
                currentTrack->getItemInfo((BSTR)"Artist",(BSTR *)&bArtist);
                qArtist = QString::fromWCharArray( (LPCTSTR)bArtist );

                currentTrack->Release();
                spWMPCore->Release();
                return true;
            } else
                spWMPCore->Release();
        }
        return false;
    }
#else
    Q_UNUSED(qTitle);
    Q_UNUSED(qArtist);
#endif
    return false;
}
*/
