/**
  \file     LH_QtPlugin.h
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009-2011 Johan Lindh

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

#ifndef LH_QTPLUGIN_H
#define LH_QTPLUGIN_H

#include <QtGlobal>
#include <QObject>

#include "LH_QtObject.h"

#ifndef EXPORT
# define EXPORT extern "C" Q_DECL_EXPORT
#endif

/**
  Base class for Qt-based LCDHost shared libraries.
  */
class LH_QtPlugin : public LH_QtObject
{
    Q_OBJECT

    static LH_QtPlugin *instance_;
    lh_object obj_;

public:
    LH_QtPlugin();
    LH_QtPlugin( QObject *parent );
    ~LH_QtPlugin();

    lh_object& pluginObject() { return obj_; }
    virtual const char *userInit();
    static LH_QtPlugin *instance() { return instance_; }

public slots:
    void requestReload( const char *msg = 0 );
};

/**
  This macro creates the required things for your LH_QtPlugin
  descendant to be recognized as a LCDHost plugin. Sample usage:

    LH_PLUGIN(MyPlugin) =
      "<?xml version=\"1.0\"?>"
      "<lcdhostplugin>"
        "<id>MyPlugin</id>"
        "<rev>" STRINGIZE(REVISION) "</rev>"
        "<api>" STRINGIZE(LH_API_MAJOR) "." STRINGIZE(LH_API_MINOR) "</api>"
        "<ver>" "r" STRINGIZE(REVISION) "</ver>"
        "<versionurl>http://where.version.info.is.kept/</versionurl>"
        "<author>My Name</author>"
        "<homepageurl>My Homepage</homepageurl>"
        "<logourl></logourl>"
        "<shortdesc>"
        "ONE_LINE_DESCRIPTION"
        "</shortdesc>"
        "<longdesc>"
        "MULTI_LINE_DESCRIPTION"
        "</longdesc>"
      "</lcdhostplugin>";
  */
#define LH_PLUGIN(classname) \
    EXPORT lh_object *lh_create() { return &(new classname())->pluginObject(); } \
    EXPORT void lh_destroy( lh_object *obj ) { delete reinterpret_cast<classname*>(obj->ref); } \
    lh_signature lh_##classname##_signature = LH_SIGNATURE_BLANK; \
    char lh_##classname##_xml[]

#endif // LH_QTPLUGIN_H
