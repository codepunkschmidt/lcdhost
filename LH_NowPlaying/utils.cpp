/**
  \file     LH_NP_MSN_Compat.cpp
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
 * utils.c
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

#ifndef UTILS
#define UTILS

#define PLAYER_STATUS_INVALID INT_MIN
#define PLAYER_STATUS_CLOSED -1
#define PLAYER_STATUS_STOPPED 0
#define PLAYER_STATUS_PAUSED 1
#define PLAYER_STATUS_PLAYING 2

#define STRLEN 100

#include <QTime>

struct TrackInfo
{
        QString track;
        QString artist;
        QString album;
        QString player;
        int status;
        int totalSecs;
        int currentSecs;
        QDateTime updatedAt;
};


#ifdef WIN32
#include <QString>
#include <QDebug>
# include <windows.h>


static QString GetWindowTitle(HWND hWnd)
{
  int title_length = GetWindowTextLengthW(hWnd)+1;
  wchar_t wtitle[title_length];
  GetWindowTextW(hWnd, wtitle, title_length);
  QString title = QString::fromWCharArray(wtitle);
  //qDebug() << "Got window title: " << title;
  return title;
}

#endif
#endif
