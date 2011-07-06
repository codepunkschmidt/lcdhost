/**
  \file     LH_MonitoringConditionImage.cpp
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

#include "LH_MonitoringConditionImage.h"

#include <QtGlobal>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QStringList>
#include <QString>
#include <QRegExp>
#include <QHash>
#include <QTime>
#include <QRgb>

#include "../LH_Qt_QStringList.h"

LH_MonitoringConditionImage::LH_MonitoringConditionImage( )
{
    conditionsListBusy_ = "init";

    emptyImg_ = QImage(1,1,QImage::Format_ARGB32);
    emptyImg_.setPixel(0,0,qRgba(0,0,0,0));

    LH_Qt_QString* hr = new LH_Qt_QString(this,tr("~Mon-Data-Rule"),"<hr>", LH_FLAG_NOSAVE,lh_type_string_html );
    hr->setOrder(-5);

    setup_available_contitions_ = new LH_Qt_QStringList(this,"Condition",QStringList());
    setup_available_contitions_->setHelp( "<p>To define an image to display for a certain value, first select the desired value from this list (the list only contains values seen recently, if you know the value and wish to add it manually, tick the advanced editing box).</p>"
                                          "<p>Once you have selected an item you can customize the image associated with this value via the &quot;image&quot; field.</p>"
                                          "<p>You may then change the selected value and define other images. Returning to a value with a defined image will allow you to change a previously defined image for another one.</p>");
    setup_available_contitions_->setOrder(-5);
    connect( setup_available_contitions_, SIGNAL(changed()), this, SLOT(changeSelectedConditions()) );

    setup_condition_image_ = new LH_Qt_QFileInfo(this,"Image",QFileInfo());
    setup_condition_image_->setHelp( "<p>Select the image used when the selected condition is active.</p>");
    setup_condition_image_->setOrder(-4);
    connect( setup_condition_image_, SIGNAL(changed()), this, SLOT(updateConditionImage()) );

    setup_advanced_editing_ = new LH_Qt_bool(this,"Advanced Editing",false);
    setup_advanced_editing_->setHelp( "<p>This switches between the simple, user friendly interface for one that allows direct editing of the conditions & images.</p>");
    setup_advanced_editing_->setOrder(-2);
    connect( setup_advanced_editing_, SIGNAL(changed()), this, SLOT(updateAdvancedEditing()) );

    setup_conditions_list_ = new LH_Qt_QTextEdit(this,"Conditions","",LH_FLAG_HIDDEN);
    setup_conditions_list_->setHelp( "<p>This list displays all currently specified conditions and the images that are associated with them.</p>");
    setup_conditions_list_->setOrder(100);
    connect( setup_conditions_list_, SIGNAL(changed()), this, SLOT(updateConditions()) );

    setup_text_ = new LH_Qt_QString( this, tr("~"), QString(), LH_FLAG_READONLY|LH_FLAG_NOSAVE|LH_FLAG_HIDDEN|LH_FLAG_AUTORENDER );
    setup_text_->setOrder(-1);
    conditionsListBusy_ = "";

    LH_Qt_QString* hr2 =new LH_Qt_QString(this,tr("~Mon-Props-Rule"),"<hr>", LH_FLAG_NOSAVE,lh_type_string_html );
    hr2->setOrder(-1);
}


LH_MonitoringConditionImage::~LH_MonitoringConditionImage()
{

}

int LH_MonitoringConditionImage::polling()
{
    updateImage();
    return 200;
}

int LH_MonitoringConditionImage::width( void*obj,int h )
{
    Q_UNUSED(obj);
    Q_UNUSED(h);
    return -1;
}

int LH_MonitoringConditionImage::height( void*obj,int h )
{
    Q_UNUSED(obj);
    Q_UNUSED(h);
    return -1;
}

QImage *LH_MonitoringConditionImage::render_qimage(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);

    if( image_ ) delete image_;

    QFileInfo imageFile = QFileInfo( getImageName() );
    if(imageFile.isFile())
        image_ = new QImage( imageFile.absoluteFilePath() );
    else
        image_ = new QImage( emptyImg_ );
    return image_;
}

QString LH_MonitoringConditionImage::getImageName()
{
    if(conditionsListBusy_!="") return "";
    conditionsListBusy_ = "getName";

    QString imageName;
    if (conditions_.contains(conditionValue_))
        imageName = state()->dir_layout + conditions_.value(conditionValue_).image;
    else if(data_)
    {
        conditionData condition;
        condition.listIndex = -1;
        condition.code = conditionValue_;
        condition.image = "";
        conditions_.insert(condition.code, condition);

        //if(setup_available_contitions_->list().count()!=0)
        {
            setup_available_contitions_->list().append(condition.code);
            setup_available_contitions_->refreshList();
        }

        imageName = "";
    }
    conditionsListBusy_ = "";
    return imageName;
}


void LH_MonitoringConditionImage::updateConditions()
{
    //if(conditionsListBusy_!="") return;
    conditionsListBusy_ = "updateConds";


    conditions_.clear();
    setup_available_contitions_->list().clear();

    QStringList conditionsList = setup_conditions_list_->value().split('\n',QString::SkipEmptyParts);
    QRegExp re = QRegExp(";.*$");
    for(int i = 0 ; i<conditionsList.count(); i++)
    {
        QString conditionItem = conditionsList[i].remove(re).trimmed();
        if (conditionItem!="")
        {
            QStringList conditionParts = conditionItem.split('\t',QString::SkipEmptyParts);
            if (conditionParts.count()>=2)
            {
                conditionData condition;
                condition.listIndex = i;
                condition.code = conditionParts.at(0);
                condition.image = conditionParts.at(1);
                if (condition.image.startsWith(state()->dir_layout))
                    condition.image.remove(state()->dir_layout);
                conditions_.insert(condition.code, condition);

                setup_available_contitions_->list().append(condition.code);
            }
        }
    }
    setup_available_contitions_->refreshList();
    updateImage(true);
    conditionsListBusy_ = "";
}



void LH_MonitoringConditionImage::changeType()
{
    //do we need to do anything here??
}


void LH_MonitoringConditionImage::updateImage(bool rerender)
{
    setup_text_->setFlag(LH_FLAG_HIDDEN,false);
    setup_text_->setValue("Accessing memory..." );

    QString newValue = data_->getText(false);
    if(conditionValue_!=newValue)
    {
        conditionValue_ = newValue;
        rerender = true;
    }
    setup_text_->setValue(QString("Value %1 received; resolved to image: %3").arg(newValue, getImageName()));
    if (rerender) callback(lh_cb_render,NULL);
}


void LH_MonitoringConditionImage::connect_changeType(QObject* obj)
{
    connect( obj, SIGNAL(changed()), this, SLOT(changeType()) );
}

void LH_MonitoringConditionImage::connect_updateImage(QObject* obj)
{
    connect( obj, SIGNAL(changed()), this, SLOT(updateImage()) );
}

void LH_MonitoringConditionImage::changeSelectedConditions()
{
    QFileInfo imgFile;
    if (setup_available_contitions_)
        if (setup_available_contitions_->value() != -1)
            if (setup_available_contitions_->list().count() != 0)
            {
                QString selVal = setup_available_contitions_->list().at(setup_available_contitions_->value());
                if (conditions_.contains(selVal))
                {
                    QString filePath = conditions_.value(selVal).image;
                    if (filePath!="") filePath = state()->dir_layout + filePath;
                    imgFile.setFile(filePath);
                }
            }
    setup_condition_image_->setValue(imgFile);
}

void LH_MonitoringConditionImage::updateConditionImage()
{
    if (setup_available_contitions_)
        if (setup_available_contitions_->value() != -1)
            if (setup_available_contitions_->list().count() != 0)
            {
                QString selVal = setup_available_contitions_->list()[setup_available_contitions_->value()];
                if (conditions_.contains(selVal))
                {
                    conditions_.operator [](selVal).image = setup_condition_image_->value().filePath();
                    if (conditions_.operator [](selVal).image.startsWith(state()->dir_layout))
                        conditions_.operator [](selVal).image.remove(state()->dir_layout);
                    updateConditionsList();
                    callback(lh_cb_render,NULL);
                }
            }
}

void LH_MonitoringConditionImage::updateConditionsList()
{
    QStringList newList;
    foreach(QString key, conditions_.keys())
        if(conditions_.value(key).image != "")
            newList.append(conditions_.value(key).code + "\t" + conditions_.value(key).image);
    setup_conditions_list_->setValue(newList.join("\n"));
}

void LH_MonitoringConditionImage::updateAdvancedEditing()
{
    setup_available_contitions_->setFlag(LH_FLAG_HIDDEN,setup_advanced_editing_->value());
    setup_condition_image_->setFlag(LH_FLAG_HIDDEN,setup_advanced_editing_->value());
    setup_conditions_list_->setFlag(LH_FLAG_HIDDEN,!setup_advanced_editing_->value());
}

