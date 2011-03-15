/**
  \file     LH_WeatherConnector.cpp
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

#ifndef LH_WeatherConnector_H
#define LH_WeatherConnector_H

#include <QHttp>
#include <QUrl>
#include <QWidget>
#include <QBuffer>
#include <QXmlStreamReader>
#include <QDateTime>

#include "../LH_Text/LH_Text.h"
#include "../LH_Qt_QString.h"
#include "../LH_Qt_QSlider.h"
#include "../LH_Qt_InputState.h"
#include "../LH_Qt_QStringList.h"
#include "../LH_Qt_int.h"
#include "../LH_Qt_QTextEdit.h"

#include <stdio.h>
#include <windows.h>

#include "LH_WeatherStructs.cpp"

class LH_WeatherConnector : public LH_Text
{
    Q_OBJECT
    int connectionId;
    QDateTime lastrefresh_;
    static const bool get5Day = true;

    static const bool debugHTTP = false;
    static const bool debugMemory = false;
    static const bool debugForecast = false;
    static const bool debugSaveXML = false;

    QXmlStreamReader xml2Day_;
    QXmlStreamReader xml5Day_;
    QXmlStreamReader xmlWOEID_;

    QHttp http2Day;
    QHttp http5Day;
    QHttp httpWOEID;

    void parseXmlWeather(bool is5Day, QXmlStreamReader& xml_, QHttp& http);
    void parseXml2Day();
    void parseXml5Day();
    void parseXmlWOEID();

    bool openWeatherMemory();
    void closeWeatherMemory();
    QDate toDate(QString str, bool isLong);

    QByteArray getWeatherValue(QXmlStreamReader& xml_, QString attrName);
    QByteArray getWeatherValue(QXmlStreamReader& xml_, QString attrName, QString preText);

    HANDLE weatherMap;
    weatherData *weather;

    void setNoForecast(forecastData& forecast);
    void setForecast(QXmlStreamReader& xml_, forecastData& forecast, int relativeDay);

protected:
    LH_Qt_QString *setup_location_name_;
    LH_Qt_QString *setup_yahoo_woeid_;
    LH_Qt_QString *setup_longlat_;
    LH_Qt_QString *setup_yahoo_5dayid_;
    LH_Qt_QString *setup_city_;
    LH_Qt_QSlider *setup_delay_;
    LH_Qt_QStringList *setup_method_;
    LH_Qt_int *setup_refresh_;
    LH_Qt_QStringList *setup_units_type_;
    LH_Qt_QTextEdit *setup_usage_;
    LH_Qt_InputState *setup_browser_;
    LH_Qt_QString *setup_current_url_;

public:
    LH_WeatherConnector(const char *name);
    ~LH_WeatherConnector();

    int notify(int code,void* param);
    QImage *render_qimage( int w, int h );

    static lh_class *classInfo();

    bool checkNight(weatherData* weather);
    int toTime(QString time, bool isDateTime);

public slots:
    void fetchWeather(bool is5Day, QXmlStreamReader& xml_, QHttp& http);
    void fetch2Day();
    void fetch2DayU();
    void fetch5Day();
    void fetchWOEID();
    void finished2Day(int,bool);
    void finished5Day(int,bool);
    void finishedWOEID(int,bool);
    void openBrowser(QString,int,int);
    void saveXMLResponse(QByteArray,QString);
};

#endif // LH_WeatherConnector_H
