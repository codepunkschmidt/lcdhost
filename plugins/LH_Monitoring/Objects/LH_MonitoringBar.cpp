/**
  \file     LH_MonitoringBar.cpp
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese
    This module is based on original work by Johan Lindh.

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

#include "LH_MonitoringBar.h"

LH_PLUGIN_CLASS(LH_MonitoringBar)

lh_class *LH_MonitoringBar::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        STRINGIZE(MONITORING_FOLDER),
        STRINGIZE(COMMON_OBJECT_NAME)"Bar",
        STRINGIZE(COMMON_OBJECT_NAME)" (Bar)",
        48,48
        
    };

    return &classInfo;
}

LH_MonitoringBar::LH_MonitoringBar() : LH_Bar()
{
    ui_ = NULL;
}

const char *LH_MonitoringBar::userInit()
{
    if( const char *err = LH_Bar::userInit() ) return err;

    ui_ = new LH_MonitoringUI(this, mdmNumbers, true, false);

    setup_max_ = new LH_Qt_int(this, "Maximum", 100, 0, 99999);
    setup_max_->setHelp( "<p>The bar's maximum value.</p>");
    setup_max_->setOrder(-3);
    connect( setup_max_, SIGNAL(changed()), this, SLOT(updateBounds()) );

    setup_min_ = new LH_Qt_int(this, "Minimum", 0, 0, 99999);
    setup_min_->setHelp( "<p>The bar's minimum value.</p>");
    setup_min_->setOrder(-3);
    connect( setup_min_, SIGNAL(changed()), this, SLOT(updateBounds()) );

    (new LH_Qt_QString(this,("image-hr2"), QString("<hr>"), LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_HIDETITLE,lh_type_string_html ))->setOrder(-3);

    updateBounds();

    connect(ui_, SIGNAL(appChanged()), this, SLOT(configChanged()) );
    connect(ui_, SIGNAL(typeChanged()), this, SLOT(configChanged()) );
    connect(ui_, SIGNAL(groupChanged()), this, SLOT(configChanged()) );
    connect(ui_, SIGNAL(itemChanged()), this, SLOT(configChanged()) );
    connect(ui_, SIGNAL(initialized()), this, SLOT(configChanged()) );

    return 0;
}


int LH_MonitoringBar::notify(int n, void *p)
{
    Q_UNUSED(p);
    if(!n || n&LH_NOTE_SECOND)
        requestRender();
    return LH_Bar::notify(n,p) | LH_NOTE_SECOND;
}

QImage *LH_MonitoringBar::render_qimage( int w, int h )
{
    if( LH_Bar::render_qimage(w,h) == NULL ) return NULL;
    if(ui_ && ui_->data_)
    {
        float max;
        if(ui_->data_->getUpperLimit(max))
        {
            this->setup_max_->setHidden(true);
            this->setup_min_->setHidden(true);
            this->setMax((qreal)max);
            this->setMin((qreal)0);
        }
        else
        {
            this->setup_max_->setHidden(false);
            this->setup_min_->setHidden(false);
        }
        if(!ui_->data_->isGroup())
        {
            float currVal=0;
            if(ui_->data_->getValue(currVal))
                drawSingle( currVal );
        } else {
            int barCount;
            ui_->data_->getCount(barCount);

            float deadVal;
            bool hasDead = (ui_->data_->getDeadValue_Transformed(deadVal));

            QVector<qreal> currVals;
            for(int i=0; i<barCount; i++)
            {
                float currVal=0;
                ui_->data_->getValue(currVal, i);
                if (!hasDead || (deadVal != currVal) )
                    currVals.append(currVal);
            }
            drawList(currVals);
        }
    }
    return image_;
}

void LH_MonitoringBar::refresh()
{
    requestRender();
}

void LH_MonitoringBar::updateBounds()
{
    setMax(setup_max_->value());
    setMin(setup_min_->value());
    requestRender();
}
