/**
  \file     LH_MonitoringText.cpp
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese
    This module is based on original work by Johan Lindh and uses code freely
    available from http://allthingsgeek.wordpress.com/ by "Noccy"

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

  */

#include <QDebug>
#include <QFont>
#include <QFontMetrics>
#include <QTime>
#include <QRegExp>

#include "LH_MonitoringText.h"

LH_MonitoringText::LH_MonitoringText(const char *name) : LH_Text(name)
{
    setup_value_round_ = new LH_Qt_bool(this,"Round",false, LH_FLAG_AUTORENDER);
    setup_value_round_->setHelp( "<p>Round non integer values.</p>");
    setup_value_round_->setOrder(-3);
    connect( setup_value_round_, SIGNAL(changed()), this, SLOT(updateText()) );

    setup_append_units_ = new LH_Qt_bool(this, "Append Units", true, 0);
    setup_append_units_->setHelp( "<p>Append the units to the text.</p>");
    setup_append_units_->setOrder(-3);
    connect( setup_append_units_, SIGNAL(changed()), this, SLOT(updateText()) );

    setup_pre_text_ = new LH_Qt_QString( this, "Pre-Text", "", LH_FLAG_AUTORENDER);
    setup_pre_text_->setHelp( "<p>Text to be displayed before the sensor value.</p>");
    setup_pre_text_->setOrder(-3);
    connect( setup_pre_text_, SIGNAL(changed()), this, SLOT(updateText()) );

    setup_post_text_ = new LH_Qt_QString(this, "Post-Text", "", LH_FLAG_AUTORENDER);
    setup_post_text_->setHelp( "<p>Text to be displayed after the sensor value.</p>");
    setup_post_text_->setOrder(-3);
    connect( setup_post_text_, SIGNAL(changed()), this, SLOT(updateText()) );

    setup_text_->setFlag( LH_FLAG_HIDDEN, true );

    setup_use_thresh_pen_colors_ = new LH_Qt_bool(this,"Status Colors (Text)",false, LH_FLAG_AUTORENDER);
    setup_use_thresh_pen_colors_->setHelp( "<p>Vary the text colour based on the sensor value.</p>");
    setup_use_thresh_pen_colors_->setOrder(-1);
    setup_use_thresh_bg_colors_ = new LH_Qt_bool(this,"Status Colors (BG)",false, LH_FLAG_AUTORENDER);
    setup_use_thresh_bg_colors_->setHelp( "<p>Vary the backgorund colour based on the sensor value.</p>");
    setup_use_thresh_bg_colors_->setOrder(-1);

    createThresholdSet(&setup_thresh_vlo_color_pen_,&setup_thresh_vlo_color_bg_,
                       "Very Low", QColor(128,128,255), QColor(192,192,255), true,
                       &setup_thresh_vlo_float_,&setup_thresh_vlo_int_);

    createThresholdSet(&setup_thresh_lo_color_pen_,&setup_thresh_lo_color_bg_,
                       "Low", Qt::blue, QColor(128,128,255), true,
                       &setup_thresh_lo_float_,&setup_thresh_lo_int_);

    createThresholdSet(&setup_thresh_ok_color_pen_,&setup_thresh_ok_color_bg_,
                       "Ok", Qt::blue, QColor(128,128,255), true);

    createThresholdSet(&setup_thresh_hi_color_pen_,&setup_thresh_hi_color_bg_,
                       "High", QColor(255,128,0), QColor(255,192,128), false,
                       &setup_thresh_hi_float_,&setup_thresh_hi_int_);

    createThresholdSet(&setup_thresh_vhi_color_pen_,&setup_thresh_vhi_color_bg_,
                       "Very High", Qt::red, QColor(255,128,128), false,
                       &setup_thresh_vhi_float_,&setup_thresh_vhi_int_);

    connect( setup_use_thresh_pen_colors_, SIGNAL(changed()), this, SLOT(changeType()) );
    connect( setup_use_thresh_bg_colors_, SIGNAL(changed()), this, SLOT(changeType()) );

    statusCode_ = "NA";
    setText(" ");
    return;
}

