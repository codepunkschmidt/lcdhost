/**
  \file     LH_TextRSS.h
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009 Johan Lindh

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

#ifndef LH_TEXTRSS_H
#define LH_TEXTRSS_H

#include <QNetworkAccessManager>
#include <QUrl>
#include <QWidget>
#include <QBuffer>
#include <QXmlStreamReader>
#include <QDateTime>

#include "LH_Text.h"
#include "LH_Qt_QString.h"
#include "LH_Qt_QSlider.h"
#include "LH_Qt_InputState.h"
#include "LH_Qt_QStringList.h"
#include "LH_Qt_int.h"

class LH_TextRSS : public LH_Text
{
    Q_OBJECT
    QStringList titles_;
    QStringList links_;
    int nowshowing_;
    int switchwait_;
    QXmlStreamReader xml_;
    QUrl url_;
    QDateTime lastrefresh_;
    QTime lastopen_;

    QString currentTag;
    QString linkString;
    QString titleString;
    QNetworkAccessManager *nam_;
    int connectionId;

    void parseXml();

protected:
    LH_Qt_QString *setup_url_;
    LH_Qt_QSlider *setup_delay_;
    LH_Qt_QStringList *setup_method_;
    LH_Qt_InputState *setup_browser_;
    LH_Qt_InputState *setup_prev_;
    LH_Qt_InputState *setup_next_;
    LH_Qt_int *setup_refresh_;

public:
    LH_TextRSS(const char *name);

    int notify(int code,void* param);
    QImage *render_qimage( int w, int h );
    void setRssItem();

    static lh_class *classInfo();

public slots:
    void openBrowser(QString,int,int);
    void prevHeadline(QString,int,int);
    void nextHeadline(QString,int,int);
    void fetch();
    void finished(QNetworkReply*);
};

#endif // LH_TEXTRSS_H
