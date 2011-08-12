/**
  \file     LH_QtDevice.h
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

#ifndef LH_QTOUTPUTDEVICE_H
#define LH_QTOUTPUTDEVICE_H

#include <QImage>

#include "LH_QtObject.h"

/**
  Suggested base class for output devices reported by your driver. You
  don't need to inherit from this, but it does provide a good starting
  point.
  */
class LH_QtOutputDevice : public LH_QtObject
{
    Q_OBJECT
    lh_output_device lh_dev_;

public:
    LH_QtOutputDevice( const char *ident, int w, int h, int d, bool noauto );
    virtual ~LH_QtOutputDevice();

    LH_QtObject *parent() const { return static_cast<LH_QtObject *>(LH_QtObject::parent()); }

    lh_output_device *lh_dev() { return &lh_dev_; }

    /**
      These methods are the ones you need to override in your
      descendant classes. And the constructor is usually a good
      idea to override as well.
      */
    virtual const char* open() { return NULL; }
    virtual const char* render_qimage(QImage*) { return NULL; }
    virtual const char* render_argb32(int,int,const void*) { return NULL; }
    virtual const char* render_mono(int,int,const void*) { return NULL; }
    virtual const char* get_backlight(lh_device_backlight*) { return NULL; }
    virtual const char* set_backlight(lh_device_backlight*) { return NULL; }
    virtual const char* close() { return NULL; }

    QSize size() const { return QSize( lh_dev_.width, lh_dev_.height); }
    int depth() const { return lh_dev_.depth; }
    bool autoselect() const { return lh_dev_.noauto == 0; } /* available for autoselection by LCDHost */
    bool monochrome() const { return lh_dev_.depth == 1; }
};

#endif // LH_QTOUTPUTDEVICE_H
