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
 * msn-compat.c
 * retrieve track info being sent to MSN
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

#include <QRegExp>
#include <windows.h>
#include "LH_QtPlugin_NowPlaying.h"

#ifndef WIN32
//??
#else
#define ENABLE_NLS 0 //1
#endif

#include "gettext.h"
#define _(String) dgettext (PACKAGE, String)

// put up a hidden window called "MsnMsgrUIManager", which things know to send a WM_COPYDATA message to,
// containing strangely formatted string, for setting the now-playing status of MSN Messenger
// this might be broken by the sending side only sending to the first window of the class "MsnMsgrUIManager"
// it finds if there are more than one (for e.g., if the real MSN Messenger is running as well :D)
//
// Useful description of the message format: http://kentie.net/article/nowplaying/index.htm
// although we rely on the fact that players generate a very limited subset of the possible
// messages to be able to parse the message back into the track details...

static struct TrackInfo msnti;

static
void process_message(wchar_t *MSNTitle)
{
    QString s = QString::fromWCharArray(MSNTitle);
    QString player = ""; QString enabled = "0"; QString format = "";
    QString artist = ""; QString title = ""; QString album = ""; QString uuid = "";

  // this has to be escaped quite carefully to prevent literals being interpreted as metacharacters by the compiler or in the pcre pattern
  // so yes, four \ before a 0 is required to match a literal \0 in the regex :-)
  // and marking the regex as ungreedy is a lot easier than writing \\\\0([^\\\\0]*)\\\\0 :)
  QRegExp re1("^(.*)\\\\0Music\\\\0(.*)\\\\0(.*)\\\\0(.*)\\\\0(.*)\\\\0(.*)\\\\0(.*)\\\\0");
  re1.setMinimal(true);
  QRegExp re2("^(.*)\\\\0Music\\\\0(.*)\\\\0(.*)\\\\0(.*)\\\\0(.*)\\\\0(.*)\\\\0");
  re2.setMinimal(true);
  QRegExp re3("^(.*)\\\\0Music\\\\0(.*)\\\\0(.*)\\\\0(.*)\\\\0(.*)\\\\0");
  re3.setMinimal(true);
  QRegExp re4("^(.*)\\\\0Music\\\\0(.*)\\\\0(.*) - (.*)\\\\0$");

  if (re1.indexIn(s) != -1)
    {
      player = re1.cap(1); enabled = re1.cap(2); format = re1.cap(3);
      artist = re1.cap(4); title = re1.cap(5); album = re1.cap(6); uuid = re1.cap(7);
#ifndef QT_NO_DEBUG
      qDebug() << "player '" << player << "', enabled '" << enabled << "', format '" << format << "', title '" << title << "', artist '" << artist << "', album '" << album << "', uuid '" << uuid << "'";
#endif
      msnti.player = player;
      msnti.artist = artist;
      msnti.album = album;
      msnti.track = title;
    }
  else if (re2.indexIn(s) != -1)
    {
      player = re2.cap(1); enabled = re2.cap(2); format = re2.cap(3);
      title = re2.cap(4); artist = re2.cap(5); album = re2.cap(6);
#ifndef QT_NO_DEBUG
      qDebug() << "player '" << player << "', enabled '" << enabled << "', format '" << format << "', title '" << title << "', artist '" << artist << "', album '" << album << "'";
#endif
      msnti.player = player;
      msnti.artist = artist;
      msnti.album = album;
      msnti.track = title;
    }
  else if (re3.indexIn(s) != -1)
    {
      player = re3.cap(1); enabled = re3.cap(2); format = re3.cap(3);
      title = re3.cap(4); artist = re3.cap(5);

      // Spotify likes this format
#ifndef QT_NO_DEBUG
      qDebug() << "player '" << player << "', enabled '" << enabled << "', format '" << format << "', title '" << title << "', artist '" << artist << "' ";
#endif
      msnti.player = player;
      msnti.artist = artist;
      msnti.track = title;
    }
  else if (re4.indexIn(s) != -1)
    {
      player = re4.cap(1); enabled = re4.cap(2); artist = re4.cap(3);
      title = re4.cap(4);
#ifndef QT_NO_DEBUG
      qDebug() << "player '" << player << "', enabled '" << enabled << "', artist '" << artist << "', title '" << title << "'";
#endif
      msnti.player = player;
      msnti.artist = artist;
      msnti.track = title;
    }
  else
    {
      msnti.status = PLAYER_STATUS_STOPPED;
    }
  msnti.updatedAt = QDateTime::currentDateTime();


  //
  // Winamp seems to generate messages with enabled=1 but all the fields empty when it stops, so we need to
  // check if any fields have non-empty values as well as looking at that flag
  //
  if ((enabled == "1") &&
      ((msnti.artist != "") || (msnti.track != "") || (msnti.album != "")))
    {
      msnti.status = PLAYER_STATUS_PLAYING;
    }
  else
    {
      msnti.status = PLAYER_STATUS_STOPPED;
    }

  //
  // Some players have artist and title the other way around
  // (As "{0} - {1}","artist","title" and "{1} - {0}","title","artist" are equivalent,
  //  but which field is actually artist and title isn't described by the message)
  //
  // From testing, the order we expect seems to be the popular consensus, but provide
  // a configuration option to override this as I've got this wrong at least once...
  //
  bool swap = false; //purple_prefs_get_bool(PREF_MSN_SWAP_ARTIST_TITLE);

  if (player == "ZUNE")
    swap = !swap;

  if (swap)
  {
    QString temp = msnti.artist;
    msnti.artist = msnti.track;
    msnti.track = temp;
#ifndef QT_NO_DEBUG
    qDebug() << "swapping order to artist '" << msnti.artist << "' and title '" << msnti.track << "'";
#endif
  }

  //
  // Usually we don't find out the actual player name, so report it as unknown...
  //
  if (msnti.player == "")
    {
      msnti.player = "Unknown";
    }

  //free(s);
}

