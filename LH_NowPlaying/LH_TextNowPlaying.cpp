/**
  \file     LH_TextNowPlaying.cpp
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese
    This module is based on original work by Johan Lindh. API files are (c) of
    their respective authors (see the api files for more information)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.

  */
#include <QFont>
#include <QFontMetrics>
#include <QTime>
#include <QRegExp>

#include <windows.h>

#include "iTunesCOMInterface.h"
#include "wa_ipc.h"
#include "wmp.h"

#include "../LH_Text/LH_Text.h"



class LH_TextNowPlaying : public LH_Text
{
    QTime t;
public:
    LH_TextNowPlaying(const char* name) : LH_Text(name)
    {
        setup_text_->setName( "Now Playing" );
        setup_text_->setFlag( LH_FLAG_READONLY, true );
        setText( "?%" );
        t.start();
        return;
    }

    static lh_class *classInfo()
    {
        static lh_class classInfo =
        {
            sizeof(lh_class),
            "3rdParty/Music",
            "NowPlayingText",
            "Now Playing (iTunes/Spotify/Winamp)",
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

    int notify(int code,void* param)//polling()
    {
        Q_UNUSED(code);
        Q_UNUSED(param);
        if (t.elapsed()>=500)
        {
            t.restart();

            QString qTitle = "";
            QString qArtist = "";

            bool playerFound = false;
            //playerFound = playerFound || getWMP(qTitle, qArtist);
            playerFound = playerFound || getiTunes(qTitle, qArtist);
            playerFound = playerFound || getWinamp(qTitle, qArtist);
            //Spotify goes last because if it's open & playing nothing it's the same as if it's paused.
            playerFound = playerFound || getSpotify(qTitle, qArtist);


            if (playerFound)
            {
                QString qOutput = qTitle;
                if (qArtist!="") qOutput = qArtist + ": \""+qTitle+"\"";
                if( setText( qOutput ) )  callback(lh_cb_render,NULL);
            } else {
                // No player found
                if( setText( " " ) )  callback(lh_cb_render,NULL);
            }
        }
        //return 500;
        return LH_NOTE_SECOND | LH_NOTE_CPU | LH_NOTE_MEM | LH_NOTE_NET;
    }


    /*QString getWindowClass()
    {
        HWND hwnd = FindWindowA(NULL,"Winamp 5.581");
        if (hwnd != 0)
        {
            WCHAR winTitle[100];
            GetClassName(hwnd,winTitle,100);
            return QString::fromWCharArray(winTitle);
        } else
            return "Not found.";
    }*/

    bool getSpotify(QString& qTitle, QString& qArtist)
    {
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
        else
            return false;
    }
    bool getiTunes(QString& qTitle, QString& qArtist)
    {
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

                    /*IITArtworkCollection *artColl;
                    if(currentTrack->get_Artwork(&artColl) == S_OK)
                    {
                        long count;
                        IITArtwork *art;
                        artworkCollection->get_Count(&count);
                        if(count > 0 && artColl->get_Item(1,&art) == S_OK)
                        {
                            //http://rainmeter.googlecode.com/svn-history/r542/trunk/Plugins/PluginiTunes/iTunesPlugin.cpp
                        }
                    }*/

                    currentTrack->Release();
                    iITunes->Release();
                    return true;
                } else
                    iITunes->Release();
            }
            CoUninitialize();
        }
        return false;
    }
    bool getWinamp(QString& qTitle, QString& qArtist)
    {
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
        return false;
    }

    bool getWMP(QString& qTitle, QString& qArtist)
    {        
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
        return false;
    }
};

LH_PLUGIN_CLASS(LH_TextNowPlaying)

