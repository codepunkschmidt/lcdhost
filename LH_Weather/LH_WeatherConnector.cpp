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

#include <QDebug>
#include <QPainter>
#include <QDesktopServices>
#include "LH_WeatherConnector.h"
#include <QDate>
#include <QRegExp>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QNetworkProxy>

LH_PLUGIN_CLASS(LH_WeatherConnector)

lh_class *LH_WeatherConnector::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "Weather",
        "WeatherConnector",
        "Weather Connector",
        -1, -1,
        lh_instance_calltable_NULL
    };

    if( classInfo.width == -1 )
    {
        QFont arial10("Arial",10);
        QFontMetrics fm( arial10 );
        classInfo.height = fm.height();
        classInfo.width = classInfo.height * 4;
    }

    return &classInfo;
}

LH_WeatherConnector::LH_WeatherConnector(const char *name) : LH_Text (name)
{
    weatherMap.setKey("LHWeatherSharedMemory");
    lastrefresh_ = QDateTime::currentDateTime();

    // Hide inherited attributes we don't use
    setup_text_->setFlag( LH_FLAG_HIDDEN, true );
    setup_text_->setFlag( LH_FLAG_READONLY, true );
    setup_text_->setFlag( LH_FLAG_NOSAVE, true );
    setText(" ");
    setup_horizontal_->setFlag( LH_FLAG_HIDDEN, true );
    setup_vertical_->setFlag( LH_FLAG_HIDDEN, true );
    setup_scrollrate_->setFlag( LH_FLAG_HIDDEN, true );
    setup_scrollstep_->setFlag( LH_FLAG_HIDDEN, true );

    setup_font_->setFlag( LH_FLAG_HIDDEN, true );
    setup_fontresize_->setFlag( LH_FLAG_HIDDEN, true );
    setup_pencolor_->setFlag( LH_FLAG_HIDDEN, true );


    setup_location_name_ = new LH_Qt_QString(this,"Location",QString("London UK"));
    setup_location_name_->setHelp("The location whose weather you want to display");
    setup_location_name_->setOrder(-5);
    connect( setup_location_name_, SIGNAL(changed()), this, SLOT(fetchWOEID()));

    setup_yahoo_woeid_ = new LH_Qt_QString(this,"Y! WOEID",QString("26459500"), LH_FLAG_HIDDEN);
    setup_yahoo_woeid_->setHelp("Internal use only: Yahoo Where On Earth ID");
    setup_yahoo_woeid_->setOrder(-4);

    setup_longlat_ = new LH_Qt_QString(this,"LongLat",QString(""), LH_FLAG_HIDDEN);
    setup_longlat_->setHelp("Internal use only: Longitude & Latitude");
    setup_longlat_->setOrder(-4);

    setup_yahoo_5dayid_ = new LH_Qt_QString(this,"Y! 5Day ID",QString("UKXX1726"), LH_FLAG_HIDDEN);
    setup_yahoo_5dayid_->setHelp("Internal use only: Yahoo id code for the 5-day feed");
    setup_yahoo_5dayid_->setOrder(-4);

    setup_city_ = new LH_Qt_QString(this," ",QString(), LH_FLAG_READONLY);
    setup_city_->setHelp("<p>The location whose weather is currently being displayed.</p>"
                         "<p>The weather connector tries to look up the city as entered in the \"Location\" box, and displays the best result here.</p>");
    setup_city_->setOrder(-4);

    setup_browser_ = new LH_Qt_InputState(this,tr("Open in browser"),QString(),LH_FLAG_AUTORENDER);
    setup_browser_->setHelp("Defining a key here will allow you to open the forecast in your browser for more details");
    setup_browser_->setOrder(-4);
    connect( setup_browser_, SIGNAL(input(QString,int,int)), this, SLOT(openBrowser(QString,int,int)) );

    setup_usage_ = new LH_Qt_QTextEdit( this, "~","The Weather Connector draws on Yahoo's weather feeds and makes the data available to all the other weather components. Adding multiple connectors is not recommended as this will end up confusing the other components.",LH_FLAG_LAST|LH_FLAG_READONLY);

    QStringList unitTypes = QStringList();
    unitTypes.append("Metric (Centigrade, Kilometers, etc)");
    unitTypes.append("Imperial (Fahrenheit, Miles, etc)");
    setup_units_type_ = new LH_Qt_QStringList(this, "Units", unitTypes, 0);
    setup_units_type_->setHelp("Select whether you want metric (European) units or imperial (British Commonwealth & USA)");
    setup_units_type_->setOrder(-1);
    connect( setup_units_type_, SIGNAL(changed()), this, SLOT(fetch2DayU()) );

    setup_method_ = NULL;

    setup_refresh_ = new LH_Qt_int(this,tr("Refresh (minutes)"),5);
    setup_refresh_->setHelp("How long to wait before checking for an update to the feed (in minutes)");
    connect( setup_refresh_, SIGNAL(changed()), this, SLOT(requestPolling()) );

    setup_current_url_ = new LH_Qt_QString(this,"Full Weather URL",QString("N/A"),LH_FLAG_READONLY | LH_FLAG_HIDDEN);
    setup_current_url_->setHelp("Internal use only: URL to the page showing the full forecast");

    connect(&http2Day,  SIGNAL(requestFinished(int,bool)), this, SLOT(finished2Day(int,bool)));
    connect(&http5Day,  SIGNAL(requestFinished(int,bool)), this, SLOT(finished5Day(int,bool)));
    connect(&httpWOEID, SIGNAL(requestFinished(int,bool)), this, SLOT(finishedWOEID(int,bool)));

    if (!openWeatherMemory())
    {
        // TODO: Find a way of actually detecting if multiple connectors are active & do this stuff...
        setup_text_->setName( "~" );
        setup_text_->setFlag( LH_FLAG_HIDDEN, false );
        setup_text_->setValue("Cannot have multiple weather connectors. Please delete me.");
    }
}

