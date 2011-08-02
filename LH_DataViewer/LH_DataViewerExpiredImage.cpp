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
        
        
    };

    return &classinfo;
}

const char *LH_DataViewerExpiredImage::userInit()
{
    if( const char *err = LH_QtInstance::userInit() ) return err;
    setup_file_ = new LH_Qt_QFileInfo( this, ("File"), QFileInfo(), LH_FLAG_AUTORENDER );
    setup_file_->setOrder(-1);
    connect( setup_file_, SIGNAL(changed()), this, SLOT(fileChanged()) );
    isExpired = true;
    return 0;
}

int LH_DataViewerExpiredImage::polling()
{
    updateImage();
    return polling_rate;
}

QImage *LH_DataViewerExpiredImage::render_qimage(int w, int h)
{
    delete image_;
    if( setup_file_->value().isFile() )
        image_ = new QImage(setup_file_->value().absoluteFilePath());
    else
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
    setVisible(isExpired);
}
