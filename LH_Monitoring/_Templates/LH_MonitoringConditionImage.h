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

#ifndef LH_MONITORINGCONDITIONIMAGE_H
#define LH_MONITORINGCONDITIONIMAGE_H

#include <QtGlobal>
#include <QHash>
#include <QStringList>


#include "../LH_QtInstance.h"
//#include "../LH_Qt_QString.h"
#include "../LH_Qt_QStringList.h"
#include "../LH_Qt_QFileInfo.h"
#include "../LH_Qt_QTextEdit.h"
//#include "../LH_Qt_bool.h"

#include "LH_MonitoringData.h"

struct conditionData
{
    QString code;
    QString image;
    int listIndex;
};

class LH_MonitoringConditionImage:public LH_QtInstance
{
    Q_OBJECT
    QHash<QString,conditionData> conditions_;
    QString conditionValue_;
    QImage emptyImg_;
    QString conditionsListBusy_;

protected:
    LH_MonitoringData *data_;

    LH_Qt_QString *setup_text_;

    LH_Qt_QStringList *setup_available_contitions_;
    LH_Qt_QFileInfo *setup_condition_image_;

    LH_Qt_bool *setup_advanced_editing_;
    LH_Qt_QTextEdit *setup_conditions_list_;

public:
    LH_MonitoringConditionImage( const char *name, LH_QtPlugin *parent = 0 );
    ~LH_MonitoringConditionImage();

    int polling();
    int width( void*obj,int h );
    int height( void*obj,int h );
    QImage *render_qimage( int w, int h );

    QString getImageName();
    static lh_class *classInfo() { return NULL; }

    void updateConditionsList();

    void connect_changeType(QObject* obj);
    void connect_updateImage(QObject* obj);

public slots:
    void changeType();
    void updateImage(bool rerender = false);
    void updateConditions();
    void changeSelectedConditions();
    void updateConditionImage();
    void updateAdvancedEditing();

    void setTypeSelection()    { data_->setTypeSelection();     }
    void changeTypeSelection() { data_->changeTypeSelection();  }
    void setGroupSelection()   { data_->setGroupSelection();    }
    void changeGroupSelection(){ data_->changeGroupSelection(); }
    void setItemSelection()    { data_->setItemSelection();     }
    void changeItemSelection() { data_->changeItemSelection();  }
};

#endif // LH_MONITORINGCONDITIONIMAGE_H