LH_WeatherConnector::~LH_WeatherConnector()
{
    closeWeatherMemory();
    disconnect(&http2Day, SIGNAL(requestFinished(int,bool)));
    disconnect(&http5Day, SIGNAL(requestFinished(int,bool)));
    disconnect(&httpWOEID, SIGNAL(requestFinished(int,bool)));
    return ;
}


void LH_WeatherConnector::fetch2Day()
{
    fetchWeather(false, xml2Day_, http2Day);
}

void LH_WeatherConnector::fetch2DayU()
{
    if(debugHTTP) qDebug() << "LH_WeatherConnector: Units Changed: Update Weather (2Day)";
    fetchWeather(false, xml2Day_, http2Day);
}

void LH_WeatherConnector::fetch5Day()
{
    lastrefresh_ = QDateTime::currentDateTime();
    fetchWeather(true, xml5Day_, http5Day);
}

void LH_WeatherConnector::fetchWeather(bool is5Day, QXmlStreamReader& xml_, QHttp& http)
{
    if(setup_yahoo_woeid_->value()=="") return;
    http.abort();

    xml_.clear();
    QString unitValue = "c";
    if(setup_units_type_->value()==1) unitValue = "f";

    QString host;
    QString path;
    QString params = "";
    if(is5Day)
    {
        host = "xml.weather.yahoo.com";
        path = QString("/forecastrss/%1_%2.xml").arg(setup_yahoo_5dayid_->value(),unitValue);
    } else {
        host = "weather.yahooapis.com";
        path = QString("/forecastrss");
        params = QString("w=%1&u=%2").arg(setup_yahoo_woeid_->value(),unitValue);
    }
    QUrl url = QUrl::fromUserInput(QString("http://%1%2").arg(host,path));

    QNetworkProxyQuery npq(url);
    QList<QNetworkProxy> listOfProxies = QNetworkProxyFactory::systemProxyForQuery(npq);
    if(listOfProxies.count()!=0)
        if(listOfProxies.at(0).type() != QNetworkProxy::NoProxy) {
            if(debugHTTP) qDebug() << "LH_WeatherConnector: Using Proxy: " << listOfProxies.at(0).hostName()<< ":" << QString::number(listOfProxies.at(0).port());
            http.setProxy(listOfProxies.at(0));
        }

    QString fullUrl = QString("http://%1%2?%3").arg(host,path,params);
    if(debugHTTP) qDebug() << "LH_WeatherConnector: Fetch " << (is5Day? "5Day": "2Day") << " via " << fullUrl;
    http.setHost(url.host());
    connectionId = http.get(fullUrl);

    /*
    QHttpRequestHeader header("POST", QString("http://%1%2?%3").arg(host,path,params));
    header.setValue( "Host", host );
    header.setContentType("application/x-www-form-urlencoded");
    header.setValue( "Accept-Language" , "fr-CA" );
    http.setHost(host);
    connectionId = http.request(header, params.toUtf8());
    */
}

