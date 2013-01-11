/**
  \file     LH_QtPlugin_LCoreReboot.h
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese
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

#ifndef LH_QTPLUGIN_LCOREREBOOT_H
#define LH_QTPLUGIN_LCOREREBOOT_H

#include "LH_QtPlugin.h"
#include "LH_Qt_QString.h"

#ifdef Q_OS_WIN32
#include <windows.h>
#include <winbase.h>
#include <Psapi.h>
#endif

class LH_QtPlugin_LCoreReboot : public LH_QtPlugin
{
    Q_OBJECT

    bool enableDebugPrivileges();
    //bool SetPrivilege( HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege );
    //void SetProcessPrivilege( );

    QString getLastErrorMessage();
    bool getLCorePath(QString&);
    bool killLCore();
    void launchLCore();

protected:
    LH_Qt_QString *setup_reboot_;
    LH_Qt_QString *setup_lcore_path_;

public:
    const char *userInit();

public slots:
    void rebootLCore();

};

#endif // LH_QTPLUGIN_LCOREREBOOT_H
