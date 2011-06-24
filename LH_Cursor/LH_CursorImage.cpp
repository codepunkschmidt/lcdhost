/**
  \file     LH_CursorImage.cpp
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

#include "LH_CursorImage.h"

#include <QtGlobal>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QStringList>
#include <QString>
#include <QRegExp>
#include <QHash>

// #include <windows.h>
#include "../LH_Qt_QStringList.h"

LH_PLUGIN_CLASS(LH_CursorImage)

lh_class *LH_CursorImage::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "Cursor",
        "CursorImage",
        "Cursor Image",
        48,48,
        lh_object_calltable_NULL,
        lh_instance_calltable_NULL
    };

    return &classInfo;
}

LH_CursorImage::LH_CursorImage()
{
    setup_file_ = new LH_Qt_QFileInfo( this, tr("File"), QFileInfo(), LH_FLAG_AUTORENDER );
    setup_file_->setHelp( "<p>The image map file instructs the status image on how to match up "
                          "a state with the appropriate images in the same folder.</p>"
                          "<p>There are four states for Cursor Images:"
                          "<ul>"
                          "<li><b>OFF</b> (The cursor is not over this item, nor is it selected)</li>"
                          "<li><b>ON</b> (The cursor is over this item, but it is <i>not</i> selected)</li>"
                          "<li><b>OFF_SEL</b> (The cursor is not over this item, but it <i>is</i> selected)</li>"
                          "<li><b>ON_SEL</b> (The cursor is over this item, and it <i>is</i> selected)</li>"
                          "</ul>"
                          "</p>"
                          "<p>The format is as follows:</p>"
                          "<p>&lt;Code&gt;	&lt;Image&gt;</p>"
                          "<p>Note that each item is seperated by a tab.</p>"
                          "<p>e.g.<br/>"
                          "OFF	blank.png<br/>"
                          "ON	green_arrow.png<br/>"
                          "OFF_SEL	blue_arrow.png<br/>"
                          "ON_SEL	red_arrow.png"
                          "</p>");
    setup_file_->setOrder(-1);
    connect( setup_file_, SIGNAL(changed()), this, SLOT(fileChanged()) );

    setup_coordinate_ = new LH_Qt_QString(this, "Coordinate", "1,1", LH_FLAG_AUTORENDER);
    setup_coordinate_->setHelp("This is the coordinate of this object, i.e. when the cursor is at the point specified here this object is selected. <br/>"
                               "<br/>"
                               "Note that many objects can have the same coordinate if the user requires.<br/>"
                               "<br/>"
                               "The format is [x],[y] <br/>"
                               "e.g.: 1,1"
                               );

    setup_text_ = new LH_Qt_QString( this, tr("~"), QString(), LH_FLAG_READONLY|LH_FLAG_NOSAVE|LH_FLAG_HIDDEN|LH_FLAG_AUTORENDER );
    setup_text_->setOrder(-1);

    statusCode_ = "OFF";
    imageDefinitions = new QHash<QString, QStringList>();
}

int LH_CursorImage::polling()
{
    if(updateState())  callback(lh_cb_render,NULL);
    return 100;
}

QImage *LH_CursorImage::render_qimage(int w, int h)
{
    delete image_;
    if( setup_file_->value().isFile() )
    {
        QString folderPath = setup_file_->value().dir().path() + "/";
        QString imageName = getImageName();

        if (imageName=="")
        {
            uchar *data = new uchar[4];
            data[0] = 255;
            data[1] = 0;
            data[2] = 0;
            data[3] = 0;
            image_ = new QImage(data,1,1,QImage::Format_ARGB32);
        }
        else
            image_ = new QImage(folderPath + imageName);
    } else
        image_ = new QImage(w,h,QImage::Format_Invalid);
    return image_;
}

QString LH_CursorImage::getImageName()
{
    QStringList imageItem;

    QString fallbackStatus = "OFF";

    if (statusCode_=="ON_SEL") fallbackStatus="ON";
    if (statusCode_=="OFF_SEL") fallbackStatus="OFF";

    if (imageDefinitions->contains(statusCode_))
        imageItem = imageDefinitions->value(statusCode_);
    else if (imageDefinitions->contains(fallbackStatus))
        imageItem = imageDefinitions->value(fallbackStatus);
    else
        imageItem = QStringList();

    QString imageName;
    if (imageItem.count()==0)
        imageName = "";
    else
        imageName = imageItem.at(1);

    return imageName;
}

void LH_CursorImage::fileChanged()
{
    setup_file_->value().refresh();
    if( !setup_file_->value().isFile() )
    {
        setup_text_->setValue(tr("No such file."));
        setup_text_->setFlag(LH_FLAG_HIDDEN,false);
        return;
    }
    else
    {
        setup_text_->setFlag(LH_FLAG_HIDDEN,true);
        QFile file( setup_file_->value().filePath() );

        if( file.open( QIODevice::ReadOnly) )
        {
            QTextStream stream(&file);
            QRegExp re = QRegExp(";.*$");
            QString fileContent = stream.readAll();

            QStringList items = fileContent.split('\r',QString::SkipEmptyParts);
            imageDefinitions->clear();

            foreach (QString item, items)
            {
                item = item.remove(re).trimmed();
                if (item!="")
                {
                    QStringList parts = item.split('\t',QString::SkipEmptyParts);
                    imageDefinitions->insert(QString(parts.at(0)), parts);
                }
            }
            callback(lh_cb_render,NULL);
        } else {
            setup_text_->setValue(tr("Unable to open file."));
            setup_text_->setFlag(LH_FLAG_HIDDEN,false);
            return;
        }
    }
}

bool LH_CursorImage::updateState()
{
    QStringList mycoords = setup_coordinate_->value().split(';');

    bool newSelected = false;
    bool newActive = false;
    foreach (QString mycoord_str, mycoords)
    {
        QStringList mycoord = mycoord_str.split(',');
        if(mycoord.length()==2)
        {
            int myX = mycoord.at(0).toInt();
            int myY = mycoord.at(1).toInt();

            newSelected = newSelected || ( cursor_data.selState && cursor_data.selX==myX && cursor_data.selY==myY );
            newActive = newActive ||  ( cursor_data.active && cursor_data.x==myX && cursor_data.y==myY );
        }
    }
    QString newStatusCode = QString("%1%2").arg(newActive? "ON" : "OFF").arg(newSelected? "_SEL" : "");
    if(statusCode_ != newStatusCode)
    {
        statusCode_ = newStatusCode;
        return true;
    }
    else
        return false;
}