void LH_WeatherConnector::fetchWOEID()
{
    if( weather )
    {
        if(setup_location_name_->value()=="") return;
        httpWOEID.abort();
        // App ID is meant to be unique to each app. If you want to build your own weather-related
        // application, Yahoo ask that sign up for your own ID (free of charge) via their developer
        // site. Alternatvely you can use the id "YahooDemo" (without the quotes) to use the service
        // without signing up. The only thing Yahoo might get upset about is if someone uses the id
        // below for something other than "LH_Weather". But as it's open source there's nothing I
        // can do about it apart from write this...
        QString appid = "itP1aXDV34FW8OAAepdI2XJOKWWqJRUvV0NC_QaGlLwTryEZGw228CtxtzzYv9wceq73jDvqTYFhhA--";
        QString locationName = setup_location_name_->value();
        locationName = QString(QUrl::toPercentEncoding(locationName.replace(' ','-')));
        QUrl url = QUrl::fromUserInput(QString("http://where.yahooapis.com/v1/places.q('%1')").arg(locationName));
        url.addQueryItem("appid",appid);

        QNetworkProxyQuery npq(url);
        QList<QNetworkProxy> listOfProxies = QNetworkProxyFactory::systemProxyForQuery(npq);
        if(listOfProxies.count()!=0)
            if(listOfProxies.at(0).type() != QNetworkProxy::NoProxy) {
                if(debugHTTP) qDebug() << "LH_WeatherConnector: Using Proxy: " << listOfProxies.at(0).hostName() << ":" << QString::number(listOfProxies.at(0).port());
                httpWOEID.setProxy(listOfProxies.at(0));
            }

        httpWOEID.setHost(url.host());
        if(debugHTTP) qDebug() << "LH_WeatherConnector: Fetch WOEID for: " << setup_location_name_->value() << " via " << url.toString();

        setNoForecast(weather->forecastToday);
        setNoForecast(weather->forecastTomorrow);
        setNoForecast(weather->forecastDay3);
        setNoForecast(weather->forecastDay4);
        setNoForecast(weather->forecastDay5);

        connectionId = httpWOEID.get( url.toString() );
    }
}

void LH_WeatherConnector::finished2Day( int id, bool error )
{
    Q_UNUSED(id);

    if (error)
    {
        qWarning() << "LH_WeatherConnector: Error during HTTP (2Day) fetch:" << http2Day.errorString() << http2Day.lastResponse().statusCode();
    }
    else
    {
        if (debugHTTP) qDebug() << "LH_WeatherConnector: HTTP (2Day) Request Complete" << http2Day.lastResponse().statusCode();
        xml2Day_.clear();
        QByteArray respData = http2Day.readAll();
        if(respData.length()!=0)
        {
            if(debugSaveXML)saveXMLResponse(respData,"2Day");
            xml2Day_.addData(respData);
            parseXml2Day();
        }
    }
}

