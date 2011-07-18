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

LH_PLUGIN_CLASS(LH_MonitoringImage)

lh_class *LH_MonitoringImage::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "3rdParty/Monitoring",
        "MonitoringImage",
        "Monitoring (Image)",
        48,48,
        lh_instance_calltable_NULL
    };

    return &classInfo;
}

LH_MonitoringImage::LH_MonitoringImage() : LH_QImage()
{
    ui_ = NULL;
}

const char *LH_MonitoringImage::userInit()
{
    if( const char *err = LH_QImage::userInit() ) return err;

    ui_ = new LH_MonitoringUI(this, mdmAll, false);

    setup_image_file_->setFlags( LH_FLAG_AUTORENDER | LH_FLAG_READONLY | LH_FLAG_NOSAVE );

    LH_Qt_QString* hr = new LH_Qt_QString(this,tr("~Mon-Data-Rule"), QString(), LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_NOSINK,lh_type_string_htmlhelp );
    hr->setHelp("<hr>");
    hr->setOrder(-1);

    setup_value_ = new LH_Qt_QString( this, tr("Current Value"), "N/A", LH_FLAG_READONLY|LH_FLAG_NOSAVE );
    setup_value_->setOrder(-1);

    LH_Qt_QString* hr2 =new LH_Qt_QString(this,tr("~Mon-Props-Rule"), QString(), LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_NOSINK,lh_type_string_htmlhelp );
    hr2->setHelp("<hr>");
    hr2->setOrder(-1);

    add_cf_target(setup_image_file_);
    add_cf_source(setup_value_);

    connect( ui_->setup_value_type_, SIGNAL(changed()), this, SLOT(changeType()) );
    return 0;
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
    if(ui_ && ui_->data_)
    {
        setup_value_->setValue(ui_->data_->getText(false));
    }
}

void LH_MonitoringImage::changeType()
{
    updateValue();
}