void LH_MonitoringText::createThresholdSet(LH_Qt_QColor **color_pen, LH_Qt_QColor **color_bg,
                                           QString title, QColor pen_color, QColor bg_color, bool low_value,
                                           LH_Qt_float **thresh_flt, LH_Qt_int **thresh_int)
{
    for(int i = 0; i<=1; i++)
    {
        if((i==0 && low_value)||(i==1 && !low_value))
        {
            if(color_pen)
            {
                *color_pen = new LH_Qt_QColor(this,title + tr(" Color (Text)"), pen_color, LH_FLAG_HIDDEN);
                (*color_pen)->setHelp( "<p>Color to be used for the foreground text when in the \""+ title + "\" state.</p>");
                (*color_pen)->setOrder(-1);
                connect( *color_pen, SIGNAL(changed()), this, SLOT(updateText()) );
            }

            if(color_bg)
            {
                *color_bg = new LH_Qt_QColor(this,title + tr(" Color (BG)"), bg_color, LH_FLAG_HIDDEN);
                (*color_bg)->setHelp( "<p>Color to be used for the background when in the \""+ title + "\" state.</p>");
                (*color_bg)->setOrder(-1);
                connect( *color_bg, SIGNAL(changed()), this, SLOT(updateText()) );
            }
        }
        else
        {
            QString thresh_label = QString("<p>Values %1 this amount trigger the \"%2\" state.</p>").arg(low_value? "below" : "equal to or above").arg(title);

            if(thresh_flt)
            {
                *thresh_flt = new LH_Qt_float(this, title, 0, LH_FLAG_READONLY | LH_FLAG_HIDDEN);
                (*thresh_flt)->setHelp( thresh_label );
                (*thresh_flt)->setOrder(-1);
                connect( *thresh_flt, SIGNAL(changed()), this, SLOT(updateText()) );
                connect( *thresh_flt, SIGNAL(changed()), this, SLOT(syncThresh_Float()) );
            }

            if(thresh_int)
            {
                *thresh_int = new LH_Qt_int(this,tr(" ")+title, 0, LH_FLAG_READONLY | LH_FLAG_HIDDEN);
                (*thresh_int)->setHelp( thresh_label );
                (*thresh_int)->setOrder(-1);
                connect( *thresh_int, SIGNAL(changed()), this, SLOT(updateText()) );
                connect( *thresh_int, SIGNAL(changed()), this, SLOT(syncThresh_Int()) );
            }
        }
    }
}

LH_MonitoringText::~LH_MonitoringText()
{

}

QColor LH_MonitoringText::pencolor()
{
    if(setup_use_thresh_pen_colors_->value())
    {
        if(statusCode_=="VLO") return setup_thresh_vlo_color_pen_->value();
        if(statusCode_=="LO") return setup_thresh_lo_color_pen_->value();
        if(statusCode_=="OK") return setup_thresh_ok_color_pen_->value();
        if(statusCode_=="HI") return setup_thresh_hi_color_pen_->value();
        if(statusCode_=="VHI") return setup_thresh_vhi_color_pen_->value();
    }
    return setup_pencolor_->value();
};

QColor LH_MonitoringText::bgcolor()
{
    if(setup_use_thresh_bg_colors_->value())
    {
        if(statusCode_=="VLO") return setup_thresh_vlo_color_bg_->value();
        if(statusCode_=="LO") return setup_thresh_lo_color_bg_->value();
        if(statusCode_=="OK") return setup_thresh_ok_color_bg_->value();
        if(statusCode_=="HI") return setup_thresh_hi_color_bg_->value();
        if(statusCode_=="VHI") return setup_thresh_vhi_color_bg_->value();
    }
    return setup_bgcolor_->value();
};

int LH_MonitoringText::notify(int n,void* p)
{
    if( !n || n&LH_NOTE_SECOND )
        updateText();

    return LH_Text::notify(n,p) | LH_NOTE_SECOND;
};

void LH_MonitoringText::updateText(bool rerender)
{
    if(data_)
    {
        QString valueString = data_->getText(setup_append_units_->value(), setup_value_round_->value());
        QString resultText = setup_pre_text_->value() + valueString + setup_post_text_->value();

        if(setup_use_thresh_pen_colors_->value() || setup_use_thresh_bg_colors_->value())
        {
            QString newStatusCode = "NA";
            float val;
            if(data_->getValue(val))
            {
                newStatusCode = "OK";
                if (data_->getType()==mdtFloat) {
                    if (val<setup_thresh_vlo_float_->value())  newStatusCode = "VLO"; else
                    if (val<setup_thresh_lo_float_->value())   newStatusCode = "LO";  else
                    if (val>=setup_thresh_vhi_float_->value()) newStatusCode = "VHI"; else
                    if (val>=setup_thresh_hi_float_->value())  newStatusCode = "HI";
                } else {
                    if (val<setup_thresh_vlo_int_->value())  newStatusCode = "VLO"; else
                    if (val<setup_thresh_lo_int_->value())   newStatusCode = "LO";  else
                    if (val>=setup_thresh_vhi_int_->value()) newStatusCode = "VHI"; else
                    if (val>=setup_thresh_hi_int_->value())  newStatusCode = "HI";
                }
            }
            if(statusCode_!=newStatusCode)
            {
                statusCode_ = newStatusCode;
                rerender = true;
            }
        }
        rerender = setText( resultText ) || rerender;

        if( rerender ) callback(lh_cb_render,NULL);
    }
}

void LH_MonitoringText::setVisibility(LH_Qt_int* qt, bool Hidden, int minValue, int maxValue)
{
    Hidden = Hidden | !(setup_use_thresh_pen_colors_->value() | setup_use_thresh_bg_colors_->value());
    qt->setFlag(LH_FLAG_READONLY, Hidden);
    qt->setFlag(LH_FLAG_HIDDEN, Hidden);
    qt->item()->param.slider.min = minValue;
    qt->item()->param.slider.max = maxValue;
    qt->refresh();
}