void LH_WeatherConnector::finished5Day( int id, bool error )
{
    Q_UNUSED(id);
    if (error)
    {
        qWarning() << "LH_WeatherConnector: Error during HTTP (5Day) fetch:" << http5Day.errorString() << http5Day.lastResponse().statusCode();
    }
    else
    {
        if (debugHTTP) qDebug() << "LH_WeatherConnector: HTTP (5Day) Request Complete" << http5Day.lastResponse().statusCode();
        xml5Day_.clear();
        QByteArray respData = http5Day.readAll();
        if(respData.length()!=0)
        {
            if(debugSaveXML)saveXMLResponse(respData,"5Day");
            xml5Day_.addData(respData);
            parseXml5Day();
        }
    }
}

void LH_WeatherConnector::finishedWOEID( int id, bool error )
{
    Q_UNUSED(id);
    if (error)
    {
        qWarning() << "LH_WeatherConnector: Error during HTTP (WOEID) fetch:" << httpWOEID.errorString() << httpWOEID.lastResponse().statusCode();
    }
    else
    {
        if (debugHTTP) qDebug() << "LH_WeatherConnector: HTTP (WOEID) Request Complete" << httpWOEID.lastResponse().statusCode();
        xmlWOEID_.clear();
        QByteArray respData = httpWOEID.readAll();
        if(respData.length()!=0)
        {
            if(debugSaveXML)saveXMLResponse(respData,"WOEID");
            xmlWOEID_.addData(respData);
            parseXmlWOEID();
        }
    }
}

void LH_WeatherConnector::openBrowser(QString key,int flags,int value)
{
    qDebug() << "Open Browser";
    Q_UNUSED(key);
    Q_UNUSED(flags);
    Q_UNUSED(value);
    if( setup_current_url_->value() !="" )
        QDesktopServices::openUrl( QUrl::fromUserInput(setup_current_url_->value()) );
}

void LH_WeatherConnector::saveXMLResponse(QByteArray data, QString docType)
{
    QDateTime now = QDateTime::currentDateTime();
    QFile file(QString("%3\\lcdhost.weather.%1.%2.xml").arg(docType, now.toString("yyMMddhhmmsszzz"), state()->dir_binaries));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "LH_WeatherConnector: Unable to save XML data";
    } else {
        file.write(data);
        file.close();
        qDebug() << "LH_WeatherConnector: XML file created" << file.fileName() << data.length();
    }
}

int LH_WeatherConnector::notify(int code,void* param)
{
    if( code & LH_NOTE_SECOND )
    {
        if( lastrefresh_.addSecs(60*setup_refresh_->value()) < QDateTime::currentDateTime() )
        {
            lastrefresh_ = QDateTime::currentDateTime();
            if(debugHTTP) qDebug() << "LH_WeatherConnector: Update Weather (2Day)";
            fetch2Day();
        }
    }
    return LH_NOTE_SECOND | LH_Text::notify(code,param);
}

QDate LH_WeatherConnector::toDate(QString str, bool isLong)
{
    QString ptn = "^([0-9]*) (\\w\\w\\w) ([0-9]*)$";
    if(isLong)
        ptn = "^\\w\\w\\w, ([0-9]*) (\\w\\w\\w) ([0-9]*) [0-9]*:[0-9]* \\w\\w.*$";
    QRegExp re(ptn);
    if (re.indexIn(str) != -1) {
        int day = re.cap(1).toInt();
        int month = 0;
        if ( re.cap(2) == "Jan") month = 1; else
        if ( re.cap(2) == "Feb") month = 2; else
        if ( re.cap(2) == "Mar") month = 3; else
        if ( re.cap(2) == "Apr") month = 4; else
        if ( re.cap(2) == "May") month = 5; else
        if ( re.cap(2) == "Jun") month = 6; else
        if ( re.cap(2) == "Jul") month = 7; else
        if ( re.cap(2) == "Aug") month = 8; else
        if ( re.cap(2) == "Sep") month = 9; else
        if ( re.cap(2) == "Oct") month = 10; else
        if ( re.cap(2) == "Nov") month = 11; else
        if ( re.cap(2) == "Dec") month = 12;
        int year = re.cap(3).toInt();
        return QDate(year,month,day);
    }

    return QDate(1,1,1);
}

