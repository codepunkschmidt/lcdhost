/**
  \file     LH_QtCPU.h
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009-2011, Johan Lindh

  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of Link Data Stockholm nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
  */

#ifndef LH_QTCPU_H
#define LH_QTCPU_H

#include <QObject>
#include <QQueue>

#include "lh_plugin.h"
#include "LH_Qt_int.h"
#include "LH_Qt_array_int.h"
#include "LH_QtInstance.h"
#include "LH_Qt_QSlider.h"

class LH_QtCPU : public QObject
{
    Q_OBJECT

    QQueue<int*> load_;

protected:
    LH_Qt_array_int *link_coreloads_;
    LH_Qt_QSlider *setup_smoothing_;

public:
    explicit LH_QtCPU(LH_QtInstance *parent);
    ~LH_QtCPU();

    LH_QtInstance *parent() const { return static_cast<LH_QtInstance *>(QObject::parent()); }

    int count() const { return link_coreloads_->size(); }
    int min() const { return link_coreloads_->min(); }
    int max() const { return link_coreloads_->max(); }
    int samples() { return setup_smoothing_->value() + 1; }

    int coreload(int n); // min() ... max()
    int averageload(); // min() ... max()

    void smoothingOrder(int n) { setup_smoothing_->setOrder(n); }
    void smoothingHidden(bool hide) { setup_smoothing_->setFlag(LH_FLAG_HIDDEN, hide); }

public slots:
    void gotData();
};

#endif // LH_QTCPU_H
