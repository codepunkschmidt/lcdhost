/**
  \file     LH_Dial.cpp
  \author   Andy Bridges <andy@bridgesuk.com>
  \legalese Copyright (c) 2011 Andy Bridges

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
#include <QtCore/qmath.h>
#include <QLinearGradient>
#include <QPainter>

#include "LH_Dial.h"

static inline uint PREMUL(uint x)
{
    uint a = x >> 24;
    uint t = (x & 0xff00ff) * a;
    t = (t + ((t >> 8) & 0xff00ff) + 0x800080) >> 8;
    t &= 0xff00ff;

    x = ((x >> 8) & 0xff) * a;
    x = (x + ((x >> 8) & 0xff) + 0x80);
    x &= 0xff00;
    x |= t | (a << 24);
    return x;
}

LH_Dial::~LH_Dial()
{
    min(0);
    max(0);
    polling_on_ = false;
    isClock = false;

    faceImage_ = new QImage();

    for(int i = 0; i<needleImage_.count(); i++)
        delete needleImage_[i];
    useLinkedValueAverage_ = false;
    linkedValueMultiplier_ = 1;

    ticks.fullCircle.append(tickObject(20, 1, 0.05, 0.90));
    ticks.fullCircle.append(tickObject(10, 2, 0.15, 0.80));
    ticks.semiCircle.append(tickObject(21, 1, 0.05, 0.90));
    ticks.semiCircle.append(tickObject(11, 2, 0.15, 0.80));
    ticks.quarterCircle.append(tickObject(11, 1, 0.05, 0.90));
    ticks.quarterCircle.append(tickObject(3, 2, 0.15, 0.80));
}

const char *LH_Dial::userInit()
{
    if( const char *err = LH_QtInstance::userInit() ) return err;

    setup_type_ = new LH_Qt_QStringList(this, "Dial Type", QStringList()<<"Full Circle"<<"Semi-Circle"<<"Quarter Circle", LH_FLAG_AUTORENDER);
    setup_type_->setHelp( "<p>The dial's shape.</p>");

    setup_orientation_ = new LH_Qt_QStringList(this,"Orientation",QStringList()<<"N/A"<<"Left"<<"Top"<<"Right"<<"Bottom"<<"Bottom Left"<<"Top Left"<<"Top Right"<<"Bottom Right", LH_FLAG_AUTORENDER | LH_FLAG_HIDDEN);
    setup_orientation_->setHelp( "<p>The orientation of the dial (does not apply to circular dials).</p>");

    setup_bgcolor_ = new LH_Qt_QColor(this,"Background color",Qt::transparent,LH_FLAG_AUTORENDER);
    setup_bgcolor_->setHelp( "<p>The color for the background (i.e. <i>behind</i> the face).</p>");

    setup_face_style_ = new LH_Qt_QStringList(this, "Face Style",QStringList()<<"None"<<"Line"<<"Gradient Fill"<<"Image", LH_FLAG_AUTORENDER);
    setup_face_style_->setHelp( "<p>How the face of the dial should be drawn - if at all.</p>");

    setup_face_pencolor_ = new LH_Qt_QColor(this,"Pen color",Qt::black,LH_FLAG_AUTORENDER);
    setup_face_pencolor_->setHelp( "<p>The colour used for any lines on the face (i.e. outline & ticks).</p>");

    setup_face_fillcolor1_ = new LH_Qt_QColor(this,"Fill color (start)",Qt::white,LH_FLAG_AUTORENDER);
    setup_face_fillcolor1_->setHelp( "<p>The color used to fill dial's face at the top</p>");

    setup_face_fillcolor2_ = new LH_Qt_QColor(this,"Fill color (end)",Qt::lightGray,LH_FLAG_AUTORENDER);
    setup_face_fillcolor2_->setHelp( "<p>The color used to fill dial's face at the bottom</p>");

    setup_face_image_ = new LH_Qt_QFileInfo( this, ("Face Image"), QFileInfo(), LH_FLAG_AUTORENDER | LH_FLAG_HIDDEN);
    setup_face_image_->setHelp( "<p>Image file to load and use as the dial's face</p>");

    setup_face_ticks_ = new LH_Qt_bool(this,"Show Ticks",false,LH_FLAG_AUTORENDER);
    setup_face_ticks_->setHelp( "<p>Whether to overlay marks denoting significant points along the dial.</p>");

    connect( setup_face_style_, SIGNAL(changed()), this, SLOT(changeFaceStyle()));

    setup_max_ = new LH_Qt_double(this, "Dial Max",0,-99999999,99999999, LH_FLAG_AUTORENDER | LH_FLAG_HIDDEN);
    setup_min_ = new LH_Qt_double(this, "Dial Min",1000,-99999999,99999999, LH_FLAG_AUTORENDER | LH_FLAG_HIDDEN);
    setup_linked_values_ = new LH_Qt_array_double(this,"Linked Value",0,LH_FLAG_NOSAVE);

    setup_needles_reverse_ = new LH_Qt_bool(this, "Reverse Needles", false, LH_FLAG_AUTORENDER);
    setup_needles_reverse_->setHelp( "<p>By default all needles move clockwise; this setting changes that to anti-clockwise.</p>"
                                     "<p>This makes particular sense for some dial orientations, e.g. half-cicle dials in the \"top\" orientation default to having 0% on the right and 100% on the left. Applying this setting reverses that, which for this example will look more natural.</p>");

    setup_needles_smooth_ = new LH_Qt_bool(this, "Smooth Needles", true, LH_FLAG_AUTORENDER);
    setup_needles_smooth_->setHelp( "<p>This setting makes needles move more slowly by rendering needle movement in 8 gradual steps instead of a single step. This looks much nicer but causes a small increase in CPU usage.</p>");

    setup_needle_selection_ = new LH_Qt_QStringList(this,"Selected Needle",QStringList(),LH_FLAG_NOSAVE);
    setup_needle_selection_->setHelp( "<p>Select a needle here and configure it below. Seperate settings are stored for each needle.</p>");

    setup_needle_style_ = new LH_Qt_array_string_ui(this, "Needle Style", QStringList(),
                                                    LH_FLAG_AUTORENDER|LH_FLAG_NOSAVE, lh_type_string_list,
                                                    QStringList()<<"Line"<<"Image [Needle Only]"<<"Image [Full Face] (Full Circle Only)"
                                                    );
    setup_needle_style_->setHelp( "<p>How the selected needle should be drawn.</p>"
                                  "<p>Needle images can be created in one of two ways:<ul>"
                                  "<li>\"Needle Only\": "
                                  "<br/>This type of image is only as wide and as long as the needle. The image will be stretched or compressed as required to match the needle's length; the width will be scaled so as to preserve the aspect ratio. "
                                  "<br/>The needle is assumed to be in the vertical position and will be rotated around the image's <i>base</i> as required.</li>"
                                  "<li>\"Full Face\": "
                                  "<br/>This type of image is the size of the entire dial. The image will be stretched or compressed as required to completely fill the dial (and may therefore not preserve the aspect ratio). "
                                  "<br/>The needle is assumed to be in the vertical position and will be rotated around the image's <i>center point</i> as required.</li>"
                                  "</ul></p>");

    setup_needle_color_ = new LH_Qt_array_int_ui(this,"Needle Color",0,LH_FLAG_AUTORENDER|LH_FLAG_NOSAVE, lh_type_integer_color);
    setup_needle_color_->setHelp( "<p>The colour used do draw \"Line\" needles.</p>");

    setup_needle_thickness_ = new LH_Qt_array_int_ui(this,"Needle Thickness",0,1,20,LH_FLAG_AUTORENDER|LH_FLAG_NOSAVE|LH_FLAG_NOSAVE);
    setup_needle_thickness_->setHelp( "<p>The colour used do draw \"Line\" needles.</p>");

    setup_needle_length_ = new LH_Qt_array_int_ui(this,"Needle Length (%)",0,0,100,LH_FLAG_AUTORENDER|LH_FLAG_NOSAVE|LH_FLAG_NOSAVE);
    setup_needle_length_->setHelp( "<p>The needle's length as a percentage of the dial's radius.</p>");

    setup_needle_gap_ = new LH_Qt_array_int_ui(this,"Needle Gap (%)",0,0,100,LH_FLAG_AUTORENDER|LH_FLAG_NOSAVE);
    setup_needle_gap_->setHelp( "<p>The gap between the center of the dial and the needle's start as a percentage of the dial's radius.</p>");

    setup_needle_image_ = new LH_Qt_array_string_ui( this, "Needle Image", QStringList(),
                                                     LH_FLAG_AUTORENDER | LH_FLAG_HIDDEN |LH_FLAG_NOSAVE,
                                                     lh_type_string_filename);
    setup_needle_image_->setHelp( "<p>Image file to load and use for this needle (see \"Needle Style\" for more information about how the image will be used).</p>");
    connect( setup_needle_style_, SIGNAL(changed()), this, SLOT(changeNeedleStyle()));

    addNeedle("Default");
    connect( setup_linked_values_, SIGNAL(changed()), this, SLOT(newLinkedValue()) );
    connect( setup_needle_selection_, SIGNAL(changed()), this, SLOT(changeSelectedNeedle()) );
    connect( this, SIGNAL(initialized()), this, SLOT(initializeDefaults()) );

    return 0;
}

void LH_Dial::initializeDefaults()
{
    changeType(true);
    changeSelectedNeedle();
    changeFaceStyle();
    changeNeedleStyle();
    connect( setup_type_, SIGNAL(changed()), this, SLOT(changeType()));
}

void LH_Dial::addNeedle(QString name)
{
    setup_needle_selection_->list().append(name);
    setup_needle_selection_->refreshList();
    needle_pos_.append(0);
    needle_val_.append(0);
    needle_step_.append(1);
    needle_vis_.append(true);
    needleCode_.append("");
    needleImage_.append( new QImage() );

    setup_needle_selection_->setFlag(LH_FLAG_HIDDEN, setup_needle_selection_->list().count()==1);
    setup_needle_selection_->setValue(0);
    syncNeedleDataArrays();
}

void LH_Dial::clearNeedles()
{
    setup_needle_selection_->list().clear();
    setup_needle_selection_->refreshList();
    needle_pos_.clear();
    needle_val_.clear();
    needle_step_.clear();
    needleCode_.clear();
    needle_vis_.clear();
    for(int i = 0; i<needleImage_.count(); i++)
        delete needleImage_[i];
    needleImage_.clear();
    setup_needle_selection_->setFlag(LH_FLAG_HIDDEN, false);
}

int LH_Dial::needleCount()
{
    return setup_needle_selection_->list().count();
}

void LH_Dial::setNeedleCount(int count)
{
    if(count <= 0)
        return;
    if(count == setup_needle_selection_->list().count())
        return;
    QStringList names;
    for(int i = 0; i<count;)
        names.append(QString("Needle #%1").arg(++i));
    setNeedles(names);
}

double LH_Dial::max()
{
    if (setup_min_->value() < setup_max_->value())
        return setup_max_->value();
    else
        return setup_min_->value()+1;
}
double LH_Dial::max(double val)
{
    setup_max_->setValue(val);
    return max();
}

double LH_Dial::min()
{
    return setup_min_->value();
}
double LH_Dial::min(double val)
{
    setup_min_->setValue(val);
    return min();
}

bool LH_Dial::setMin( qreal r )
{
    if( min() == r ) return false;
    min(r);
    return true;
}

bool LH_Dial::setMax( qreal r )
{
    if( max() == r ) return false;
    max(r);
    return true;
}

QString LH_Dial::colString(QColor col)
{
    QString str = "#";
    str += QString::number(col.alpha() , 16);
    str += QString::number(col.red() , 16);
    str += QString::number(col.green() , 16);
    str += QString::number(col.blue() , 16);
    return str;
}

QImage LH_Dial::getFace()
{
    QString faceCode = "";
    int w = image_->width();
    int h = image_->height();

    switch(setup_face_style_->index())
    {
    case 0: // None
        break;
    case 2: // Gradient Fill
        faceCode = QString("%1;%2;").arg(colString(setup_face_fillcolor1_->value())).arg(colString(setup_face_fillcolor2_->value()));
    case 1: // Line
        faceCode = QString("%1,%2;%3,%4;").arg(w).arg(h).arg(maxDegrees()).arg(startDegrees()) + faceCode;
        faceCode += QString("%1;%2;%3").arg(colString(setup_bgcolor_->value())).arg(colString(setup_face_pencolor_->value())).arg(setup_face_ticks_->value());
        break;
    case 3: //Image
        faceCode = QString("%1,%2;%3").arg(w).arg(h).arg(setup_face_image_->value().absoluteFilePath());
        break;
    }
    faceCode = QString::number(setup_face_style_->index()) + ":" + faceCode;


    if (faceCode_ != faceCode)
    {
        delete faceImage_;
        faceImage_ = new QImage(w,h,QImage::Format_ARGB32_Premultiplied);
        faceImage_->fill( PREMUL( setup_bgcolor_->value().rgba() ) );

        if (setup_face_style_->index()!=0)
        {
            QPainter painter;
            if( painter.begin( faceImage_ ) )
            {
                if(setup_face_style_->index()==3)//image
                {
                    if(setup_face_image_->value().isFile())
                    {
                        QImage face_img = QImage(setup_face_image_->value().absoluteFilePath());
                        painter.drawImage(QRectF( 0, 0, w, h ), face_img);
                    }
                } else {
                    QPen pen = QPen(setup_face_pencolor_->value());
                    if (setup_face_style_->index()==2)
                    {
                        QLinearGradient gradient;
                        gradient.setStart( QPointF(0, 0) );
                        gradient.setFinalStop( QPointF(0,h) );
                        gradient.setColorAt(0,setup_face_fillcolor1_->value());
                        gradient.setColorAt(1,setup_face_fillcolor2_->value());
                        painter.setBrush(QBrush(gradient));
                    } else
                        painter.setBrush(QBrush(Qt::transparent));

                    int x = 0;
                    int y = 0;
                    int xm = 1;
                    int ym = 1;
                    int xo = 2;
                    int yo = 2;
                    painter.setPen(pen);
                    switch(setup_type_->index())
                    {
                    case 0: //full circle
                        painter.drawEllipse(0,0,w-1,h-1);
                        break;
                    case 1: //semi-circle
                        switch(setup_orientation_->index())
                        {
                        case 0: //left / bottom left
                            x = -1;
                            xm = 2;
                            break;
                        case 1: //top / top left
                            y = -1;
                            ym = 2;
                            break;
                        case 2: //right / top right
                            xm = 2;
                            xo = 4;
                            break;
                        case 3: //bottom / bottom right
                            ym = 2;
                            yo = 4;
                            break;
                        }
                        painter.drawPie(x*w+1, y*h+1, w*xm-xo, h*ym-yo, startDegrees()*-16 + 90*-16,maxDegrees()*16);
                        break;
                    case 2: //quarter circle
                        xm = 2;
                        ym = 2;
                        switch(setup_orientation_->index())
                        {
                        case 0: //left / bottom left
                            x = -1;
                            yo = 4;
                            break;
                        case 1: //top / top left
                            x = -1;
                            y = -1;
                            break;
                        case 2: //right / top right
                            y = -1;
                            xo = 4;
                            break;
                        case 3: //bottom / bottom right
                            xo = 4;
                            yo = 4;
                            break;
                        }
                        painter.drawPie(x*w+1, y*h+1, w*xm-xo, h*ym-yo, startDegrees()*-16,maxDegrees()*16);
                        break;
                    }

                    if(setup_face_ticks_->value())
                    {
                        QList<tickObject> tickdef;
                        switch(setup_type_->index())
                        {
                        case 0:
                            tickdef = ticks.fullCircle;
                            break;
                        case 1:
                            tickdef = ticks.semiCircle;
                            break;
                        case 2:
                            tickdef = ticks.quarterCircle;
                            break;
                        }

                        foreach(tickObject tick, tickdef)
                        {
                            int m = (setup_type_->index() == 0? 1 : 0); // fix for full circle having an overlapping last/first tick
                            for(int i=0; i<tick.count+m; i++)
                            {
                                QPen pen = QPen(tick.color);
                                pen.setWidth(tick.width);
                                paintLine(painter, pen, startDegrees(), maxDegrees() / (tick.count+m-1) * i, tick.length, tick.gap );
                            }
                        }
                    }

                }

                painter.end();
            }
        }

        faceCode_ = faceCode;
    }
    return *faceImage_;
}

QImage LH_Dial::getNeedle(int needleID, qreal degrees, int& needleStyle)
{
    needleStyle = setup_needle_style_->indexAt(needleID);
    QColor needleColor = QColor::fromRgba(setup_needle_color_->at(needleID,0));
    int needleThick = setup_needle_thickness_->at(needleID,3);
    int needleLength = setup_needle_length_->at(needleID,90);
    int needleGap = setup_needle_gap_->at(needleID,0);
    QString needleImagePath = setup_needle_image_->at(needleID,"");

    QString needleCode = "";
    int h = image_->height();
    int w = image_->width();

    int relH = h/2;
    int relW = w/2;
    float radiansM = 0;

    switch(setup_type_->index())
    {
    case 0: //full circle
        break;
    case 1: //semi-circle
        switch(setup_orientation_->index())
        {
        case 0: //left / bottom left
            relW = w;
            break;
        case 1: //top / top left
            relH = h;
            radiansM = M_PI/2;
            break;
        case 2: //right / top right
            relW = w;
            radiansM = M_PI;
            break;
        case 3: //bottom / bottom right
            relH = h;
            radiansM = -M_PI/2;
            break;
        }
        break;
    case 2: //quarter circle            
        relW = (setup_needles_reverse_->value()? h : w);
        relH = (setup_needles_reverse_->value()? w : h);
        break;
    }

    float radians = degrees * M_PI/180;
    radiansM += radians;
    float drawLen = qSqrt( qPow(relH,2) * qPow(relW,2) / ( qPow(relH * qSin(radiansM),2) + qPow(relW * qCos(radiansM),2) ) );

    int drawWid = 2;

    switch(needleStyle)
    {
    case 0: // Line
        needleCode = QString("%1;%2;%3;%4;%5").arg(drawLen).arg(colString(needleColor)).arg(needleThick).arg(needleLength).arg(needleGap);
        break;
    case 1: // Image (needle only)
    case 2: // Image (full face)
        needleCode = QString("%1;%2;%3;%4").arg(drawLen).arg(needleImagePath).arg(h).arg(w);
        break;
    }
    needleCode = QString::number(needleStyle) + ":" + needleCode;

    QPainter painter;
    if (needleCode_[needleID] != needleCode)
    {
        delete needleImage_[needleID];

        QFileInfo f(needleImagePath);
        if(needleStyle == 1 && f.isFile())
        {
            QImage needle_img(f.absoluteFilePath());
            drawWid = qCeil(((float)(needle_img.width() * drawLen)) / needle_img.height());

            needleImage_[needleID] = new QImage(drawWid,drawLen,QImage::Format_ARGB32_Premultiplied);
            needleImage_[needleID]->fill( PREMUL( QColor(Qt::transparent).rgba() ) );

            if( painter.begin( needleImage_[needleID] ) )
            {
                painter.drawImage(QRectF( 0, 0, drawWid, drawLen ), needle_img);
                painter.end();
            }
        }else
        if(needleStyle == 2 && f.isFile())
        {
            float drawWid = qSqrt( qPow(relH,2) * qPow(relW,2) / ( qPow(relW * qSin(radians),2) + qPow(relH * qCos(radians),2) ) );
            QImage needle_img(f.absoluteFilePath());

            needleImage_[needleID] = new QImage(drawWid*2,drawLen*2,QImage::Format_ARGB32_Premultiplied);
            needleImage_[needleID]->fill( PREMUL( QColor(Qt::transparent).rgba() ) );

            if( painter.begin( needleImage_[needleID] ) )
            {
                painter.drawImage(QRectF( 0, 0, drawWid*2, drawLen*2 ), needle_img);
                painter.end();
            }
        }
        else
        {
            drawWid = needleThick;
            needleImage_[needleID] = new QImage(drawWid,drawLen,QImage::Format_ARGB32_Premultiplied);
            needleImage_[needleID]->fill( PREMUL( QColor(Qt::transparent).rgba() ) );

            if( painter.begin( needleImage_[needleID] ) )
            {
                int y =  drawLen * (1.0 - (needleLength + needleGap)/100.0);
                painter.fillRect(0,y,drawWid, drawLen * needleLength/100.0, needleColor);
                painter.end();
            }
        }

        needleCode_[needleID] = needleCode;
    }
    return *needleImage_[needleID];
}

void LH_Dial::getCenter(QPointF& center)
{
    float horzSize;
    float vertSize;
    getCenter(center, horzSize, vertSize);
}

void LH_Dial::getCenter(QPointF& center, float& horzSize, float& vertSize)
{
    float w = image_->width();
    float h = image_->height();

    //h*0.9, w*0.9
    center.setX(w/2-.5) ;
    center.setY(h/2-.5) ;
    horzSize = w/2;
    vertSize = h/2;

    switch(setup_type_->index())
    {
    case 0: //full circle
        break;
    case 1: //semi-circle
        switch(setup_orientation_->index())
        {
        case 0: //left / bottom left
            center.setX( 0 );
            horzSize = w;
            break;
        case 1: //top / top left
            center.setY( 0 );
            vertSize = h;
            break;
        case 2: //right / top right
            center.setX( w );
            horzSize = w;
            break;
        case 3: //bottom / bottom right
            center.setY( h );
            vertSize = h;
            break;
        }
        break;
    case 2: //quarter circle
        horzSize = w;
        vertSize = h;
        switch(setup_orientation_->index())
        {
        case 0: //left / bottom left
            center.setX( 0 );
            center.setY( h );
            break;
        case 1: //top / top left
            center.setX( 0 );
            center.setY( 0 );
            break;
        case 2: //right / top right
            center.setX( w );
            center.setY( 0 );
            break;
        case 3: //bottom / bottom right
            center.setX( w );
            center.setY( h );
            break;
        }
        break;
    }
}

void LH_Dial::getRotationData(qreal startAngle, qreal angle, float& centerX, float& centerY, float& horzSize, float& vertSize, float& radians)
{
    radians = (180+(angle+startAngle)) * M_PI/180;
    QPointF center;
    getCenter(center, horzSize, vertSize);
    centerX = center.x();
    centerY = center.y();
}

void LH_Dial::paintLine(QPainter& painter, QPen& pen, qreal startAngle, qreal angle, qreal relLength, qreal gap)
{
    float x1; float y1; float horzSize; float vertSize; float radians;
    getRotationData(startAngle, angle, x1, y1, horzSize, vertSize, radians);

    int x2 = (qSin(radians) * (gap+relLength) * horzSize) + x1;
    int y2 = (qCos(radians) * (gap+relLength) * vertSize) + y1;

    x1 = (qSin(radians) * gap * horzSize) + x1;
    y1 = (qCos(radians) * gap * vertSize) + y1;

    painter.setPen(pen);
    painter.drawLine(x1,y1,x2,y2);
}

void LH_Dial::paintImage(QPainter& painter, QImage needleImage, qreal startAngle, qreal angle, bool reverseOffsetting)
{
    float x1; float y1; float horzSize; float vertSize; float radians;

    //int m = (setup_needles_reverse_->value() ? -1 : 1);
    getRotationData(startAngle, angle, x1, y1, horzSize, vertSize, radians);

    float A = (x1!=0? qAtan(y1/x1) : M_PI/2);
    float Hyp = qSqrt( qPow(y1,2) + qPow(x1,2) );

    float x2 = Hyp * qCos(A - (radians-M_PI));
    float y2 = Hyp * qSin(A - (radians-M_PI));

    x2-=(needleImage.width()/2.0);
    y2-=(needleImage.height());

    painter.save();

    painter.rotate(startAngle + angle);

    float drawLen = qSqrt( qPow(y1,2) * qPow(x1,2) / ( qPow(y1 * qSin(radians),2) + qPow(x1 * qCos(radians),2) ) );
    if(reverseOffsetting) y2+=drawLen;

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.drawImage(QPointF(x2,y2),needleImage);

    painter.restore();
}

bool LH_Dial::setVal(qreal value, int i, bool repoll )
{
    if(i >= needle_val_.count()) return repoll;

    if(value < min()) value = min();
    if(value > max()) value = max();
    if(needle_val_[i] != value)
    {
        needle_val_[i] = value;
        if(isClock && (needle_pos_[i] != max() && needle_pos_[i] != min()) && (needle_val_[i] == min() || needle_val_[i] == max()) ) {
            needle_pos_[i] -= max();
            needle_val_[i] = min();
        }
        needle_step_[i] = (needle_val_[i] - needle_pos_[i])/8;
        // minimum step is 0.5 degrees, otherwise rendering is very inefficient
        if(needle_step_[i]!=0)
            if(qAbs(needle_step_[i])<(max()-min())/360/2)
                needle_step_[i] = (max()-min())/360/2 * (qAbs(needle_step_[i]) / needle_step_[i]);
        repoll = true;
    }
    if(!polling_on_ && repoll && i==needleCount()-1)
        callback(lh_cb_polling, NULL);
    return repoll;
}

float LH_Dial::maxDegrees()
{
    switch(setup_type_->index())
    {
    case 0: //full circle
        return 360;
        break;
    case 1: //semi-circle
        return 180;
        break;
    case 2: //quarter circle
        return 90;
        break;
    }
    return 360;
}

float LH_Dial::startDegrees()
{
    switch(setup_orientation_->index())
    {
    case 0: //left / bottom left
        return 0;
        break;
    case 1: //top / top left
        return 90;
        break;
    case 2: //right / top right
        return 180;
        break;
    case 3: //bottom / bottom right
        return 270;
        break;
    }
    return 0;
}

void LH_Dial::setNeedleVisibility(bool visible, int index)
{
    if(index>=0 && index<needle_vis_.count())
        needle_vis_[index] = visible;
}

void LH_Dial::drawDial()
{
    if( image_ == NULL || image_->isNull() ) return;

    QPainter painter;
    if( painter.begin( image_ ) )
    {
        //Draw Face
        painter.drawImage(0,0, getFace() );

        //Draw Needle(s)
        for( int i=0; i<needleCount(); ++i )
        {
            if(!needle_vis_[i]) continue;

            if(!setup_needles_smooth_->value())
                needle_pos_[i] = needle_val_[i];
            else
            {
                needle_pos_[i] += needle_step_[i];
                if( ((needle_step_[i]<0) && (needle_pos_[i]<needle_val_[i])) ||
                    ((needle_step_[i]>0) && (needle_pos_[i]>needle_val_[i])) )
                {
                    needle_pos_[i] = needle_val_[i];
                    needle_step_[i] = 0;
                }
            }
            qreal angle = maxDegrees() * (needle_pos_[i]-min()) / (max()-min());
            int needleStyle;
            QImage needleImage = getNeedle(i, angle, needleStyle);
            if( !setup_needles_reverse_->value() )
                paintImage(painter, needleImage, startDegrees(), angle, (needleStyle==2));
            else
                paintImage(painter, needleImage, startDegrees()+maxDegrees(), -1*angle, (needleStyle==2));
        }

        painter.end();
    }
}

int LH_Dial::polling()
{
    callback(lh_cb_render, NULL);
    if(setup_needles_smooth_->value())
        for( int i=0; i<needleCount(); i++ )
            if (qAbs(needle_pos_[i]-needle_val_[i])>qAbs(needle_step_[i])) {
                polling_on_ = true;
                return 50;
            }
    polling_on_ = false;
    return 0;
}

int LH_Dial::notify(int n, void *p)
{
    Q_UNUSED(p);
    if(setup_linked_values_->link()==NULL)
        return 0;
    else
    {
        if(n&LH_NOTE_SECOND)
        {
            double totalTotal = 0;
            if(useLinkedValueAverage_)
                setNeedleCount(1);
            else
                setNeedleCount(linkedValues.count());
            for(int n=0; n<linkedValues.count(); n++)
            {
                double total = 0;
                foreach(double d, linkedValues[n])
                    total += d;
                if(linkedValues[n].count()!=0)
                    total/=linkedValues[n].count();
                if(!useLinkedValueAverage_)
                    setVal(total, n);
                else
                    totalTotal += total;
                linkedValues[n].clear();
            }
            if(linkedValues.count()!=0)
                totalTotal/=linkedValues.count();
            if(useLinkedValueAverage_)
                setVal(totalTotal);
            requestRender();
        }
        return LH_NOTE_SECOND;
    }
}

QImage *LH_Dial::render_qimage( int w, int h )
{
    if( LH_QtInstance::initImage(w,h) == NULL ) return NULL;
    image_->fill( PREMUL( setup_bgcolor_->value().rgba() ) );
    drawDial();
    return image_;
}

void LH_Dial::changeType(bool preserveOrientation)
{
    setup_orientation_->setFlag(LH_FLAG_HIDDEN, setup_type_->index()==0 );
    QString val = setup_orientation_->value();
    setup_orientation_->list().clear();
    switch(setup_type_->index())
    {
    case 0: //full circle
        setup_orientation_->list().append("N/A");
        break;
    case 1: //semi-circle
        setup_orientation_->list().append("Left");
        setup_orientation_->list().append("Top");
        setup_orientation_->list().append("Right");
        setup_orientation_->list().append("Bottom");
        break;
    case 2: //full circle
        setup_orientation_->list().append("Bottom Left");
        setup_orientation_->list().append("Top Left");
        setup_orientation_->list().append("Top Right");
        setup_orientation_->list().append("Bottom Right");
        break;
    }
    setup_orientation_->refreshList();
    //qDebug() << "changeType: " << ident() << ": " << setup_type_->value() << " pres. orient.:" << preserveOrientation << " : " << val;
    //if(preserveOrientation)
        setup_orientation_->setValue(val);
    //else
    //    setup_orientation_->setIndex(0);
}

void LH_Dial::changeFaceStyle()
{
    setup_face_pencolor_->setFlag(LH_FLAG_HIDDEN, setup_face_style_->index()==0 || setup_face_style_->index()==3);
    setup_face_fillcolor1_->setFlag(LH_FLAG_HIDDEN, setup_face_style_->index()!=2);
    setup_face_fillcolor2_->setFlag(LH_FLAG_HIDDEN, setup_face_style_->index()!=2);
    setup_face_ticks_->setFlag(LH_FLAG_HIDDEN, setup_face_style_->index()==0 || setup_face_style_->index()==3);
    setup_face_image_->setFlag(LH_FLAG_HIDDEN, setup_face_style_->index()!=3);
}

void LH_Dial::changeNeedleStyle()
{
    if(setup_needle_style_->currentIndex()==2 && setup_type_->index()!=0)
        setup_needle_style_->setCurrentIndex(1);
    int needleID = setup_needle_style_->currentIndex();
    setup_needle_color_->setFlag(LH_FLAG_HIDDEN, needleID!=0);
    setup_needle_thickness_->setFlag(LH_FLAG_HIDDEN, needleID!=0);
    setup_needle_length_->setFlag(LH_FLAG_HIDDEN, needleID!=0);
    setup_needle_gap_->setFlag(LH_FLAG_HIDDEN, needleID!=0);
    setup_needle_image_->setFlag(LH_FLAG_HIDDEN, needleID==0);
}

void LH_Dial::changeSelectedNeedle()
{
    if (setup_needle_selection_->index() >= needleCount()) setup_needle_selection_->setValue(needleCount()-1);
    if (setup_needle_selection_->index() < 0) setup_needle_selection_->setValue(0);

    setup_needle_style_->setEditIndex(setup_needle_selection_->index());
    setup_needle_color_->setEditIndex(setup_needle_selection_->index());
    setup_needle_thickness_->setEditIndex(setup_needle_selection_->index());
    setup_needle_length_->setEditIndex(setup_needle_selection_->index());
    setup_needle_gap_->setEditIndex(setup_needle_selection_->index());
    setup_needle_image_->setEditIndex(setup_needle_selection_->index());

    setup_needle_length_->setMax(100 - setup_needle_gap_->at(setup_needle_selection_->index()));
    setup_needle_gap_->setMax(100 - setup_needle_length_->at(setup_needle_selection_->index()));

    changeNeedleStyle();
}

void LH_Dial::newLinkedValue()
{
    while(linkedValues.count()<setup_linked_values_->size())
        linkedValues.append( QList<double>() );
    while(linkedValues.count()>setup_linked_values_->size())
        linkedValues.removeLast();

    for(int n=0; n<setup_linked_values_->size(); n++)
    {
        double val = setup_linked_values_->at(n);
        linkedValues[n].append(val * linkedValueMultiplier_);
    }
    callback(lh_cb_notify);
}

void LH_Dial::syncNeedleDataArrays()
{
    int _needleCount = needleCount();
    setup_needle_style_->resize(_needleCount, "Line" );
    setup_needle_color_->resize(_needleCount, QColor(Qt::red).rgba());
    setup_needle_thickness_->resize(_needleCount, 3);
    setup_needle_length_->resize(_needleCount, 90);
    setup_needle_gap_->resize(_needleCount, 0);
    setup_needle_image_->resize(_needleCount, "");
}
