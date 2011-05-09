/**
  \file     LH_NP_iTunes.cpp
  \author   Andy Bridges <andy@bridgesuk.com>

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

  Please note that this file is a modified version of the file outlined below,
  whose ownership, copyright and limitiations remain unaltered.

  **/
/*
 * musictracker
 * itunes.c
 * retrieve track info being sent from iTunes
 *
 * Copyright (C) 2008, Jon TURNEY <jon.turney@dronecode.org.uk>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02111-1301, USA.
 *
 */

#include <QDebug>
#include <QFile>
#include <windows.h>
#include "iTunesCOMInterface.h"

#include "LH_QtPlugin_NowPlaying.h"

//#define iTunes_debug

static IiTunes *itunes = NULL;

bool open_itunes()
{
    if(itunes==NULL)
    {
    #ifdef iTunes_debug
        qDebug() << "iTunes: Preparing to connect";
    #endif
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    #ifdef iTunes_debug
        qDebug() << "iTunes: COM Initialized";
    #endif

    if (FindWindowA("iTunes", NULL))
    {
        if (CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID *) &itunes) != S_OK) {
            #ifdef iTunes_debug
                qDebug() << "iTunes: Failed to get interface";
            #endif
            CoUninitialize();
            return false;
        }
    } else {
        #ifdef iTunes_debug
            qDebug() << "iTunes: Window not found?!";
        #endif
        CoUninitialize();
        return false;
    }

    #ifdef iTunes_debug
        qDebug() << "iTunes: Connected OK";
    #endif
    }
    return true;
}

void close_itunes()
{
    if(itunes!=NULL)
    {
        #ifdef iTunes_debug
            qDebug() << "iTunes: Preparing to disconnect";
        #endif
        itunes->Release();
        CoUninitialize();
        #ifdef iTunes_debug
            qDebug() << "iTunes: Disconnected Cleanly";
        #endif
        itunes=NULL;
    }
}

bool save_itunes_artwork(IITTrack *track, QString artworkPath, artworkDescription &cachedArtwork, artworkDescription &trackArtwork)
{
    bool success = false;

    if(cachedArtwork.album == trackArtwork.album && cachedArtwork.artist == trackArtwork.artist)
        return false;

    #ifdef iTunes_debug
        qDebug() << "iTunes: artwork: Get Artwork";
    #endif

    IITArtworkCollection *artColl;
    switch (track->get_Artwork(&artColl))
    {
    case S_FALSE:
        #ifdef iTunes_debug
            qDebug() << "iTunes: artwork: Artwork unvailable (1)";
        #endif
        break;
    case S_OK:
        #ifdef iTunes_debug
            qDebug() << "iTunes: artwork: Artwork list acquired";
        #endif
        long artCount = 0;
        switch (artColl->get_Count(&artCount))
        {
        case S_FALSE:
            #ifdef iTunes_debug
                qDebug() << "iTunes: artwork: No count unvailable";
            #endif
            break;
        case S_OK:
            #ifdef iTunes_debug
                qDebug() << "iTunes: artwork: Count = " << artCount;
            #endif
            break;
        }
        if(artCount>=1)
        {
            IITArtwork *artItem;
            switch (artColl->get_Item(1, &artItem))
            {
            case S_FALSE:
                #ifdef iTunes_debug
                    qDebug() << "iTunes: artwork: Artwork unvailable (2)";
                #endif
                break;
            case S_OK:
                #ifdef iTunes_debug
                    qDebug() << "iTunes: artwork: Artwork item acquired";
                #endif
                ITArtworkFormat fmt;
                QString extension = "png";

                switch(artItem->get_Format(&fmt))
                {
                case S_FALSE:
                    #ifdef iTunes_debug
                        qDebug() << "iTunes: artwork: format unavailable";
                    #endif
                    break;
                case S_OK:
                    switch(fmt)
                    {
                    case ITArtworkFormatJPEG:
                        extension = "jpg";
                        break;
                    case ITArtworkFormatPNG:
                        extension = "png";
                        break;
                    case ITArtworkFormatBMP:
                        extension = "png";
                        break;
                    case ITArtworkFormatUnknown:
                    default:
                        extension = "unknown";
                        break;
                    }
                    #ifdef iTunes_debug
                        qDebug() << "iTunes: artwork: format is " << extension;
                    #endif
                    if (extension=="unknown") extension = "jpg";
                    break;
                }

                if(QFile::exists(cachedArtwork.fileName))
                    QFile::remove(cachedArtwork.fileName);

                trackArtwork.fileName = QString("%0%1art.%2").arg(artworkPath).arg(artworkPath.endsWith("/")? "" : "/").arg(extension);
                switch (artItem->SaveArtworkToFile(::SysAllocString((const OLECHAR*)(trackArtwork.fileName.replace("/","\\")).utf16())))
                {
                case S_FALSE:
                    #ifdef iTunes_debug
                        qDebug() << "iTunes: artwork: saving failed";
                    #endif
                    break;
                case S_OK:
                    #ifdef iTunes_debug
                        qDebug() << "iTunes: artwork: saving completed";
                    #endif
                    cachedArtwork = trackArtwork;
                    success = true;
                    break;
                }
                break;
            }
        }
        break;
    }

    return success;
}




