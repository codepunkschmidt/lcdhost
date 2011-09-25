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

#ifndef LH_QTDEVICE_H
#define LH_QTDEVICE_H

#include <QImage>

#include "LH_QtPlugin.h"
#include "LH_QtObject.h"

/**
  Suggested base class for LCD devices reported by your driver. You
  don't need to inherit from this, but it does provide a good starting
  point.
  */
class LH_QtDevice : public LH_QtObject
{
    Q_OBJECT
    QByteArray devid_;
    QSize size_;
    int depth_;
    bool noauto_;

public:
    LH_QtDevice( LH_QtObject *parent );
    virtual ~LH_QtDevice();

    /**
      These methods are the ones you need to override in your
      descendant classes. And the constructor is usually a good
      idea to override as well.
      */
    virtual const char* open() { return NULL; }
    virtual const char* render_qimage(QImage*) { return NULL; }
    virtual const char* render_argb32(int,int,const void*) { return NULL; }
    virtual const char* render_mono(int,int,const void*) { return NULL; }
    virtual const char* close() { return NULL; }

    void arrive();
    void leave();

    QByteArray devid() const { return devid_; }
    void setDevid( QByteArray );
    QString name() const { return objectName(); }
    void setName( QString );

    QSize size() const { return size_; }
    void setSize( QSize s ) { size_ = s; }
    void setSize( int w, int h ) { size_.setWidth(w); size_.setHeight(h); }
    int depth() const { return depth_; }
    void setDepth( int n ) { depth_ = n; }
    void setAutoselect(bool b) { noauto_ = !b; }
    bool autoselect() const { return noauto_ == 0; } /* available for autoselection by LCDHost */
    bool monochrome() const { return depth_ == 1; }
};

#endif // LH_QTDEVICE_H