void LH_WeatherConnector::parseXml2Day()
{ parseXmlWeather(false, xml2Day_, http2Day); }

void LH_WeatherConnector::parseXml5Day()
{ parseXmlWeather(true, xml5Day_, http5Day); }

void LH_WeatherConnector::parseXmlWeather(bool is5Day, QXmlStreamReader& xml_, QHttp& http)
{
    Q_UNUSED(http);
    if( weather )
    {
        weather->forecastDays = 0;

        if(!is5Day)setNoForecast(weather->forecastToday);
        if(!is5Day)setNoForecast(weather->forecastTomorrow);
        if(is5Day) setNoForecast(weather->forecastDay3);
        if(is5Day) setNoForecast(weather->forecastDay4);
        if(is5Day) setNoForecast(weather->forecastDay5);

        QString currentTag;
        if(!is5Day) setup_current_url_->setValue( "" );
        while (!xml_.atEnd())
        {
            xml_.readNext();
            if (xml_.isStartElement())
            {
                currentTag = xml_.name().toString();
                if(!is5Day)
                {
                    if( xml_.name() == "location" )
                    {
                        strcpy(weather->location.city, getWeatherValue(xml_, "city"));
                        strcpy(weather->location.region, getWeatherValue(xml_, "region"));
                        strcpy(weather->location.country, getWeatherValue(xml_, "country"));
                    }
                    if( xml_.name() == "units" )
                    {
                        strcpy(weather->units.temperature, getWeatherValue(xml_, "temperature", "°"));
                        strcpy(weather->units.distance, getWeatherValue(xml_, "distance"));
                        strcpy(weather->units.pressure, getWeatherValue(xml_, "pressure"));
                        strcpy(weather->units.speed, getWeatherValue(xml_, "speed"));
                    }
                    if( xml_.name() == "wind" )
                    {
                        strcpy(weather->wind.chill, getWeatherValue(xml_, "chill"));
                        strcpy(weather->wind.direction, getWeatherValue(xml_, "direction"));
                        strcpy(weather->wind.speed, getWeatherValue(xml_, "speed"));
                    }
                    if( xml_.name() == "atmosphere" )
                    {
                        strcpy(weather->atmosphere.humidity, getWeatherValue(xml_, "humidity"));
                        strcpy(weather->atmosphere.visibility, getWeatherValue(xml_, "visibility"));
                        strcpy(weather->atmosphere.pressure, getWeatherValue(xml_, "pressure"));
                        strcpy(weather->atmosphere.rising, getWeatherValue(xml_, "rising"));
                    }
                    if( xml_.name() == "astronomy" )
                    {
                        strcpy(weather->astronomy.sunrise, getWeatherValue(xml_, "sunrise"));
                        strcpy(weather->astronomy.sunset, getWeatherValue(xml_, "sunset"));
                    }
                    if( xml_.name() == "condition" )
                    {
                        strcpy(weather->condition.text, getWeatherValue(xml_, "text"));
                        strcpy(weather->condition.code, getWeatherValue(xml_, "code"));
                        strcpy(weather->condition.temp, getWeatherValue(xml_, "temp"));
                        strcpy(weather->condition.date, getWeatherValue(xml_, "date"));
                    }
                }
                if( xml_.name() == "forecast" )
                {
                    weather->forecastDays ++;
                    QDate conditionDate = toDate(weather->condition.date, true);
                    QDate forecastDate = toDate(xml_.attributes().value("date").toString(), false);

                    if (forecastDate == conditionDate)            {setForecast(xml_, weather->forecastToday, 0);} else
                    if (forecastDate == conditionDate.addDays(1)) {setForecast(xml_, weather->forecastTomorrow, 1);} else
                    if (forecastDate == conditionDate.addDays(2)) {setForecast(xml_, weather->forecastDay3, 2);} else
                    if (forecastDate == conditionDate.addDays(3)) {setForecast(xml_, weather->forecastDay4, 3);} else
                    if (forecastDate == conditionDate.addDays(4)) {setForecast(xml_, weather->forecastDay5, 4);}
                }
            }
            else if (xml_.isEndElement())
            {
                //nothing
            }
            else if (xml_.isCharacters() && !xml_.isWhitespace())
            {
                if(!is5Day && currentTag == "link")
                {
                    if (setup_current_url_->value()=="")
                    {
                        setup_current_url_->setValue( xml_.text().toString() );
                        QRegExp re = QRegExp("(/([^/_]*)(?:_.|)\\.html)$");
                        if (re.indexIn(xml_.text().toString()) != -1)
                        {
                            if(debugHTTP) qDebug() << "LH_WeatherConnector: Set 5dayid" << re.cap(2);
                            setup_yahoo_5dayid_->setValue(re.cap(2));
                        }
                    }
                }
            }
        }

        if (xml_.error() && xml_.error() != QXmlStreamReader::PrematureEndOfDocumentError)
        {
            if (!is5Day)
                qWarning() << "LH_WeatherConnector: XML ERROR (2Day Parser):" << xml_.lineNumber() << ": " << xml_.errorString();
            else
                qWarning() << "LH_WeatherConnector: XML ERROR (5Day Parser):" << xml_.lineNumber() << ": " << xml_.errorString();
            //http.abort();
        } else {
            if (!is5Day) weather->isNight = checkNight(weather);

            QString cityName = weather->location.city;
            if(QString(weather->location.region).trimmed() != "")
                cityName = cityName + QString(", %1").arg(weather->location.region);
            if(QString(weather->location.country).trimmed() != "")
                cityName = cityName + QString(", %1").arg(weather->location.country);

            setup_city_-> setValue( cityName );

            if (!is5Day && get5Day) fetch5Day();
        }
    }
}

