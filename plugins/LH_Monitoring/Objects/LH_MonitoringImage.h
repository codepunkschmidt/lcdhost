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

#include "LH_QImage.h"
#include "LH_Qt_QString.h"

#include "Sources/LH_MonitoringUI.h"

class LH_MonitoringImage : public LH_QImage
{
    Q_OBJECT

protected:
    LH_MonitoringUI *ui_;
    LH_Qt_QString *setup_value_;

public:
    LH_MonitoringImage();
    const char *userInit();
    int notify(int n,void* p);
    static lh_class *classInfo();

    void updateValue();

public slots:
    void configChanged()  { updateValue(); }

};

#endif // LH_MONITORINGIMAGE_H
