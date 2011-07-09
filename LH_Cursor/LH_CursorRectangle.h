/**
  \file     LH_CursorRectangle.h
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2010,2011 Andy Bridges & Johan Lindh
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.

 **/

#ifndef LH_CURSORRECTANGLE_H
#define LH_CURSORRECTANGLE_H

#include "../LH_QtInstance.h"
#include "../LH_Qt_bool.h"
#include "../LH_Qt_QSlider.h"
#include "../LH_Qt_QColor.h"
#include "../LH_Qt_QString.h"
#include "../LH_Qt_QFileInfo.h"

#include "LH_CursorData.h"

#include <QHash>

struct colorMapData
{
    QString code;
    QColor color1;
    bool isGradient;
    QColor color2;
    bool isHorizontal;
};


class LH_CursorRectangle : public LH_QtInstance
{
    Q_OBJECT

    bool updateState();
    void applyOverrides();
    void applyOverrides(QString testState, LH_Qt_bool *testOverride, LH_Qt_QColor *testCol1, LH_Qt_QColor *testCol2);

protected:
    QString statusCode_;
    QHash<QString, colorMapData> colorDefinitions;

    LH_Qt_QSlider *setup_penwidth_;
    LH_Qt_QSlider *setup_rounding_;
    LH_Qt_QColor *setup_pencolor_;

    LH_Qt_QFileInfo *setup_file_;


    LH_Qt_bool *setup_color_overrides_;

    LH_Qt_bool *setup_override_off_;
    LH_Qt_QColor *setup_bgcolor1_off_;
    LH_Qt_QColor *setup_bgcolor2_off_;

    LH_Qt_bool *setup_override_on_;
    LH_Qt_QColor *setup_bgcolor1_on_;
    LH_Qt_QColor *setup_bgcolor2_on_;

    LH_Qt_bool *setup_override_off_sel_;
    LH_Qt_QColor *setup_bgcolor1_off_sel_;
    LH_Qt_QColor *setup_bgcolor2_off_sel_;

    LH_Qt_bool *setup_override_on_sel_;
    LH_Qt_QColor *setup_bgcolor1_on_sel_;
    LH_Qt_QColor *setup_bgcolor2_on_sel_;

    //LH_Qt_bool *setup_gradient_;
    //LH_Qt_bool *setup_horizontal_;

    LH_Qt_QString *setup_coordinate_;
    LH_Qt_bool *setup_layout_trigger_;
    LH_Qt_QFileInfo *setup_layout_;
    LH_Qt_QString *setup_text_;

public:
    const char *userInit();
    QImage *render_qimage( int w, int h );
    QColor pencolor() const { return setup_pencolor_->value(); }
    int penwidth() const { return setup_penwidth_->value(); }
    int rounding() const { return setup_rounding_->value(); }
    bool gradient();
    bool horizontal();
    QColor bgcolor1();
    QColor bgcolor2();
    int polling() ;

    static lh_class *classInfo();


public slots:
    void changeLayoutTrigger();
    void changeColorOverrides();

    void fileChanged();
    void overridesChanged();
};

#endif // LH_CURSORRECTANGLE_H
