/**
  \file     LH_Background.cpp
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

#include <QDebug>
#include <QPainter>

#include "LH_Background.h"

LH_PLUGIN_CLASS(LH_Background)

lh_class *LH_Background::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "Static",
        "StaticBackground",
        "Background",
        48,48,
        lh_object_calltable_NULL,
        lh_instance_calltable_NULL
    };

    return &classInfo;
}

LH_Background::LH_Background()
{
    setup_startcolor_ = new LH_Qt_QColor(this,tr("First color"),Qt::white,LH_FLAG_AUTORENDER);
    setup_gradient_ = new LH_Qt_bool(this,tr("^Background is a gradient"),false);
    connect( setup_gradient_, SIGNAL(change(bool)), this, SLOT(enableGradient(bool)) );
    setup_stopcolor_ = new LH_Qt_QColor(this,tr("Second color"),Qt::lightGray,LH_FLAG_HIDDEN|LH_FLAG_AUTORENDER);
    setup_horizontal_ = new LH_Qt_bool(this,tr("^Gradient is horizontal"),false,LH_FLAG_HIDDEN|LH_FLAG_AUTORENDER);
}

void LH_Background::enableGradient(bool b)
{
    setup_stopcolor_->setFlag( LH_FLAG_HIDDEN, !b );
    setup_horizontal_->setFlag( LH_FLAG_HIDDEN, !b );
    requestRender();
}

QImage *LH_Background::render_qimage( int w, int h )
{
    if(QImage *img = initImage(w,h))
    {
        img->fill( qRgba(0,0,0,0) );

        QPainter painter;
        if( painter.begin(img) )
        {
            painter.setPen(Qt::NoPen);
            if( setup_gradient_->value() )
            {
                QLinearGradient gradient;
                gradient.setStart(0,0);
                if( setup_horizontal_->value() ) gradient.setFinalStop(img->width(),0);
                else gradient.setFinalStop(0,img->height());
                gradient.setColorAt(0, setup_startcolor_->value() );
                gradient.setColorAt(1, setup_stopcolor_->value() );
                QBrush brush(gradient);
                painter.setBrush( brush );
            }
            else
                painter.setBrush( setup_startcolor_->value() );
            painter.drawRect( img->rect() );
            painter.end();
        }
        return img;
    }
    return 0;
}
