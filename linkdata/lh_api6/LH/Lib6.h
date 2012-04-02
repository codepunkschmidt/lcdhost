/**
  \file     LH/Lib6.h
  \author   Johan Lindh <johan@linkdata.se>
  \brief    LCDHost plugin API version 6
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

#ifndef LH_LIB6_H
#define LH_LIB6_H

#ifndef LH_API_MAJOR
# define LH_API_MAJOR 6
# define LH_API_MINOR 0
#endif

#ifndef STRINGIZE
# define STRINGIZE_(x) #x
# define STRINGIZE(x) STRINGIZE_(x)
#endif

#ifdef __cplusplus

template <class T> class QExplicitlySharedDataPointer;

namespace LH {
namespace Lib6 {
class IdData;
typedef QExplicitlySharedDataPointer<IdData> Id;
class Loader;
class Input;
class Variant;
class Object;
class Service;
class Plugin;
class Linkable;
class Source;
class Sink;
class Setup;
class LayoutClass;
class LayoutItem;
} // namespace Lib6
} // namespace LH

typedef LH::Lib6::Loader LibLoader;
typedef LH::Lib6::Input LibInput;
typedef LH::Lib6::IdData LibIdData;
typedef LH::Lib6::Id LibId;
typedef LH::Lib6::Object LibObject;
typedef LH::Lib6::Service LibService;
typedef LH::Lib6::Plugin LibPlugin;
typedef LH::Lib6::Linkable LibLinkable;
typedef LH::Lib6::Source LibSource;
typedef LH::Lib6::Sink LibSink;
typedef LH::Lib6::Setup LibSetup;
#define lh_ui_type LH::Lib6::Setup::ui_type
#define lh_ui_flag LH::Lib6::Setup::ui_flag
typedef LH::Lib6::LayoutClass LibLayoutClass;
typedef LH::Lib6::LayoutItem LibLayoutItem;

#endif // __cplusplus

#endif // LH_LIB6_H
