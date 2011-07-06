/**
  \file     LH_MonitoringImage.cpp
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese Copyright (c) 2010 Andy Bridges
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

#include "LH_MonitoringImage.h"

#include <QtGlobal>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QStringList>
#include <QString>
#include <QRegExp>
#include <QHash>

#include "../LH_Qt_QStringList.h"

LH_MonitoringImage::LH_MonitoringImage()
{
    setup_image_file_->setFlags( LH_FLAG_AUTORENDER | LH_FLAG_READONLY | LH_FLAG_NOSAVE );

    LH_Qt_QString* hr = new LH_Qt_QString(this,tr("~Mon-Data-Rule"),"<hr>", LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_NOSINK,lh_type_string_html );
    hr->setOrder(-1);

    setup_value_ = new LH_Qt_QString( this, tr("Current Value"), "N/A", LH_FLAG_READONLY|LH_FLAG_NOSAVE );
    setup_value_->setOrder(-1);

    LH_Qt_QString* hr2 =new LH_Qt_QString(this,tr("~Mon-Props-Rule"),"<hr>", LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_NOSINK,lh_type_string_html );
    hr2->setOrder(-1);

    add_cf_target(setup_image_file_);
    add_cf_source(setup_value_);

    setup_cf_state_->setFlag(LH_FLAG_HIDDEN, false);
}

int LH_MonitoringImage::notify(int n,void* p)
{
    Q_UNUSED(p);
    if( !n || n&LH_NOTE_SECOND )
        updateValue();
    return LH_QImage::notify(n,p) | LH_NOTE_SECOND;
}

void LH_MonitoringImage::updateValue()
{
    QString val;
    if(data_)
    {
        setup_value_->setValue(data_->getText(false));
    }
}

void LH_MonitoringImage::changeType()
{
    updateValue();
}

void LH_MonitoringImage::connect_changeType(QObject* obj)
{
    connect( obj, SIGNAL(changed()), this, SLOT(changeType()) );
}

void LH_MonitoringImage::connect_updateImage(QObject* obj)
{
    connect( obj, SIGNAL(changed()), this, SLOT(updateValue()) );
}
