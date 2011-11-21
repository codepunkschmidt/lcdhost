/**
  \file     LH_QtPlugin_NowPlaying.h
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


#ifndef LH_QTPLUGIN_NOWPLAYING_H
#define LH_QTPLUGIN_NOWPLAYING_H

#include "LH_QtPlugin.h"
#include "LH_Qt_QString.h"
#include "LH_Qt_InputState.h"
#include "LH_Qt_bool.h"
#include "Player.h"


#include "PlayerITunes.h"
#include "PlayerWinamp.h"
#include "PlayerFoobar.h"
#include "PlayerSpotify.h"
#include "PlayerVLC.h"
#include "PlayerWLM.h"

#include <QTimer>
#include <QDebug>

extern CPlayer* player;
extern ArtworkCache* artworkCache;
extern bool isElevated;

class LH_QtPlugin_NowPlaying : public LH_QtPlugin
{
    Q_OBJECT
    QTimer timer_;

    QString getWindowClass(LPCSTR windowCaption)
    {
        HWND hwnd = FindWindowA(NULL,windowCaption);
        if (hwnd != 0)
        {
            WCHAR winTitle[100];
            GetClassName(hwnd,winTitle,100);
            return QString::fromWCharArray(winTitle);
        } else
            return "Not found.";
    }

    bool getElevated()
    {
        typedef BOOL (WINAPI *LPFNIUA)(void);

        HMODULE hShell32 = LoadLibrary(L"shell32.dll");
        LPFNIUA pIsAdmin = NULL;

        if (hShell32)
        {
            pIsAdmin = (LPFNIUA)GetProcAddress(hShell32, MAKEINTRESOURCEA(680));
            if (pIsAdmin)
            {
                if(pIsAdmin())
                    return true;
                else
                    return false;
            }
            FreeLibrary(hShell32);
        }
        return false;
    }

    bool playerControlCheck();
public:
    LH_Qt_QString *setup_control_play_pause_;
    LH_Qt_QString *setup_control_stop_;
    LH_Qt_QString *setup_control_next_;
    LH_Qt_QString *setup_control_prev_;
    LH_Qt_QString *setup_control_close_;
    LH_Qt_QString *setup_control_repeat_;
    LH_Qt_QString *setup_control_shuffle_;

    LH_Qt_InputState *setup_input_play_pause_;
    LH_Qt_InputState *setup_input_stop_;
    LH_Qt_InputState *setup_input_next_;
    LH_Qt_InputState *setup_input_prev_;
    LH_Qt_InputState *setup_input_shuffle_;
    LH_Qt_InputState *setup_input_repeat_;
    LH_Qt_InputState *setup_input_close_;

    LH_Qt_bool *setup_media_keys_iTunes_;
    LH_Qt_bool *setup_media_keys_Winamp_;
    LH_Qt_bool *setup_media_keys_Foobar_;
    LH_Qt_bool *setup_media_keys_Spotify_;
    LH_Qt_bool *setup_media_keys_VLC_;
    LH_Qt_bool *setup_media_keys_WLM_;

    const char *userInit();
    void userTerm();
    void clearPlayer();

public slots:
    void refresh_data();
    void controlPlayPauseClick(QString key="",int flags=0,int value=0);
    void controlStopClick(QString key="",int flags=0,int value=0);
    void controlNextClick(QString key="",int flags=0,int value=0);
    void controlPrevClick(QString key="",int flags=0,int value=0);
    void controlCloseClick();
    void controlRepeatClick();
    void controlShuffleClick();

signals:
    void updated_data();
};

#endif // LH_QTPLUGIN_NOWPLAYING_H
