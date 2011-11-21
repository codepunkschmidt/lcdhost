/*
  Copyright (C) 2011 Birunthan Mohanathas (www.poiru.net)

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "PlayerVLC.h"
#include "internet.h"
#include <QDebug>
#include <QString>
#include <QUrl>
#include <QStringList>

CPlayer* CPlayerVLC::c_Player = NULL;

// This player retrieves data through the VLC Web interface.

/*
** CPlayerVLC
**
** Constructor.
**
*/
CPlayerVLC::CPlayerVLC(std::wstring port) : CPlayer(),
	m_LastCheckTime(0),
    m_Port(port),
    xml_()
{
}

/*
** ~CPlayerVLC
**
** Destructor.
**
*/
CPlayerVLC::~CPlayerVLC()
{
	c_Player = NULL;
}

/*
** Create
**
** Creates a shared class object.
**
*/
CPlayer* CPlayerVLC::Create(std::wstring port)
{
	if (!c_Player)
	{
        c_Player = new CPlayerVLC(port);
	}

	return c_Player;
}

/*
** UpdateData
**
** Called during each update of the main measure.
**
*/
void CPlayerVLC::UpdateData()
{
    xml_.clear();
    xml_.addData(QString::fromStdWString(CInternet::DownloadUrl(L"http://127.0.0.1:"+m_Port+L"/requests/status.xml", CP_UTF8)));

    QStringList currentPosition;
    while (!xml_.atEnd())
    {
        xml_.readNext();
        if (xml_.isStartElement())
        {
            currentPosition.push_back(xml_.name().toString());
        }
        else if (xml_.isEndElement())
        {
            currentPosition.pop_back();
        }
        else if (xml_.isCharacters() && !xml_.isWhitespace())
        {
            QString nodeAddress = currentPosition.join(".");
            QString nodeValue = xml_.text().toString();
            if(nodeAddress == "root.information.meta-information.artist")
                m_Artist = nodeValue.toStdWString();
            if(nodeAddress == "root.information.meta-information.title")
                m_Title = nodeValue.toStdWString();
            if(nodeAddress == "root.information.meta-information.album")
                m_Album = nodeValue.toStdWString();
            if(nodeAddress == "root.length")
                m_Duration = nodeValue.toInt();
            if(nodeAddress == "root.time")
                m_Position = nodeValue.toInt();
            if(nodeAddress == "root.volume")
                m_Volume = nodeValue.toInt();
            if(nodeAddress == "root.state")
            {
                if(nodeValue == "paused") m_State = PLAYER_PAUSED;
                if(nodeValue == "playing") m_State = PLAYER_PLAYING;
                if(nodeValue == "stop") m_State = PLAYER_STOPPED;
            }
            if(nodeAddress == "root.random")
                m_Shuffle = (nodeValue.toInt()==1);
            if(nodeAddress == "root.repeat")
                m_Repeat = (nodeValue.toInt()==1);
        }
    }
    if (xml_.error() && xml_.error() != QXmlStreamReader::PrematureEndOfDocumentError)
    {
        qWarning() << "XML ERROR:" << xml_.lineNumber() << ": " << xml_.errorString();
    }

    m_CoverPath.clear();
    m_FilePath.clear();
    if((m_Measures & MEASURE_COVER) && m_State != PLAYER_STOPPED)
    {
        xml_.clear();
        xml_.addData(QString::fromStdWString(CInternet::DownloadUrl(L"http://127.0.0.1:"+m_Port+L"/requests/playlist.xml", CP_UTF8)));
        while (!xml_.atEnd())
        {
            xml_.readNext();
            if (xml_.isStartElement())
            {
                QString nodeName = xml_.name().toString();
                if(currentPosition.count()!=0 && xml_.attributes().value("name")!="" && nodeName=="node")
                    nodeName = QString("%1:%2").arg(nodeName).arg(xml_.attributes().value("name").toString());
                if(currentPosition.count()!=0 && xml_.attributes().value("current")!="" && nodeName=="leaf")
                    nodeName = QString("%1:%2").arg(nodeName).arg(xml_.attributes().value("current").toString());
                currentPosition.push_back(nodeName);

                if(currentPosition.join(".") == "node.node:Playlist.leaf:current")
                    m_FilePath = QUrl::fromEncoded(xml_.attributes().value("uri").toString().toUtf8()).toString().remove("file:///").toStdWString();
            }
            else if (xml_.isEndElement())
                currentPosition.pop_back();
            else if (xml_.isCharacters() && !xml_.isWhitespace())
            {
                QString nodeAddress = currentPosition.join(".");
                QString nodeValue = xml_.text().toString();
                if(nodeAddress == "node.node:Playlist.leaf:current.art_url")
                    m_CoverPath = QUrl::fromEncoded(nodeValue.toUtf8()).toString().remove("file:///").toStdWString();
            }
        }
        if (xml_.error() && xml_.error() != QXmlStreamReader::PrematureEndOfDocumentError)
        {
            qWarning() << "XML ERROR:" << xml_.lineNumber() << ": " << xml_.errorString();
        }

    }

    if (m_Measures & MEASURE_LYRICS)
    {
        FindLyrics();
    }

    /*if (m_Initialized || CheckWindow())
	{
		int playing = SendMessage(m_Window, WM_WA_IPC, 0, IPC_ISPLAYING);
		if (playing == 0)
		{
			// Make sure Winamp is still active
			if (!IsWindow(m_Window))
			{
				m_Initialized = false;
				if (m_WinampHandle) CloseHandle(m_WinampHandle);
			}

			if (m_State != PLAYER_STOPPED)
			{
				ClearData();
			}

			// Don't continue if Winamp has quit or is stopped
			return;
		}
		else
		{
			m_State = (playing == 1) ? PLAYER_PLAYING : PLAYER_PAUSED;
			m_Position = SendMessage(m_Window, WM_WA_IPC, 0, IPC_GETOUTPUTTIME) / 1000;		// ms to secs
			m_Volume = (SendMessage(m_Window, WM_WA_IPC, -666, IPC_SETVOLUME) * 100) / 255;	// 0 - 255 to 0 - 100
		}

		WCHAR wBuffer[MAX_PATH];
		char cBuffer[MAX_PATH];

		if (m_UseUnicodeAPI)
		{
			if (!ReadProcessMemory(m_WinampHandle, m_WinampAddress, &wBuffer, sizeof(wBuffer), NULL))
			{
				// Failed to read memory
				return;
			}
		}
		else
		{
			// MediaMonkey doesn't support wide IPC messages
			int pos = SendMessage(m_Window, WM_WA_IPC, 0, IPC_GETLISTPOS);
			LPCVOID address = (LPCVOID)SendMessage(m_Window, WM_WA_IPC, pos, IPC_GETPLAYLISTFILE);

			if (!ReadProcessMemory(m_WinampHandle, address, &cBuffer, sizeof(cBuffer), NULL))
			{
				// Failed to read memory
				return;
			}

			mbstowcs(wBuffer, cBuffer, MAX_PATH);
		}

		if (wcscmp(wBuffer, m_FilePath.c_str()) != 0)
		{
			++m_TrackCount;
			m_FilePath = wBuffer;
			m_PlayingStream = (m_FilePath.find(L"://") != std::wstring::npos);

			if (!m_PlayingStream)
			{
				m_Rating = SendMessage(m_Window, WM_WA_IPC, 0, IPC_GETRATING);
				m_Duration = SendMessage(m_Window, WM_WA_IPC, 1, IPC_GETOUTPUTTIME);
				m_Shuffle = (bool)SendMessage(m_Window, WM_WA_IPC, 0, IPC_GET_SHUFFLE);
				m_Repeat = (bool)SendMessage(m_Window, WM_WA_IPC, 0, IPC_GET_REPEAT);

				TagLib::FileRef fr(wBuffer, false);
				TagLib::Tag* tag = fr.tag();
				if (tag)
				{
					m_Artist = tag->artist().toWString();
					m_Album = tag->album().toWString();
					m_Title = tag->title().toWString();

					if (m_Measures & MEASURE_LYRICS)
					{
						FindLyrics();
					}
				}
				else if (m_Measures & MEASURE_LYRICS)
				{
					m_Lyrics.clear();
				}

				// Find cover if needed
				if (m_Measures & MEASURE_COVER)
				{
					if (tag && CCover::GetEmbedded(fr, m_TempCoverPath))
					{
						// Got everything, return
						m_CoverPath = m_TempCoverPath;
						return;
					}

					std::wstring trackFolder = CCover::GetFileFolder(m_FilePath);
					if (tag && !m_Album.empty())
					{
						// Winamp stores covers usually as %album%.jpg
						std::wstring file = m_Album;
						std::wstring::size_type end = file.length();
						for (std::wstring::size_type pos = 0; pos < end; ++pos)
						{
							// Replace reserved chars according to Winamp specs
							switch (file[pos])
							{
							case L'?':
							case L'*':
							case L'|':
								file[pos] = L'_';
								break;

							case L'/':
							case L'\\':
							case L':':
								file[pos] = L'-';
								break;

							case L'\"':
								file[pos] = L'\'';
								break;

							case L'<':
								file[pos] = L'(';
								break;

							case L'>':
								file[pos] = L')';
								break;
							}
						}

						if (CCover::GetLocal(file, trackFolder, m_CoverPath))
						{
							// %album% art file found
							return;
						}
					}

					if (!CCover::GetLocal(L"cover", trackFolder, m_CoverPath) &&
						!CCover::GetLocal(L"folder", trackFolder, m_CoverPath))
					{
						// Nothing found
						m_CoverPath.clear();
					}
				}

				if (tag)
				{
					return;
				}
			}
			else
			{
				m_Rating = 0;
				m_Duration = 0;
				m_CoverPath.clear();
			}
		}
		else if (!m_PlayingStream)
		{
			return;
		}

		// TagLib couldn't parse the file or Winamp is playing a stream, try to get title
		if (m_UseUnicodeAPI)
		{
			LPCVOID address = (LPCVOID)SendMessage(m_Window, WM_WA_IPC, 0, IPC_GET_PLAYING_TITLE);
			ReadProcessMemory(m_WinampHandle, address, &wBuffer, sizeof(wBuffer), NULL);
		}
		else
		{
			int pos = SendMessage(m_Window, WM_WA_IPC, 0, IPC_GETLISTPOS);
			LPCVOID address = (LPCVOID)SendMessage(m_Window, WM_WA_IPC, pos, IPC_GETPLAYLISTTITLE);
			ReadProcessMemory(m_WinampHandle, address, &cBuffer, sizeof(cBuffer), NULL);
			mbstowcs(wBuffer, cBuffer, MAX_PATH);
		}

		std::wstring title = wBuffer;
		std::wstring::size_type pos = title.find(L" - ");
		if (pos != std::wstring::npos)
		{
			m_Artist.assign(title, 0, pos);
			pos += 3;  // Skip " - "
			m_Title.assign(title, pos, title.length() - pos);
			m_Album.clear();

			if (m_PlayingStream)
			{
				// Remove crap from title if playing radio
				pos = m_Title.find(L" (");
				if (pos != std::wstring::npos)
				{
					m_Title.resize(pos);
				}
			}
		}
		else
		{
			m_Title = title;
			m_Artist.clear();
			m_Album.clear();
		}
	}
    */
}