void LH_MonitoringText::setVisibility(LH_Qt_float* qt, bool Hidden, float minValue, float maxValue)
{
    Hidden = Hidden | !(setup_use_thresh_pen_colors_->value() | setup_use_thresh_bg_colors_->value());
    qt->setFlag(LH_FLAG_READONLY, Hidden);
    qt->setFlag(LH_FLAG_HIDDEN, Hidden);
    qt->item()->param.range.min = minValue;
    qt->item()->param.range.max = maxValue;
    qt->refresh();
}

void LH_MonitoringText::changeType()
{
    setup_value_round_->setFlag(LH_FLAG_HIDDEN, data_->getType()!=mdtFloat);
    setup_use_thresh_pen_colors_->setFlag(LH_FLAG_HIDDEN, data_->getType()==mdtText);
    setup_use_thresh_bg_colors_->setFlag(LH_FLAG_HIDDEN, data_->getType()==mdtText);

    setup_thresh_vlo_color_pen_->setFlag(LH_FLAG_HIDDEN,!setup_use_thresh_pen_colors_->value());
    setup_thresh_lo_color_pen_->setFlag(LH_FLAG_HIDDEN,!setup_use_thresh_pen_colors_->value());
    setup_thresh_ok_color_pen_->setFlag(LH_FLAG_HIDDEN,!setup_use_thresh_pen_colors_->value());
    setup_thresh_hi_color_pen_->setFlag(LH_FLAG_HIDDEN,!setup_use_thresh_pen_colors_->value());
    setup_thresh_vhi_color_pen_->setFlag(LH_FLAG_HIDDEN,!setup_use_thresh_pen_colors_->value());

    setup_thresh_vlo_color_bg_->setFlag(LH_FLAG_HIDDEN,!setup_use_thresh_bg_colors_->value());
    setup_thresh_lo_color_bg_->setFlag(LH_FLAG_HIDDEN,!setup_use_thresh_bg_colors_->value());
    setup_thresh_ok_color_bg_->setFlag(LH_FLAG_HIDDEN,!setup_use_thresh_bg_colors_->value());
    setup_thresh_hi_color_bg_->setFlag(LH_FLAG_HIDDEN,!setup_use_thresh_bg_colors_->value());
    setup_thresh_vhi_color_bg_->setFlag(LH_FLAG_HIDDEN,!setup_use_thresh_bg_colors_->value());

    if(data_)
    {
        setVisibility(setup_thresh_vlo_int_,   data_->getType()!=mdtInt, data_->getThreshMin(), data_->getThreshMax());
        setVisibility(setup_thresh_lo_int_,    data_->getType()!=mdtInt, data_->getThreshMin(), data_->getThreshMax());
        setVisibility(setup_thresh_hi_int_,    data_->getType()!=mdtInt, data_->getThreshMin(), data_->getThreshMax());
        setVisibility(setup_thresh_vhi_int_,   data_->getType()!=mdtInt, data_->getThreshMin(), data_->getThreshMax());

        setVisibility(setup_thresh_vlo_float_, data_->getType()!=mdtFloat, data_->getThreshMin(), data_->getThreshMax());
        setVisibility(setup_thresh_lo_float_,  data_->getType()!=mdtFloat, data_->getThreshMin(), data_->getThreshMax());
        setVisibility(setup_thresh_hi_float_,  data_->getType()!=mdtFloat, data_->getThreshMin(), data_->getThreshMax());
        setVisibility(setup_thresh_vhi_float_, data_->getType()!=mdtFloat, data_->getThreshMin(), data_->getThreshMax());
    }
    updateText();
}

void LH_MonitoringText::connect_updateText(QObject* obj)
{
    connect( obj, SIGNAL(changed()), this, SLOT(updateText()) );
}

void LH_MonitoringText::connect_changeType(QObject* obj)
{
    connect( obj, SIGNAL(changed()), this, SLOT(changeType()) );
}

void LH_MonitoringText::syncThresh_Int()
{
    setup_thresh_vlo_float_->setValue((float)setup_thresh_vlo_int_->value());
    setup_thresh_lo_float_->setValue((float)setup_thresh_lo_int_->value());
    setup_thresh_hi_float_->setValue((float)setup_thresh_hi_int_->value());
    setup_thresh_vhi_float_->setValue((float)setup_thresh_vhi_int_->value());
}

void LH_MonitoringText::syncThresh_Float()
{
    setup_thresh_vlo_int_->setValue(qRound(setup_thresh_vlo_float_->value()));
    setup_thresh_lo_int_->setValue(qRound(setup_thresh_lo_float_->value()));
    setup_thresh_hi_int_->setValue(qRound(setup_thresh_hi_float_->value()));
    setup_thresh_vhi_int_->setValue(qRound(setup_thresh_vhi_float_->value()));
}