static
LRESULT CALLBACK MSNWinProc(
                            HWND hwnd,      // handle to window
                            UINT uMsg,      // message identifier
                            WPARAM wParam,  // first message parameter
                            LPARAM lParam   // second message parameter
                            )
{
  switch(uMsg) {
  case WM_COPYDATA:
    {
      wchar_t MSNTitle[2048];
      COPYDATASTRUCT *cds = (COPYDATASTRUCT *) lParam;
      CopyMemory(MSNTitle,cds->lpData,cds->cbData);
      MSNTitle[2047]=0;
      process_message(MSNTitle);
      return TRUE;
    }
  default:
    return DefWindowProc(hwnd,uMsg,wParam,lParam);
  }
}

#define 	HWND_MESSAGE   ((HWND)-3)

bool
get_msn_compat_info(struct TrackInfo &ti)
{
    static HWND MSNWindow = 0;

    if (MSNWindow == 0)
    {
         WNDCLASSEX MSNClass = {
            sizeof(WNDCLASSEX),0,
            MSNWinProc,0,0,
            GetModuleHandle(NULL),
            NULL,NULL,NULL,
            NULL,L"MsnMsgrUIManager",NULL
        };

        ATOM a = RegisterClassEx(&MSNClass);
#ifndef QT_NO_DEBUG
        qDebug() << "RegisterClassEx returned 0x" << a ;//<< " :: " << QString::fromWCharArray(wctMsnMsgrUIManager);
#else
        Q_UNUSED(a);
#endif
        MSNWindow = CreateWindowEx(
            0,
            L"MsnMsgrUIManager",L"",
            0,0,0,0,0,
            HWND_MESSAGE,
            NULL,
            GetModuleHandle(NULL),
            NULL);
#ifndef QT_NO_DEBUG
        qDebug() << "CreateWindowEx returned 0x" << MSNWindow;
#endif
        // Alternatively could use setWindowLong() to override WndProc for this window ?
    }

    // did we receive a message with something useful in it?
    if (msnti.status == PLAYER_STATUS_PLAYING)
    {
        if(msnti.updatedAt.msecsTo(QDateTime::currentDateTime())>1500)
            return false; //data hasn't been updated for 1.5 seconds

        //*ti = msnti;
        ti.album = msnti.album;
        ti.artist = msnti.artist;
        ti.player = msnti.player;
        ti.status = msnti.status;
        ti.track = msnti.track;
        ti.currentSecs = 0;
        ti.totalSecs = 0;
        ti.updatedAt = msnti.updatedAt;
        return true;
    }
    else
        return false;
}

/*
static
void cb_toggled(GtkToggleButton *button, gpointer data)
{
  gboolean state = gtk_toggle_button_get_active(button);
  purple_prefs_set_bool(data, state);
}

void get_msn_compat_pref(GtkBox *vbox)
{
  GtkWidget *widget, *hbox;

  hbox = gtk_hbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
  widget = gtk_check_button_new_with_label(_("Swap artist and title"));
  gtk_box_pack_start(GTK_BOX(hbox), widget, TRUE, TRUE, 0);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), purple_prefs_get_bool(PREF_MSN_SWAP_ARTIST_TITLE));
  g_signal_connect(G_OBJECT(widget), "toggled", G_CALLBACK(cb_toggled), (gpointer) PREF_MSN_SWAP_ARTIST_TITLE);
}
*/
// XXX: cleanup needed on musictracker unload?
// XXX: we've also heard tell that HKEY_CURRENT_USER\Software\Microsoft\MediaPlayer\CurrentMetadata has been used to pass this data....
