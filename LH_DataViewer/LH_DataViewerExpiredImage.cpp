/**
  \file     LH_DataViewerExpiredImage.cpp
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

#include <QtGlobal>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QStringList>
#include <QString>
#include <QRegExp>
#include <QHash>

#include "LH_DataViewerExpiredImage.h"
#include "../LH_Qt_QStringList.h"

LH_PLUGIN_CLASS(LH_DataViewerExpiredImage)

lh_class *LH_DataViewerExpiredImage::classInfo()
{
    static lh_class classinfo =
    {
        sizeof(lh_class),
        "Dynamic/DataViewer",
        "DataViewerExpiredImage",
        "Data Expired Image",
        -1, -1,
        lh_instance_calltable_NULL
    };

    return &classinfo;
}

LH_DataViewerExpiredImage::LH_DataViewerExpiredImage() : data_(this)
{
    setup_file_ = new LH_Qt_QFileInfo( this, tr("File"), QFileInfo(), LH_FLAG_AUTORENDER );
    setup_file_->setOrder(-1);
    connect( setup_file_, SIGNAL(changed()), this, SLOT(fileChanged()) );
    isExpired = true;
}

LH_DataViewerExpiredImage::~LH_DataViewerExpiredImage()
{
    return;
}

int LH_DataViewerExpiredImage::polling()
{
    updateImage();
    return polling_rate;
}

int LH_DataViewerExpiredImage::width( void*obj,int h )
{
    Q_UNUSED(obj);
    Q_UNUSED(h);
    return -1;
}

int LH_DataViewerExpiredImage::height( void*obj,int h )
{
    Q_UNUSED(obj);
    Q_UNUSED(h);
    return -1;
}

QImage *LH_DataViewerExpiredImage::render_qimage(int w, int h)
{
    delete image_;
    if( setup_file_->value().isFile() )
    {
        //setup_file_->value;
        //QString folderPath = setup_file_->value().dir().path() + "/";
        //QString imageName = getImageName();


        if ( isExpired )
        {
            image_ = new QImage(setup_file_->value().absoluteFilePath());
        } else
        {
            uchar *data = new uchar[4];
            data[0] = 255;
            data[1] = 0;
            data[2] = 0;
            data[3] = 0;

            image_ = new QImage(data,1,1,QImage::Format_ARGB32);
        }            
    } else
        image_ = new QImage(w,h,QImage::Format_Invalid);
    return image_;
}

void LH_DataViewerExpiredImage::fileChanged()
{
    updateImage(true);
}

void LH_DataViewerExpiredImage::updateImage(bool rerender)
{
    if (data_.open()) {
        if(isExpired != data_.expired) rerender = true;
        isExpired = data_.expired;
    }
    if (rerender) callback(lh_cb_render,NULL);
}
