/**
  \file     LH_WeatherImage.cpp
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
#include <QSharedMemory>

#include "LH_WeatherImage.h"
#include "LH_Qt_QStringList.h"
#include "LH_Qt_html.h"

LH_PLUGIN_CLASS(LH_WeatherImage)

lh_class *LH_WeatherImage::classInfo()
{
    static lh_class classinfo =
    {
        sizeof(lh_class),
        "Weather",
        "WeatherImage",
        "Weather Image",
        -1, -1,


    };

    return &classinfo;
}

const char *LH_WeatherImage::userInit()
{
    if( const char *err = LH_QtInstance::userInit() ) return err;
    QStringList valueTypes = QStringList();
    valueTypes.append("Current Conditions");
    valueTypes.append("Forecast Day 1 (Today)");
    valueTypes.append("Forecast Day 2 (Tomorrow)");
    valueTypes.append("Forecast Day 3");
    valueTypes.append("Forecast Day 4");
    valueTypes.append("Forecast Day 5");
    setup_value_type_ = new LH_Qt_QStringList(this, "Icon Type", valueTypes, 0);
    setup_value_type_->setHelp( "<p>The type of value to use to determine the image.</p>");
    setup_value_type_->setOrder(-4);
    connect( setup_value_type_, SIGNAL(changed()), this, SLOT(updateImage()) );

    setup_file_ = new LH_Qt_QFileInfo( this, ("File"), QFileInfo(), LH_FLAG_AUTORENDER );
    setup_file_->setHelp( "<p>The image map file instructs the weather image on how to match up "
                          "a weather code with the appropriate images in the same folder.</p>"
                          "<p>The format is as follows:</p>"
                          "<p>&lt;Code&gt;	&lt;Day Image&gt;	&lt;Night Image&gt;</p>"
                          "<p>Note that each item is seperated by a tab.</p>"
                          "<p>e.g.<br/>"
                          "3200	day.png	night.png</p>");
    setup_file_->setOrder(-1);
    connect( setup_file_, SIGNAL(changed()), this, SLOT(fileChanged()) );

    setup_feedback_ = new LH_Qt_html(this, "", LH_FLAG_READONLY | LH_FLAG_NOSINK | LH_FLAG_NOSOURCE );

    imageDefinitions = new QHash<int, QStringList>();

    weatherCode = "3200";
    isNight = false;
    return 0;
}

int LH_WeatherImage::notify(int n,void* p)
{
    Q_UNUSED(p);
    if( !n || n&LH_NOTE_SECOND )
        updateImage();
    return LH_NOTE_SECOND;
}

int LH_WeatherImage::width( void*obj,int h )
{
    Q_UNUSED(obj);
    Q_UNUSED(h);
    return -1;
}

int LH_WeatherImage::height( void*obj,int h )
{
    Q_UNUSED(obj);
    Q_UNUSED(h);
    return -1;
}

QImage *LH_WeatherImage::render_qimage(int w, int h)
{
    delete image_;
    if( setup_file_->value().isFile() )
    {
        QString folderPath = setup_file_->value().dir().path() + "/";
        QString imageName = getWeatherImageName();
        if (imageName=="")
            image_ = new QImage();
        else
            image_ = new QImage(folderPath + imageName);
    } else
        image_ = new QImage(w,h,QImage::Format_Invalid);
    return image_;
}

QString LH_WeatherImage::getWeatherImageName()
{
    QStringList imageItem;
    if (imageDefinitions->contains(weatherCode.toInt()))
        imageItem = imageDefinitions->value(weatherCode.toInt());
    else if (imageDefinitions->contains(3200))
        imageItem = imageDefinitions->value(3200);
    else
        imageItem = QStringList();

    QString imageName;
    if (imageItem.count()==0)
        imageName = "";
    else if (isNight)
        imageName = imageItem.at(2);
    else
        imageName = imageItem.at(1);

    return imageName;
}

void LH_WeatherImage::fileChanged()
{
    setup_file_->value().refresh();
    if( !setup_file_->value().isFile() )
    {
        qWarning() << "LH_Weather: Image map does not exist: " << setup_file_->value().absoluteFilePath();
        return;
    }
    else
    {
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
                     imageDefinitions->insert(QString(parts.at(0)).toInt(), parts);
                }
            }
            updateImage(true);
        } else {
            qWarning() << "LH_Weather: Unable to open file: " << setup_file_->value().absoluteFilePath();
            return;
        }
    }
}

void LH_WeatherImage::updateImage(bool rerender)
{
    bool newIsNight = (setup_value_type_->index()<=1) && weather_data.isNight;
    if (isNight!=newIsNight) rerender = true;
    isNight = newIsNight;

    QString newWeatherCode = weather_data.condition.code;
    switch(setup_value_type_->index())
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        newWeatherCode = weather_data.forecast[setup_value_type_->index()-1].code;
        break;
    }


    if(weatherCode != newWeatherCode) rerender = true;
    weatherCode = newWeatherCode;

    QString dayNight;
    if (isNight)
        dayNight = "Night";
    else
        dayNight = "Day";

    setup_feedback_->setHelp(QString("Weather Code %1 received for %2; resolved to image: %3").arg(weatherCode, dayNight, getWeatherImageName()));

    if (rerender) callback(lh_cb_render,NULL);
}
