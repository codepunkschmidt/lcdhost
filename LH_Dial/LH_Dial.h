/**
  \file     LH_Dial.h
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

#ifndef LH_DIAL_H
#define LH_DIAL_H

#include "../LH_QtInstance.h"
#include "../LH_Qt_QColor.h"
#include "../LH_Qt_QStringList.h"
#include "../LH_Qt_QSlider.h"
#include "../LH_Qt_QFileInfo.h"
#include "../LH_Qt_bool.h"
#include "../LH_Qt_int.h"
#include "../LH_Qt_QTextEdit.h"

#include <QList>
#include <QStringList>

class tickObject
{
public:
    tickObject(int _count, int _width, float _length, float _gap, QColor _color = Qt::black)
    {
        color = _color;
        width = _width;
        count = _count;
        length = _length;
        gap = _gap;
    }

    int count;
    int width;
    QColor color;
    float gap;
    float length;
};

struct tickSet
{
    QList<tickObject> fullCircle;
    QList<tickObject> semiCircle;
    QList<tickObject> quarterCircle;
};

class LH_Dial : public LH_QtInstance
{
    Q_OBJECT

    qreal min_;
    qreal max_;

    QString faceCode_;
    QImage *faceImage_;

    QImage getFace();
    QImage getNeedle(int needleID, qreal degrees, int& needleStyle);
    void getCenter(QPointF& center);
    void getCenter(QPointF& center, float& horzSize, float& vertSize);
    void getRotationData(qreal startAngle, qreal angle, float& centerX, float& centerY, float& xlen, float& ylen, float& radians);
    void paintLine(QPainter& painter, QPen& pen, qreal startAngle, qreal angle, qreal relLength, qreal gap = 0);
    void paintImage(QPainter& painter, QImage needleImage, qreal startAngle, qreal angle, bool reverseOffsetting = false);
    QString colString(QColor col);

    QList<float> needle_pos_;
    QList<float> needle_val_;
    QList<float> needle_step_;
    QList<QString> needleCode_;
    QList<QImage*> needleImage_;
    QList<bool> needle_vis_;

    void loadNeedleConfig(int lineID, int& needleStyle, QColor& needleColor, int& needleThick, int& needleLength, int& needleGap, QString& needleImage);

    static const bool isDebug = false;

    QString buildNeedleConfig();

    bool polling_on_;

    float maxDegrees();
    float startDegrees();
protected:
    bool isClock;

    LH_Qt_QStringList *setup_type_;
    LH_Qt_QStringList *setup_orientation_;
    LH_Qt_bool *setup_needles_reverse_;
    LH_Qt_bool *setup_needles_smooth_;


    LH_Qt_QColor *setup_bgcolor_;

    LH_Qt_QStringList *setup_face_style_;
    LH_Qt_QColor *setup_face_pencolor_;
    LH_Qt_QColor *setup_face_fillcolor1_;
    LH_Qt_QColor *setup_face_fillcolor2_;
    LH_Qt_QFileInfo* setup_face_image_;
    LH_Qt_bool *setup_face_ticks_;

    LH_Qt_QStringList *setup_needle_selection_;

    LH_Qt_QStringList *setup_needle_style_;
    LH_Qt_QColor *setup_needle_color_;
    LH_Qt_int *setup_needle_thickness_;
    LH_Qt_int *setup_needle_length_;
    LH_Qt_int *setup_needle_gap_;
    LH_Qt_QFileInfo* setup_needle_image_;

    LH_Qt_QTextEdit *setup_needle_configs_;

    void setNeedleVisibility(bool visible, int index = 0);
public:    
    LH_Dial( const char *name, LH_QtPlugin *parent = 0 );
    ~LH_Dial();

    int polling();
    QImage *render_qimage( int w, int h );

    qreal min() const { return min_; }
    qreal max() const { return max_; }

    bool setMin( qreal r ); // return true if rendering needed
    bool setMax( qreal r ); // return true if rendering needed
    bool setVal(qreal value, int index = 0, bool repoll = false);
    void setVal( qreal *values, int total ) {
        bool repoll = false;
        for( int i=0; i<total; ++i )
            repoll = setVal( values[i], i, repoll );
    }
    void setVal( const QVector<qreal> &values ) {
        bool repoll = false;
        for( int i=0; i<values.size(); ++i )
            repoll = setVal( values.at(i), i, repoll );
    }

    void addNeedle(QString name);
    int needleCount();
    void clearNeedles();
    void setNeedles(QStringList names) {clearNeedles(); for(int i=0; i<names.length(); i++) addNeedle(names[i]);}

    void drawDial();

    tickSet ticks;

public slots:
    void changeType();
    void changeFaceStyle();
    void changeNeedleStyle();
    void changeSelectedNeedle();
    void updateSelectedNeedle();

};

#endif // LH_DIAL_H
