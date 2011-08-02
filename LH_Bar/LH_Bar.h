/**
  \file     LH_Bar.h
  \author   Johan Lindh <johan@linkdata.se>
  \author   Andy Bridges <andy@bridgesuk.com>
  \legalese Copyright (c) 2010 Johan Lindh, Andy Bridges

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

#ifndef LH_BAR_H
#define LH_BAR_H

#include "../LH_QtPlugin.h"
#include "../CF/LH_QtCFInstance.h"
#include "../LH_Qt_QColor.h"
#include "../LH_Qt_QStringList.h"
#include "../LH_Qt_QSlider.h"
#include "../LH_Qt_QFileInfo.h"
#include "../LH_Qt_bool.h"
#include "../LH_Qt_array_double.h"
#include "../LH_Qt_double.h"
#include "../LH_Qt_int.h"

class LH_Bar : public LH_QtCFInstance
{
    Q_OBJECT

    qreal min_;
    qreal max_;
    QImage bar_img_;
    QImage bar_img_bg_;
    QImage bar_img_endMask_;
    QImage bar_img_emptyMask_;

    void draw_bar( double value, int pos = 0, int total = 1 );
    double boundedValue(double value);

protected:
    LH_Qt_QStringList *setup_type_;
    LH_Qt_QFileInfo *setup_file_;
    LH_Qt_QFileInfo *setup_file_bg_;
    LH_Qt_QStringList *setup_masking_;
    LH_Qt_QFileInfo *setup_file_endMask_;
    LH_Qt_QColor *setup_pencolor1_;
    LH_Qt_QColor *setup_pencolor2_;
    LH_Qt_QColor *setup_bgcolor_;
    LH_Qt_QStringList *setup_direction_;
    LH_Qt_QSlider *setup_spacing_;
    LH_Qt_bool *setup_discrete_;
    LH_Qt_int *setup_discrete_count_;


public:
    const char *userInit();
    QImage *render_qimage( int w, int h );

    qreal min() const { return min_; }
    qreal max() const { return max_; }
    bool setMin( qreal r ); // return true if rendering needed
    bool setMax( qreal r ); // return true if rendering needed


    void drawSingle( double value )
    {
        cf_source_notify("Value", QString::number(boundedValue(value)));
        draw_bar(value);
    }

    void drawList( const double *values, int total )
    {
        for( int i=0; i<total; ++i )
            cf_source_notify("Value", QString::number(boundedValue(values[i])), i, total);
        for( int i=0; i<total; ++i )
            draw_bar( values[i], i, total );
    }

    void drawList( const QVector<double> &values )
    {
        drawList( values.constData(), values.size() );
    }

public slots:
    void changeType();
    void changeDiscrete();
    void changeFile();

};

#endif // LH_BAR_H
