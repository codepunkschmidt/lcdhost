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

LH_MonitoringImage::LH_MonitoringImage(const char *name, LH_QtPlugin *parent ) : LH_QtInstance( name, 0, parent )//, data_(this)
{
    setup_file_ = new LH_Qt_QFileInfo( this, tr("File"), QFileInfo(), LH_FLAG_AUTORENDER );
    setup_file_->setHelp( "<p>The image map file instructs the status image on how to match up "
                          "a status code with the appropriate images in the same folder.</p>"
                          "<p>The format is as follows:</p>"
                          "<p>&lt;Code&gt;	&lt;Image&gt;</p>"
                          "<p>Note that each item is seperated by a tab.</p>"
                          "<p>e.g.<br/>"
                          "VLO	veryLow.png</p>");
    setup_file_->setOrder(-1);
    connect( setup_file_, SIGNAL(changed()), this, SLOT(fileChanged()) );

    createThresholdSet("Very Low",  setup_thresh_vlo_float_, setup_thresh_vlo_int_, true  );
    createThresholdSet("Low",       setup_thresh_lo_float_,  setup_thresh_lo_int_,  true  );
    createThresholdSet("High",      setup_thresh_hi_float_,  setup_thresh_hi_int_,  false );
    createThresholdSet("Very High", setup_thresh_vhi_float_, setup_thresh_vhi_int_, false );

    setup_text_ = new LH_Qt_QString( this, tr("~"), QString(), LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_AUTORENDER );
    setup_text_->setOrder(-1);

    statusCode_ = "NA";
    imageDefinitions = new QHash<QString, QStringList>();
}

void LH_MonitoringImage::createThresholdSet(QString title, LH_Qt_float *&thresh_flt, LH_Qt_int *&thresh_int, bool low_value)
{
    QString thresh_label = QString("<p>Values %1 this amount trigger the \"%2\" state.</p>").arg(low_value? "below" : "equal to or above").arg(title);

    thresh_flt = new LH_Qt_float(this, title, 0, LH_FLAG_READONLY | LH_FLAG_HIDDEN);
    thresh_flt->setHelp( thresh_label );
    thresh_flt->setOrder(-1);
    connect( thresh_flt, SIGNAL(changed()), this, SLOT(updateImage()) );
    connect( thresh_flt, SIGNAL(changed()), this, SLOT(syncThresh_Float()) );

    thresh_int = new LH_Qt_int(this, " "+title, 0, LH_FLAG_READONLY | LH_FLAG_HIDDEN);
    thresh_int->setHelp( thresh_label );
    thresh_int->setOrder(-1);
    connect( thresh_int, SIGNAL(changed()), this, SLOT(updateImage()) );
    connect( thresh_int, SIGNAL(changed()), this, SLOT(syncThresh_Int()) );
}

LH_MonitoringImage::~LH_MonitoringImage()
{

}

int LH_MonitoringImage::polling()
{
    return 0;
}

int LH_MonitoringImage::notify(int n,void* p)
{
    Q_UNUSED(p);
    if( !n || n&LH_NOTE_SECOND )
        updateImage();
    return LH_NOTE_SECOND;
}

int LH_MonitoringImage::width( void*obj,int h )
{
    Q_UNUSED(obj);
    Q_UNUSED(h);
    return -1;
}

int LH_MonitoringImage::height( void*obj,int h )
{
    Q_UNUSED(obj);
    Q_UNUSED(h);
    return -1;
}

QImage *LH_MonitoringImage::render_qimage(int w, int h)
{
    delete image_;
    if( setup_file_->value().isFile() )
    {
        QString folderPath = setup_file_->value().dir().path() + "/";
        QString imageName = getImageName();

        if (imageName=="")
            image_ = new QImage();
        else
            image_ = new QImage(folderPath + imageName);
    } else
        image_ = new QImage(w,h,QImage::Format_Invalid);
    return image_;
}

QString LH_MonitoringImage::getImageName()
{
    QStringList imageItem;

    QString fallbackStatus = "OK";

    if (statusCode_=="VLO") fallbackStatus="LO";
    if (statusCode_=="VHI") fallbackStatus="HI";
    if (statusCode_=="LO") fallbackStatus="OK";
    if (statusCode_=="HI") fallbackStatus="OK";

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

void LH_MonitoringImage::fileChanged()
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
            changeType();
            updateImage(true);
        } else {
            setup_text_->setValue(tr("Unable to open file."));
            setup_text_->setFlag(LH_FLAG_HIDDEN,false);
            return;
        }
    }
}

void LH_MonitoringImage::setVisibility(LH_Qt_int* qt, bool ReadOnly, bool Hidden, int minValue, int maxValue)
{
    qt->setFlag(LH_FLAG_READONLY,ReadOnly);
    qt->setFlag(LH_FLAG_HIDDEN,Hidden);
    qt->item()->param.slider.min = minValue;
    qt->item()->param.slider.max = maxValue;
    qt->refresh();
}

void LH_MonitoringImage::setVisibility(LH_Qt_float* qt, bool ReadOnly, bool Hidden, float minValue, float maxValue)
{
    qt->setFlag(LH_FLAG_READONLY,ReadOnly);
    qt->setFlag(LH_FLAG_HIDDEN,Hidden);
    qt->item()->param.range.min = minValue;
    qt->item()->param.range.max = maxValue;
    qt->refresh();
}