void LH_WeatherConnector::setNoForecast(forecastData& forecast)
{
    if( weather )
    {
        strcpy(forecast.day, QByteArray("N/A"));
        strcpy(forecast.relativeDay, QByteArray("N/A"));
        strcpy(forecast.date, QByteArray("N/A"));
        strcpy(forecast.low, QByteArray("?"));
        strcpy(forecast.high, QByteArray("?"));
        strcpy(forecast.text, QByteArray("Unknown"));
        strcpy(forecast.code, QByteArray("3200"));
    }
}

void LH_WeatherConnector::setForecast(QXmlStreamReader& xml_, forecastData& forecast, int relativeDay)
{
    if( weather )
    {
        if(debugForecast) qDebug() << "LH_WeatherConnector: Adding forecast: " << getWeatherValue(xml_, "day") << getWeatherValue(xml_, "code");

        strcpy(forecast.day, getWeatherValue(xml_, "day"));
        switch (relativeDay)
        {
        case 0:
            if (!weather->isNight) strcpy(forecast.relativeDay, QByteArray("Today")); else
                          strcpy(forecast.relativeDay, QByteArray("Tonight"));
            break;
        case 1:
            strcpy(forecast.relativeDay, QByteArray("Tomorrow"));
            break;
        default:
            strcpy(forecast.relativeDay, getWeatherValue(xml_, "day"));
        }
        strcpy(forecast.date, getWeatherValue(xml_, "date"));
        strcpy(forecast.low, getWeatherValue(xml_, "low"));
        strcpy(forecast.high, getWeatherValue(xml_, "high"));
        strcpy(forecast.text, getWeatherValue(xml_, "text"));
        strcpy(forecast.code, getWeatherValue(xml_, "code"));
    }
}

