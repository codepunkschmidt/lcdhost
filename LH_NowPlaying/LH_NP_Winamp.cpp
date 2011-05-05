/**
  \file     LH_NP_Winamp.cpp
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
 * winamp.c
 * retrieve track info being sent from winamp
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

#ifndef LH_NP_WINAMP
#define LH_NP_WINAMP

#include <QDebug>
#include <QRegExp>
#include <windows.h>
#include "wa_ipc.h"

#include "LH_QtPlugin_NowPlaying.h"

static HWND hWnd;
static HANDLE hProcess;

static
bool winamp_get_w(const wchar_t *filename, const wchar_t *key, QString &dest)
{
    // Allocate memory inside Winamp's address space to exchange data with it
    void *winamp_info = VirtualAllocEx(hProcess, NULL, 4096, MEM_COMMIT, PAGE_READWRITE);
    if (!winamp_info)
      {
#ifndef QT_NO_DEBUG
        qDebug() << "VirtualAllocEx in winamp process failed";
#endif
        return false;
      }

    wchar_t *winamp_filename = (wchar_t*)(winamp_info+1024);
    wchar_t *winamp_key = (wchar_t*)(winamp_info+2048);
    wchar_t *winamp_value = (wchar_t*)(winamp_info+3072);

    // Setup structure with pointers into Winamp address space and store it into that space too
    extendedFileInfoStructW info;
    info.filename = winamp_filename;
    info.metadata = winamp_key;
    info.ret = winamp_value;
    info.retlen = 1024/sizeof(wchar_t);
    WriteProcessMemory(hProcess, winamp_info, &info, sizeof(info), NULL);

    WriteProcessMemory(hProcess, winamp_filename, filename, sizeof(wchar_t)*(wcslen(filename)+1), NULL);
    WriteProcessMemory(hProcess, winamp_key, key, sizeof(wchar_t)*(wcslen(key)+1), NULL);
    int rc = SendMessage(hWnd, WM_WA_IPC, (WPARAM)winamp_info, IPC_GET_EXTENDED_FILE_INFOW);

    SIZE_T bytesRead;
    wchar_t wdest[STRLEN];
    ReadProcessMemory(hProcess, winamp_value, wdest, (STRLEN-1)*sizeof(wchar_t), &bytesRead);
    wdest[bytesRead/sizeof(wchar_t)] = 0;

    dest = QString::fromWCharArray(wdest);
    //WideCharToMultiByte(CP_UTF8, 0, wdest, -1, dest, STRLEN, NULL, NULL);
#ifndef QT_NO_DEBUG
    qDebug() << "Got info '" << dest << "', return value " << rc;
#endif
    VirtualFreeEx(hProcess, winamp_info, 0, MEM_RELEASE);

    return (rc != 1);

}

static
bool winamp_get(const char *filename, const char *key, QString &dest)
{
    // Allocate memory inside Winamp's address space to exchange data with it
    void *winamp_info = VirtualAllocEx(hProcess, NULL, 4096, MEM_COMMIT, PAGE_READWRITE);
    if (!winamp_info)
      {
#ifndef QT_NO_DEBUG
        qDebug() << "VirtualAllocEx in winamp process failed";
#endif
        return FALSE;
      }

    char *winamp_filename = (char*)(winamp_info+1024);
    char *winamp_key = (char*)(winamp_info+2048);
    char *winamp_value = (char*)(winamp_info+3072);

    // Setup structure with pointers into Winamp address space and store it into that space too
    extendedFileInfoStruct info;
    info.filename = winamp_filename;
    info.metadata = winamp_key;
    info.ret = winamp_value;
    info.retlen = 1024;
    WriteProcessMemory(hProcess, winamp_info, &info, sizeof(info), NULL);

    WriteProcessMemory(hProcess, winamp_filename, filename, strlen(filename)+1, NULL);
    WriteProcessMemory(hProcess, winamp_key, key, strlen(key)+1, NULL);
    int rc = SendMessage(hWnd, WM_WA_IPC, (WPARAM)winamp_info, IPC_GET_EXTENDED_FILE_INFO);

    char cdest[100];
    SIZE_T bytesRead;
    ReadProcessMemory(hProcess, winamp_value, cdest, STRLEN-1, &bytesRead);
    cdest[bytesRead] = 0;
    dest = QString(cdest);

#ifndef QT_NO_DEBUG
    qDebug() << "Got info for key '" << key << "' is '" << dest << "', return value " << rc;
#endif
    VirtualFreeEx(hProcess, winamp_info, 0, MEM_RELEASE);

    return (rc != 1);
}

bool
get_winamp_info(TrackInfo& ti)
{
    ti.status = PLAYER_STATUS_CLOSED;
    hWnd = FindWindowA("Winamp v1.x", NULL);
    if (!hWnd) {
        return false;
    }

    ti.player = "Winamp";
    int version = SendMessage(hWnd, WM_WA_IPC, 0, IPC_GETVERSION);
#ifndef QT_NO_DEBUG
    qDebug() << "Winamp version " << version;
#endif
    DWORD processId;
    GetWindowThreadProcessId(hWnd, &processId);
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, processId);
    if (!hProcess)
    {
#ifndef QT_NO_DEBUG
        qDebug() << "Failed to open winamp process";
#endif
    }

    int playing = SendMessage(hWnd, WM_WA_IPC, 1, IPC_ISPLAYING);
    if (playing == 0)
        ti.status = PLAYER_STATUS_STOPPED;
    else if (playing == 3)
        ti.status = PLAYER_STATUS_PAUSED;
    else
        ti.status = PLAYER_STATUS_PLAYING;

    ti.totalSecs = SendMessage(hWnd, WM_WA_IPC, 1, IPC_GETOUTPUTTIME);
    ti.currentSecs = SendMessage(hWnd, WM_WA_IPC, 0, IPC_GETOUTPUTTIME)/1000;

    int position = SendMessage(hWnd, WM_WA_IPC, 0, IPC_GETLISTPOS);

    // first try wchar interface
    // AIMP2 supports IPC_GETPLAYLISTFILEW but not IPC_GET_EXTENDED_FILE_INFOW
    // detect that by the winamp version number it reports, and use the fallback char interface
    LPCVOID address = (LPCVOID) SendMessage(hWnd, WM_WA_IPC, position, IPC_GETPLAYLISTFILEW);
    if (((unsigned int)address > 1) && (version != 8345))
    {
        wchar_t wfilename[512];
        ReadProcessMemory(hProcess, address, wfilename, 512, 0);
#ifndef QT_NO_DEBUG
        qDebug() << "Filename(widechar): " << QString::fromWCharArray(wfilename);
#endif
        winamp_get_w(wfilename, L"ALBUM", ti.album);
        winamp_get_w(wfilename, L"ARTIST", ti.artist);
        winamp_get_w(wfilename, L"TITLE", ti.track);
    }
    else
    {
        // if that fails, fall back to char interface
        // (this is not preferred as it cannot support east asian characters)
        address = (LPCVOID) SendMessage(hWnd, WM_WA_IPC, position, IPC_GETPLAYLISTFILE);
        if ((unsigned int)address > 1)
        {
            char filename[512];
            ReadProcessMemory(hProcess, address, filename, 512, 0);
#ifndef QT_NO_DEBUG
            qDebug() << "Filename: " << filename;
#endif
            winamp_get(filename, "ALBUM", ti.album);
            winamp_get(filename, "ARTIST", ti.artist);
            winamp_get(filename, "TITLE", ti.track);
        }
    }

    // if these are all empty, which seems to happen when listening to a stream, try something cruder
    // XXX: really should try to work out how to get winamp to resolve it's tag %streamtitle% for us...
    if ((ti.album.length() == 0) && (ti.artist.length() == 0) && (ti.track.length() == 0))
    {
        QString title = GetWindowTitle(hWnd);
        QRegExp re("^\\d*\\. (.*)$");
        re.setMinimal(true);
#ifndef QT_NO_DEBUG
        qDebug()<<"Got stream track: " << re.indexIn(title);
#endif
        if(re.indexIn(title) != -1)
            ti.track = re.cap(1).remove(QRegExp(" - Winamp$"));
#ifndef QT_NO_DEBUG
        qDebug()<<"Got stream track: " << ti.track;
#endif
    }

    CloseHandle(hProcess);
    ti.updatedAt = QDateTime::currentDateTime();
    return true;
}

#endif //LH_NP_WINAMP