#ifdef DISABLED_CONTROLS
/*
** Pause
**
** Handles the Pause bang.
**
*/
void CPlayerVLC::Pause()
{
    //SendMessage(m_Window, WM_COMMAND, WINAMP_PAUSE, 0);
}

/*
** Play
**
** Handles the Play bang.
**
*/
void CPlayerVLC::Play()
{
    //SendMessage(m_Window, WM_COMMAND, WINAMP_PLAY, 0);
}

/*
** Stop
**
** Handles the Stop bang.
**
*/
void CPlayerVLC::Stop()
{
    //SendMessage(m_Window, WM_COMMAND, WINAMP_STOP, 0);
}

/*
** Next
**
** Handles the Next bang.
**
*/
void CPlayerVLC::Next()
{
    //SendMessage(m_Window, WM_COMMAND, WINAMP_FASTFWD, 0);
}

/*
** Previous
**
** Handles the Previous bang.
**
*/
void CPlayerVLC::Previous()
{
    //SendMessage(m_Window, WM_COMMAND, WINAMP_REWIND, 0);
}

/*
** SetPosition
**
** Handles the SetPosition bang.
**
*/
void CPlayerVLC::SetPosition(int position)
{
    //position *= 1000; // To milliseconds
    //SendMessage(m_Window, WM_WA_IPC, position, IPC_JUMPTOTIME);
}