void LH_WeatherConnector::parseXmlWOEID()
{
    QString currentTag;
    bool foundWOEID = false;
    while (!xmlWOEID_.atEnd())
    {
        xmlWOEID_.readNext();
        if (xmlWOEID_.isStartElement())
        {
            currentTag = xmlWOEID_.name().toString();
        }
        else if (xmlWOEID_.isEndElement())
        {
            //nothing
        }
        else if (xmlWOEID_.isCharacters() && !xmlWOEID_.isWhitespace())
        {
            if( currentTag == "woeid" && !foundWOEID) {
                setup_yahoo_woeid_->setValue( xmlWOEID_.text().toString() );
                setup_yahoo_5dayid_->setValue("");
                foundWOEID = true;
            }
        }
    }

    if (xmlWOEID_.error() && xmlWOEID_.error() != QXmlStreamReader::PrematureEndOfDocumentError)
    {
        qWarning() << "LH_WeatherConnector: XML ERROR (WOEID Parser):" << xmlWOEID_.lineNumber() << ": " << xmlWOEID_.errorString();
        httpWOEID.abort();
    }

    if (!foundWOEID)
    {
        setup_city_->setValue("Location not recognised");
        setup_yahoo_woeid_->setValue( "" );
        setup_yahoo_5dayid_->setValue( "" );
    } else {
        if(debugHTTP) qDebug() << "LH_WeatherConnector: WOEID acquired: " << setup_yahoo_woeid_->value() << " BEGIN FETCH (2Day)";
        fetch2Day();
    }

}

QImage *LH_WeatherConnector::render_qimage( int w, int h )
{
    if( !prepareForRender(w,h) ) return NULL;

    QPainter painter;
    if( painter.begin(image_) )
    {
        QRectF target;

        target.setSize( textimage().size() );
        target.moveLeft( image_->width()/2 - textimage().width()/2 );
        target.moveTop( image_->height()/2 - textimage().height()/2 );

        if( textimage().width() > image_->width() )
            target.moveLeft( 0 );

        painter.drawImage( target, textimage(), textimage().rect() );
        painter.end();
    }

    return image_;
}

bool LH_WeatherConnector::openWeatherMemory()
{
    if( !weatherMap.create(sizeof(weatherData)) && !weatherMap.attach() )
    {
        qWarning() << "LH_WeatherConnector: create memory failed:" << weatherMap.errorString();
        return false;
    }

    weather = (weatherData*)weatherMap.data();
    memset(weather, 0, sizeof(weatherData));
    return true;
}
void LH_WeatherConnector::closeWeatherMemory()
{
    weatherMap.detach();
}

QByteArray LH_WeatherConnector::getWeatherValue(QXmlStreamReader& xml_, QString attrName)
{
    return xml_.attributes().value(attrName).toString().toAscii();
}
QByteArray LH_WeatherConnector::getWeatherValue(QXmlStreamReader& xml_, QString attrName, QString preText)
{
    return ( preText + xml_.attributes().value(attrName).toString() ) .toAscii();
}

int LH_WeatherConnector::toTime(QString time, bool isDateTime)
{
    if (isDateTime) {
        QRegExp re = QRegExp("^\\w\\w\\w, [0-9]* \\w\\w\\w [0-9]* ([0-9]*:[0-9]* \\w\\w).*$");
        time = time.replace(re,"\\1");
    }

    QRegExp re = QRegExp("^([0-9]*):([0-9]*) (\\w\\w)$");
    if (re.indexIn(time) != -1) {
         int hour = re.cap(1).toInt();
         int minute = re.cap(2).toInt();
         if ( re.cap(3) == "pm" && hour != 12 ) hour += 12;
         else
             if ( re.cap(3) == "am" && hour == 12 ) hour -= 12;
         return hour*100 + minute;
    }

    return 0;
}

bool LH_WeatherConnector::checkNight(weatherData* weather)
{
    int dawnTime=toTime(weather->astronomy.sunrise, false);
    int duskTime=toTime(weather->astronomy.sunset,false);
    int currTime=toTime(weather->condition.date,true);

    bool nightResult = currTime<dawnTime || currTime>duskTime;

    return nightResult;
}
