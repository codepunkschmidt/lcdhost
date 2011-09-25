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
#include <QtPlugin>
#include <QObject>

#include "LH_QtObject.h"

#ifndef EXPORT
# define EXPORT extern "C" Q_DECL_EXPORT
#endif


/**
  Base class for Qt-based LCDHost shared libraries.
  As of alpha 17, the LH_QtPlugin object is no longer a global
  C++ object, rather it is created dynamically in lh_create()
  and destroyed in lh_destroy(). These two functions are
  defined when you use the LH_PLUGIN(classname) macro.
  */
class LH_QtPlugin : public LH_QtObject
{
    Q_OBJECT
public:
};

#define LH_SIGNATURE()

/**
  This macro creates the required exported functions
  for your LH_QtPlugin descendant.
  */
#define LH_PLUGIN(classname) \
    Q_EXPORT_PLUGIN2(classname,classname) \
    lh_signature lh_##classname##_signature = LH_SIGNATURE_BLANK; \
    char lh_##classname##_xml[]

#endif // LH_QTPLUGIN_H
