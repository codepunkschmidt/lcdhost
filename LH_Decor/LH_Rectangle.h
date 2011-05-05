/**
  \file     LH_Rectangle.h
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009-2010 Johan Lindh

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

#ifndef LH_RECTANGLE_H
#define LH_RECTANGLE_H

#include "../LH_QtPlugin.h"
#include "../LH_QtInstance.h"
#include "../LH_Qt_bool.h"
#include "../LH_Qt_QSlider.h"
#include "../LH_Qt_QColor.h"

class LH_Rectangle : public LH_QtInstance
{
    Q_OBJECT

protected:
    LH_Qt_QSlider *setup_penwidth_;
    LH_Qt_QSlider *setup_rounding_;
    LH_Qt_QColor *setup_pencolor_;
    LH_Qt_QColor *setup_bgcolor1_;
    LH_Qt_QColor *setup_bgcolor2_;
    LH_Qt_bool *setup_gradient_;
    LH_Qt_bool *setup_horizontal_;

public:
    LH_Rectangle( const char *name );

    QImage *render_qimage( int w, int h );

    int penwidth() const { return setup_penwidth_->value(); }
    int rounding() const { return setup_rounding_->value(); }
    bool gradient() const { return setup_gradient_->value(); }
    bool horizontal() const { return setup_horizontal_->value(); }
    QColor pencolor() const { return setup_pencolor_->value(); }
    QColor bgcolor1() const { return setup_bgcolor1_->value(); }
    QColor bgcolor2() const { return setup_bgcolor2_->value(); }

    static lh_class *classInfo();

public slots:
    void enableGradient(bool);
};

#endif // LH_RECTANGLE_H