void LH_MonitoringImage::updateImage(bool rerender)
{
    setup_text_->setFlag(LH_FLAG_HIDDEN,false);
    setup_text_->setValue("Accessing memory..." );

    float val;
    QString newStatusCode = "NA";
    if(data_)
    {
        if(data_->getValue(val))
        {
            newStatusCode = "OK";
            if (data_->getType() == mdtFloat) {
                if (imageDefinitions->contains("VLO") && val<setup_thresh_vlo_float_->value())  newStatusCode = "VLO"; else
                if (imageDefinitions->contains("LO")  && val<setup_thresh_lo_float_->value())   newStatusCode = "LO";  else
                if (imageDefinitions->contains("VHI") && val>=setup_thresh_vhi_float_->value()) newStatusCode = "VHI"; else
                if (imageDefinitions->contains("HI")  && val>=setup_thresh_hi_float_->value())  newStatusCode = "HI";
            } else {
                if (imageDefinitions->contains("VLO") && val<setup_thresh_vlo_int_->value())  newStatusCode = "VLO"; else
                if (imageDefinitions->contains("LO")  && val<setup_thresh_lo_int_->value())   newStatusCode = "LO";  else
                if (imageDefinitions->contains("VHI") && val>=setup_thresh_vhi_int_->value()) newStatusCode = "VHI"; else
                if (imageDefinitions->contains("HI")  && val>=setup_thresh_hi_int_->value())  newStatusCode = "HI";
            }
            setup_text_->setValue(QString("Value %1 (%2) received; resolved to image: %3").arg(QString("%1").arg(val), statusCode_, getImageName()));
        } else
            setup_text_->setValue( "Failed to open shared memory." ) ;
    }
    if(statusCode_!=newStatusCode)
    {
        statusCode_ = newStatusCode;
        rerender = true;
    }
    if (rerender) callback(lh_cb_render,NULL);
}

void LH_MonitoringImage::changeType()
{
    setVisibility(setup_thresh_vlo_int_,   !imageDefinitions->contains("VLO"), (data_->getType()!=mdtInt) | !imageDefinitions->contains("VLO"), data_->getThreshMin(), data_->getThreshMax());
    setVisibility(setup_thresh_lo_int_,    !imageDefinitions->contains("LO"),  (data_->getType()!=mdtInt) | !imageDefinitions->contains("LO"), data_->getThreshMin(), data_->getThreshMax());
    setVisibility(setup_thresh_hi_int_,    !imageDefinitions->contains("HI"),  (data_->getType()!=mdtInt) | !imageDefinitions->contains("HI"), data_->getThreshMin(), data_->getThreshMax());
    setVisibility(setup_thresh_vhi_int_,   !imageDefinitions->contains("VHI"), (data_->getType()!=mdtInt) | !imageDefinitions->contains("VHI"), data_->getThreshMin(), data_->getThreshMax());

    setVisibility(setup_thresh_vlo_float_, !imageDefinitions->contains("VLO"), (data_->getType()!=mdtFloat) | !imageDefinitions->contains("VLO"), data_->getThreshMin(), data_->getThreshMax());
    setVisibility(setup_thresh_lo_float_,  !imageDefinitions->contains("LO"),  (data_->getType()!=mdtFloat) | !imageDefinitions->contains("LO"), data_->getThreshMin(), data_->getThreshMax());
    setVisibility(setup_thresh_hi_float_,  !imageDefinitions->contains("HI"),  (data_->getType()!=mdtFloat) | !imageDefinitions->contains("HI"), data_->getThreshMin(), data_->getThreshMax());
    setVisibility(setup_thresh_vhi_float_, !imageDefinitions->contains("VHI"), (data_->getType()!=mdtFloat) | !imageDefinitions->contains("VHI"), data_->getThreshMin(), data_->getThreshMax());

    updateImage();
}

void LH_MonitoringImage::connect_changeType(QObject* obj)
{
    connect( obj, SIGNAL(changed()), this, SLOT(changeType()) );
}

void LH_MonitoringImage::connect_updateImage(QObject* obj)
{
    connect( obj, SIGNAL(changed()), this, SLOT(updateImage()) );
}

void LH_MonitoringImage::syncThresh_Int()
{
    setup_thresh_vlo_float_->setValue((float)setup_thresh_vlo_int_->value());
    setup_thresh_lo_float_->setValue((float)setup_thresh_lo_int_->value());
    setup_thresh_hi_float_->setValue((float)setup_thresh_hi_int_->value());
    setup_thresh_vhi_float_->setValue((float)setup_thresh_vhi_int_->value());
}

void LH_MonitoringImage::syncThresh_Float()
{
    setup_thresh_vlo_int_->setValue(qRound(setup_thresh_vlo_float_->value()));
    setup_thresh_lo_int_->setValue(qRound(setup_thresh_lo_float_->value()));
    setup_thresh_hi_int_->setValue(qRound(setup_thresh_hi_float_->value()));
    setup_thresh_vhi_int_->setValue(qRound(setup_thresh_vhi_float_->value()));
}
