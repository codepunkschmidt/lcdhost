/**
  \file     LH_MonitoringDial.cpp
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

#include "LH_MonitoringDial.h"

LH_MonitoringDial::LH_MonitoringDial()
{
    LH_Qt_QString* hr = new LH_Qt_QString(this,tr("~Mon-Data-Rule"),"<hr>", LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_NOSINK,lh_type_string_html );
    hr->setOrder(-3);

    setup_max_ = new LH_Qt_int(this, "Maximum", 100, 0, 99999);
    setup_max_->setHelp( "<p>The dial's maximum value.</p>");
    setup_max_->setOrder(-3);
    connect( setup_max_, SIGNAL(changed()), this, SLOT(updateBounds()) );

    setup_min_ = new LH_Qt_int(this, "Minimum", 0, 0, 99999);
    setup_min_->setHelp( "<p>The dial's minimum value.</p>");
    setup_min_->setOrder(-3);
    connect( setup_min_, SIGNAL(changed()), this, SLOT(updateBounds()) );

    LH_Qt_QString* hr2 =new LH_Qt_QString(this,tr("~Mon-Props-Rule"),"<hr>", LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_NOSINK,lh_type_string_html );
    hr2->setOrder(-3);

    updateBounds();

    pollTimer_.start();
}

LH_MonitoringDial::~LH_MonitoringDial()
{

}

int LH_MonitoringDial::polling()
{
    if(pollTimer_.elapsed()>=190 && data_)
    {
        pollTimer_.restart();
        float deadVal;
        bool hasDead = (data_->getDeadValue_Transformed(deadVal));

        int count;
        data_->getCount(count);
        if(needleCount() != count)
            updateNeedles();

        if(!data_->isGroup())
        {
            float currVal=0;
            bool hasData = data_->getValue(currVal);
            bool isDead = (hasDead && (deadVal == currVal));
            setNeedleVisibility( hasData && !isDead );
            setVal( currVal );
        } else {
            QVector<qreal> currVals;
            for(int i=0; i<count; i++)
            {
                float currVal=0;
                bool hasData = data_->getValue(currVal, i);
                bool isDead = (hasDead && (deadVal == currVal));
                setNeedleVisibility( hasData && !isDead, i );
                currVals.append(currVal);
            }
            setVal(currVals);
        }
    }
    int basePoll = LH_Dial::polling();
    return (basePoll==0? 200 : basePoll);
}


void LH_MonitoringDial::refresh()
{
    requestRender();
}

void LH_MonitoringDial::updateBounds()
{
    setMax(setup_max_->value());
    setMin(setup_min_->value());
    requestRender();
}

void LH_MonitoringDial::updateNeedles()
{
    QStringList names;
    data_->getNames(names);
    setNeedles(names);
}

void LH_MonitoringDial::connect_refresh(QObject* obj)
{
    connect( obj, SIGNAL(changed()), this, SLOT(refresh()) );
}
