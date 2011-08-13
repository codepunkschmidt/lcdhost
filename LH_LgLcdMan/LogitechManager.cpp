/**
  \file     LogitechManager.cpp
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009-2011, Johan Lindh

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
  */

#include <QCoreApplication>
#include "LogitechManager.h"

LogitechManager::LogitechManager(QObject *parent) : QObject(parent)
{
    int appname_len = QCoreApplication::applicationName().length();
#ifdef Q_WS_WIN
    appname_ = new wchar_t[appname_len+1];
    QCoreApplication::applicationName().toWCharArray(appname_);
    appname_[appname_len] = 0;
#endif
#ifdef Q_WS_MAC
    appname_ = CFStringCreateWithCharacters(
                0, reinterpret_cast<const UniChar *>(QCoreApplication::applicationName().unicode()),
                appname_len );
#endif
}

LogitechManager::~LogitechManager()
{
    if( appname_ )
    {
    #ifdef Q_WS_WIN
        delete[] (wchar_t *)appname_;
    #endif
    #ifdef Q_WS_MAC
        CFRelease( (CFStringRef) appname_ );
    #endif
    }
}

const char* LogitechManager::lglcd_Err( int result, const char *filename, unsigned line )
{
    static char uk_err_buf[64];

    if( result == ERROR_SUCCESS ) return NULL;

    switch( result )
    {
#ifdef ERROR_FAILED
    case ERROR_FAILED:                  return "Failed.";
#endif

#ifdef Q_WS_WIN
    case RPC_S_SERVER_UNAVAILABLE:	return "Logitech LCD subsystem is not available.";
    case ERROR_OLD_WIN_VERSION:		return "Attempted to initialize for Windows 9x.";
    case ERROR_NO_SYSTEM_RESOURCES:	return "Not enough system resources.";
    case ERROR_ALREADY_INITIALIZED:	return "lgLcdInit() has been called before.";
    case ERROR_SERVICE_NOT_ACTIVE:	return "lgLcdInit() has not been called yet.";
    case ERROR_FILE_NOT_FOUND:		return "LCDMon is not running on the system.";
#endif

    case ERROR_ACCESS_DENIED:           return "Access denied. LCDHost is likely disabled in the Logitech LCD Manager.";
    case ERROR_CALL_NOT_IMPLEMENTED:    return "Call not implemented.";
    case ERROR_LOCK_FAILED:             return "Lock failed.";
    case ERROR_DEVICE_NOT_CONNECTED:    return "Device not connected.";
    case ERROR_INTERNAL_ERROR:          return "Logitech driver internal error.";
    case ERROR_TIMEOUT:                 return "Timeout.";
    case RPC_S_PROTOCOL_ERROR:          return "Protocol error.";
    case ERROR_NO_MORE_ITEMS:		return "There are no more devices to be enumerated.";
    case ERROR_ALREADY_EXISTS:		return "Already exists: Can't connect or open twice.";
    case ERROR_INVALID_PARAMETER:	return "Invalid parameter.";
    case RPC_X_WRONG_PIPE_VERSION:	return "LCDMon does not understand the protocol.";
    }

#ifdef Q_WS_WIN
    LPWSTR lpMsgBuf;
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                   NULL, result, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   (LPWSTR) &lpMsgBuf, 0, NULL);
    qWarning( "LH_LgLcdMan: Unknown LCD error '%ls' (%x)\n", lpMsgBuf, result );
    LocalFree(lpMsgBuf);
#else
    qWarning( "LH_LgLcdMan: %s(%d) Unknown LCD error %x\n", filename, line, result );
#endif

    qsnprintf(uk_err_buf, sizeof(uk_err_buf), "Error %d", result);
    return uk_err_buf;
}


