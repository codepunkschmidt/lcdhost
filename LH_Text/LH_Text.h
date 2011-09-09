/**
  \file     LH_Text.h
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

#ifndef LH_TEXT_H
#define LH_TEXT_H

#include <QtGlobal>
#include <QTextDocument>
#include <QString>
#include <QByteArray>
#include <QColor>
#include <QFont>
#include <QSize>

#include "../LH_QtPlugin.h"
#include "../LH_QtCFInstance.h"
#include "../LH_Qt_bool.h"
#include "../LH_Qt_QString.h"
#include "../LH_Qt_QFont.h"
#include "../LH_Qt_QColor.h"
#include "../LH_Qt_QStringList.h"
#include "../LH_Qt_QSlider.h"

class LH_Text : public LH_QtCFInstance
{
    Q_OBJECT
    QTextDocument doc_;
    QFont font_;
    QImage textimage_;
    QSizeF textsize_;
    int scrollposx_;
    int scrollposy_;
    bool richtext_;

protected:
    LH_Qt_QString *setup_text_;
    LH_Qt_QFont *setup_font_;
    LH_Qt_bool *setup_fontresize_;
    LH_Qt_QColor *setup_pencolor_;
    LH_Qt_QColor *setup_bgcolor_;
    LH_Qt_QStringList *setup_horizontal_;
    LH_Qt_QStringList *setup_vertical_;
    LH_Qt_QSlider *setup_scrollrate_;
    LH_Qt_QSlider *setup_scrollstep_;
    LH_Qt_QSlider *setup_scrollgap_;

    void setRenderHints( QPainter& p );

public:
    typedef enum
    {
        Automatic = 0,
        Percentage,
        Unscaled,
        Kilo,
        Mega,
        Giga,
        Tera
    } NumberMode;

    LH_Text();
    ~LH_Text();

    int polling();
    int notify(int code,void* param);
    void prerender();
    int width( int forHeight = -1 );
    int height( int forWidth = -1 );
    QImage *render_qimage( int w, int h );
    QString text() const { return setup_text_->value(); }

    virtual QColor pencolor() { return setup_pencolor_->value(); }
    virtual QColor bgcolor() { return setup_bgcolor_->value(); }

    QFont font() const { return font_; }
    int horizontal() const { return setup_horizontal_->value(); }
    int vertical() const { return setup_vertical_->value(); }
    bool fontresize() const { return setup_fontresize_->value(); }
    int scrollrate() const { return setup_scrollrate_->value(); }
    int scrollstep() const { return setup_scrollstep_->value(); }
    int scrollgap() const { return setup_scrollgap_->value(); }

    void setPixelHeight( int height );
    bool setText( QString newText ); // return true if text changed
    bool setNum( qreal value, int mode = Automatic, bool suffix = true, qreal max = 0.0, bool bytes = false );
    QTextDocument& doc() { return doc_; }
    bool richtext() const { return richtext_; }
    QImage& textimage() { return textimage_; }
    bool prepareForRender(int w, int h);
    bool monochrome() const { return state() ? state()->dev_depth == 1 : false; }

    static QImage makeImage(int w = 1, int h = 1)
    {
        QImage img(w,h,QImage::Format_ARGB32_Premultiplied);
        img.setDotsPerMeterX(3780);
        img.setDotsPerMeterY(3780);
        return img;
    }

    static QImage makeImage( QSize size )
    {
        return makeImage( size.width(), size.height() );
    }

public slots:
    void fontChanged();
    void textChanged();
    void makeTextImage( int forheight = 0 );
};

#endif // LH_TEXT_H