/*
** SetRating
**
** Handles the SetRating bang.
**
*/
void CPlayerVLC::SetRating(int rating)
{
    /*if (rating < 0)
	{
		rating = 0;
	}
	else if (rating > 5)
	{
		rating = 5;
	}

	SendMessage(m_Window, WM_WA_IPC, rating, IPC_SETRATING);
    m_Rating = rating;*/
}

/*
** SetVolume
**
** Handles the SetVolume bang.
**
*/
void CPlayerVLC::SetVolume(int volume)
{
    /*if (volume > 0 && volume < 100) ++volume;	// For proper scaling

	// Winamp accepts volume in 0 - 255 range
	volume *= 255;
	volume /= 100;
    SendMessage(m_Window, WM_WA_IPC, volume, IPC_SETVOLUME);*/
}

/*
** SetShuffle
**
** Handles the SetShuffle bang.
**
*/
void CPlayerVLC::SetShuffle(bool state)
{
    /*if (!m_PlayingStream)
	{
		m_Shuffle = state;
		SendMessage(m_Window, WM_WA_IPC, (WPARAM)m_Shuffle, IPC_SET_SHUFFLE);
	}
    */
}

/*
** SetRepeat
**
** Handles the SetRepeat bang.
**
*/
void CPlayerVLC::SetRepeat(bool state)
{
    /*if (!m_PlayingStream)
	{
		m_Repeat = state;
		SendMessage(m_Window, WM_WA_IPC, (WPARAM)m_Repeat, IPC_SET_REPEAT);
	}
    */
}

