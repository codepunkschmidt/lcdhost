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

#ifndef LH_MONITORINGIMAGE_H
#define LH_MONITORINGIMAGE_H

#include <QtGlobal>
#include <QHash>

#include "../LH_QtInstance.h"
#include "../LH_Qt_QString.h"
#include "../LH_Qt_QStringList.h"
#include "../LH_Qt_QFileInfo.h"
#include "../LH_Qt_QTextEdit.h"
#include "../LH_Qt_int.h"
#include "../LH_Qt_float.h"

#include "LH_MonitoringData.h"

struct imageMapData
{
    QString code;
    QString Image;
};

class LH_MonitoringImage:public LH_QtInstance
{
    Q_OBJECT

protected:
    LH_MonitoringData *data_;

    QString statusCode_;
    QHash<QString, QStringList> *imageDefinitions;

    void setVisibility(LH_Qt_int* qt, bool ReadOnly, bool Hidden, int minValue, int maxValue);
    void setVisibility(LH_Qt_float* qt, bool ReadOnly, bool Hidden, float minValue, float maxValue);

    LH_Qt_QFileInfo *setup_file_;

    LH_Qt_int *setup_thresh_vlo_int_;
    LH_Qt_int *setup_thresh_lo_int_;
    LH_Qt_int *setup_thresh_hi_int_;
    LH_Qt_int *setup_thresh_vhi_int_;

    LH_Qt_float *setup_thresh_vlo_float_;
    LH_Qt_float *setup_thresh_lo_float_;
    LH_Qt_float *setup_thresh_hi_float_;
    LH_Qt_float *setup_thresh_vhi_float_;

    LH_Qt_QString *setup_text_;

    void createThresholdSet(QString title, LH_Qt_float *&thresh_flt, LH_Qt_int *&thresh_int, bool low_value);
public:
    LH_MonitoringImage();
    ~LH_MonitoringImage();

    int polling();
    int notify(int n,void* p);
    int width( void*obj,int h );
    int height( void*obj,int h );
    QImage *render_qimage( int w, int h );


    QString getImageName();
    static lh_class *classInfo() { return NULL; }

    void connect_changeType(QObject* obj);
    void connect_updateImage(QObject* obj);

public slots:
    void fileChanged();
    void changeType();
    void syncThresh_Int();
    void syncThresh_Float();
    void updateImage(bool rerender = false);

    void setTypeSelection()    { data_->setTypeSelection();     }
    void changeTypeSelection() { data_->changeTypeSelection();  }
    void setGroupSelection()   { data_->setGroupSelection();    }
    void changeGroupSelection(){ data_->changeGroupSelection(); }
    void setItemSelection()    { data_->setItemSelection();     }
    void changeItemSelection() { data_->changeItemSelection();  }
};

#endif // LH_MONITORINGIMAGE_H