bool
get_itunes_info(TrackInfo &ti, QString artworkPath, artworkDescription &cachedArtwork, bool &updatedArtwork)
{
    ti.status = PLAYER_STATUS_CLOSED;

    if (!FindWindowA("iTunes", NULL))
    {
        close_itunes();
        return false;
    }

    if(!open_itunes())
        return false;

    bool success = false;

    ITPlayerState state;
    if (itunes->get_PlayerState(&state) == S_OK)
    {
        if (state == ITPlayerStatePlaying)
            ti.status = PLAYER_STATUS_PLAYING;
        else if (state == ITPlayerStateStopped)
            ti.status = PLAYER_STATUS_PAUSED;
        else
            ti.status = PLAYER_STATUS_STOPPED;

        // state ITPlayerStateStopped && get_CurrentTrack() succeeds -> PLAYER_STATUS_PAUSED
        // state ITPlayerStateStopped && get_CurrentTrack() fails -> PLAYER_STATUS_STOPPED

        IITTrack *track;
        switch (itunes->get_CurrentTrack(&track))
        {
        case S_FALSE:
            #ifdef iTunes_debug
                qDebug() << "iTunes: Not playing anything";
            #endif
            ti.status = PLAYER_STATUS_STOPPED;
            break;
         case S_OK:
            #ifdef iTunes_debug
                qDebug() << "iTunes: Now Playing data acquired!";
            #endif
            ti.player = "iTunes";
            success = true;

            BSTR bstr;
            track->get_Artist(&bstr);
            ti.artist = QString::fromWCharArray(bstr);
            track->get_Album(&bstr);
            ti.album = QString::fromWCharArray(bstr);
            track->get_Name(&bstr);
            ti.track = QString::fromWCharArray(bstr);

            long duration = 0;
            track->get_Duration(&duration);
            ti.totalSecs = duration;

            long position = 0;
            itunes->get_PlayerPosition(&position);
            ti.currentSecs = position;

            ti.updatedAt = QDateTime::currentDateTime();

            if(artworkPath != "")
            {
                artworkDescription trackArtwork = (artworkDescription){ti.artist, ti.album};
                updatedArtwork = save_itunes_artwork(track, artworkPath, cachedArtwork, trackArtwork);
            }

            track->Release();
            break;
        }
    } else {
        #ifdef iTunes_debug
            qDebug() << "iTunes: unable to pull playerstate";
        #endif
        if (!FindWindowA("iTunes", NULL))
            close_itunes();
    }

    return success;
}
