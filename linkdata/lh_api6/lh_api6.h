/**
  \file     lh_api6.h
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

#ifndef LH_API6_H
#define LH_API6_H

#ifndef LH_API_MAJOR
# define LH_API_MAJOR 6
# define LH_API_MINOR 0
#endif

#ifndef STRINGIZE
# define STRINGIZE_(x) #x
# define STRINGIZE(x) STRINGIZE_(x)
#endif

#ifdef __cplusplus

#include "lh_loader.h"
#include "lh_id.h"
#include "lh_input.h"
#include "lh_qvariant.h"
#include "lh_object.h"
#include "lh_service.h"
#include "lh_plugin.h"
#include "lh_linkable.h"
#include "lh_source.h"
#include "lh_sink.h"
#include "lh_setup.h"
#include "lh_layoutclass.h"
#include "lh_layoutitem.h"

typedef lh::api6::loader lh_loader;
typedef lh::api6::id_ptr lh_id;
typedef lh::api6::input lh_input;
typedef lh::api6::object lh_object;
typedef lh::api6::service lh_service;
typedef lh::api6::plugin lh_plugin;
typedef lh::api6::linkable lh_linkable;
typedef lh::api6::source lh_source;
typedef lh::api6::sink lh_sink;
typedef lh::api6::setup lh_setup;
typedef lh::api6::setup::ui_type lh_ui_type;
typedef lh::api6::setup::ui_flag lh_ui_flag;
typedef lh::api6::layoutclass lh_layoutclass;
typedef lh::api6::layoutitem lh_layoutitem;

namespace lh_event = lh::api6::event;

#endif // __cplusplus

#endif // LH_API6_H