/*
** ClosePlayer
**
** Handles the ClosePlayer bang.
**
*/
void CPlayerVLC::ClosePlayer()
{
    /*if (m_WinampType == WA_WINAMP)
	{
		SendMessage(m_Window, WM_CLOSE, 0, 0);
	}
	else // if (m_WinampType == WA_MEDIAMONKEY)
	{
		HWND wnd = FindWindow(L"TFMainWindow", L"MediaMonkey");
		if (wnd)
		{
			SendMessage(wnd, WM_CLOSE, 0, 0);
		}
	}
    */
}

/*
** OpenPlayer
**
** Handles the OpenPlayer bang.
**
*/
void CPlayerVLC::OpenPlayer(std::wstring& path)
{
    /*if (m_WinampType == WA_WINAMP)
	{
		ShellExecute(NULL, L"open", path.empty() ? L"winamp.exe" : path.c_str(), NULL, NULL, SW_SHOW);
	}
	else // if (m_WinampType == WA_MEDIAMONKEY)
	{
		if (path.empty())
		{
			// Gotta figure out where Winamp is located at
			HKEY hKey;
			RegOpenKeyEx(HKEY_LOCAL_MACHINE,
						 L"SOFTWARE\\Clients\\Media\\MediaMonkey\\shell\\open\\command",
						 0,
						 KEY_QUERY_VALUE,
						 &hKey);

			DWORD size = 512;
			WCHAR* data = new WCHAR[size];
			DWORD type = 0;

			if (RegQueryValueEx(hKey,
								NULL,
								NULL,
								(LPDWORD)&type,
								(LPBYTE)data,
								(LPDWORD)&size) == ERROR_SUCCESS)
			{
				if (type == REG_SZ)
				{
					ShellExecute(NULL, L"open", data, NULL, NULL, SW_SHOW);
					path = data;
				}
			}

			delete [] data;
			RegCloseKey(hKey);
		}
		else
		{
			ShellExecute(NULL, L"open", path.c_str(), NULL, NULL, SW_SHOW);
		}
	}
*/
}
#endif
