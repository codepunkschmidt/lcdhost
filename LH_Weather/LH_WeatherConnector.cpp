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
        lh_object_calltable_NULL,
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

LH_WeatherConnector::LH_WeatherConnector() : translator("Weather", this)
{
    translator.requestLanguages("en");
    connect(&translator, SIGNAL(languages_updated()), this, SLOT(updateLanguagesList()));

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

    setup_languages_ = new LH_Qt_QStringList(this, "Language", QStringList(), LH_FLAG_NOSAVE);
    setup_languages_->setHelp("<p>Yahoo's Weather API doesn't have multilingual support; the translation is instead done using Google Translate.</p>"
                              "<p>Bad translations can be corrected by editing the translation cache located in the LCDHost directory.</p>");
    setup_language_ = new LH_Qt_QString(this, "^Language Code", "en", LH_FLAG_HIDDEN);
    connect(setup_languages_, SIGNAL(changed()), this, SLOT(selectLanguage()));
    connect(setup_language_, SIGNAL(changed()), this, SLOT(setLanguage()));

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

    connectionId_WOEID = NULL;
    connectionId_2Day = NULL;
    connectionId_5Day = NULL;

    connect(&nam2Day,  SIGNAL(finished(QNetworkReply*)), this, SLOT(finished2Day(QNetworkReply*)));
    connect(&nam5Day,  SIGNAL(finished(QNetworkReply*)), this, SLOT(finished5Day(QNetworkReply*)));
    connect(&namWOEID, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedWOEID(QNetworkReply*)));

}

LH_WeatherConnector::~LH_WeatherConnector()
{
    return ;
}


void LH_WeatherConnector::fetch2Day()
{
    connectionId_2Day = fetchWeather(false, xml2Day_, nam2Day, connectionId_2Day);
}

void LH_WeatherConnector::fetch2DayU()
{
    if(debugHTTP) qDebug() << "LH_WeatherConnector: Units Changed: Update Weather (2Day)";
    connectionId_2Day = fetchWeather(false, xml2Day_, nam2Day, connectionId_2Day);
}

void LH_WeatherConnector::fetch5Day()
{
    lastrefresh_ = QDateTime::currentDateTime();
    connectionId_5Day = fetchWeather(true, xml5Day_, nam5Day, connectionId_5Day);
}

QNetworkReply* LH_WeatherConnector::fetchWeather(bool is5Day, QXmlStreamReader& xml_, QNetworkAccessManager& nam, QNetworkReply* currentReply)
{
    if(setup_yahoo_woeid_->value()=="")
        return 0;
    if(currentReply!=NULL && currentReply->isRunning()) {
        currentReply->abort();
        currentReply->deleteLater();
        currentReply = NULL;
    }

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
            nam.setProxy(listOfProxies.at(0));
        }


    QString fullUrl = QString("http://%1%2?%3").arg(host,path,params);
    if(debugHTTP) qDebug() << "LH_WeatherConnector: Fetch " << (is5Day? "5Day": "2Day") << " via " << fullUrl;

    return nam.get( QNetworkRequest(fullUrl) );
}

void LH_WeatherConnector::fetchWOEID()
{
    if(setup_location_name_->value()=="") return;

    if(connectionId_WOEID!=NULL && connectionId_WOEID->isRunning()) {
        connectionId_WOEID->abort();
        connectionId_WOEID->deleteLater();
        connectionId_WOEID = NULL;
    }

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
            namWOEID.setProxy(listOfProxies.at(0));
        }

    if(debugHTTP) qDebug() << "LH_WeatherConnector: Fetch WOEID for: " << setup_location_name_->value() << " via " << url.toString();

    for(int i = 0; i<5; i++)
        setNoForecast(weather_data.forecast[i]);

    connectionId_WOEID = namWOEID.get( QNetworkRequest(url.toString()) );
}

void LH_WeatherConnector::finished2Day( QNetworkReply* reply )
{
    if( reply )
    {
        if( reply->error() == QNetworkReply::NoError )
        {
            if( reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) == 200 )
            {
                processResponse(reply->readAll(), "2Day", xml2Day_, &LH_WeatherConnector::parseXml2Day);
            }
            else
                qWarning() << "LH_WeatherConnector: Error during HTTP (2Day) fetch:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) << reply->errorString() << reply->url().toString();
        }
        reply->deleteLater();
        reply = NULL;
    }
    connectionId_2Day->deleteLater();
    connectionId_2Day = NULL;
}

void LH_WeatherConnector::finished5Day( QNetworkReply* reply )
{
    if( reply )
    {
        if( reply->error() == QNetworkReply::NoError )
        {
            if( reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) == 200 )
            {
                processResponse(reply->readAll(), "5Day", xml5Day_, &LH_WeatherConnector::parseXml5Day);
            }
            else
                qWarning() << "LH_WeatherConnector: Error during HTTP (5Day) fetch:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) << reply->errorString() << reply->url().toString();
        }
        reply->deleteLater();
        reply = NULL;
    }
    connectionId_5Day->deleteLater();
    connectionId_5Day = NULL;
}

void LH_WeatherConnector::finishedWOEID( QNetworkReply* reply )
{
    if( reply )
    {
        if( reply->error() == QNetworkReply::NoError )
        {
            if( reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) == 200 )
            {
                processResponse(reply->readAll(), "WOEID", xmlWOEID_, &LH_WeatherConnector::parseXmlWOEID);
            }
            else
                qWarning() << "LH_WeatherConnector: Error during HTTP (WOEID) fetch:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) << reply->errorString() << reply->url().toString();
        }
        reply->deleteLater();
        reply = NULL;
    }
    connectionId_WOEID->deleteLater();
    connectionId_WOEID = NULL;
}

void LH_WeatherConnector::processResponse(QByteArray xmlData, QString name, QXmlStreamReader& xmlReader, xmlParserFunc xmlParser)
{
    xmlReader.clear();
    if(xmlData.length()!=0)
    {
        if(debugSaveXML)saveXMLResponse(xmlData,name);
        xmlReader.addData(xmlData);
        (this->*xmlParser)();
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
{ parseXmlWeather(false, xml2Day_); }

void LH_WeatherConnector::parseXml5Day()
{ parseXmlWeather(true, xml5Day_); }

void LH_WeatherConnector::parseXmlWeather(bool is5Day, QXmlStreamReader& xml_)
{
    weather_data.forecastDays = 0;

    for(int i = 0; i<5; i++)
        if((!is5Day && i<2) || (is5Day && i>=2))setNoForecast(weather_data.forecast[i]);

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
                    weather_data.location.city = getWeatherValue(xml_, "city");
                    weather_data.location.region = getWeatherValue(xml_, "region");
                    weather_data.location.country = getWeatherValue(xml_, "country");
                }
                if( xml_.name() == "units" )
                {
                    weather_data.units.temperature = getWeatherValue(xml_, "temperature", "°");
                    weather_data.units.distance = getWeatherValue(xml_, "distance");
                    weather_data.units.pressure = getWeatherValue(xml_, "pressure");
                    weather_data.units.speed = getWeatherValue(xml_, "speed");
                }
                if( xml_.name() == "wind" )
                {
                    weather_data.wind.chill = getWeatherValue(xml_, "chill");
                    weather_data.wind.direction = getWeatherValue(xml_, "direction");
                    weather_data.wind.speed = getWeatherValue(xml_, "speed");
                }
                if( xml_.name() == "atmosphere" )
                {
                    weather_data.atmosphere.humidity = getWeatherValue(xml_, "humidity");
                    weather_data.atmosphere.visibility = getWeatherValue(xml_, "visibility");
                    weather_data.atmosphere.pressure = getWeatherValue(xml_, "pressure");

                    int risingState = getWeatherValue(xml_, "rising").toInt();
                    switch(risingState)
                    {
                    case 0:
                        weather_data.atmosphere.barometricReading = "Steady";
                        break;
                    case 1:
                        weather_data.atmosphere.barometricReading = "Rising";
                        break;
                    case 2:
                        weather_data.atmosphere.barometricReading = "Falling";
                        break;
                    }
                }
                if( xml_.name() == "astronomy" )
                {
                    weather_data.astronomy.sunrise = getWeatherValue(xml_, "sunrise");
                    weather_data.astronomy.sunset = getWeatherValue(xml_, "sunset");
                }
                if( xml_.name() == "condition" )
                {
                    weather_data.condition.text = getWeatherValue(xml_, "text");
                    weather_data.condition.code = getWeatherValue(xml_, "code");
                    weather_data.condition.temp = getWeatherValue(xml_, "temp");
                    weather_data.condition.date = getWeatherValue(xml_, "date");
                }
            }
            if( xml_.name() == "forecast" )
            {
                weather_data.forecastDays ++;
                QDate conditionDate = toDate(weather_data.condition.date, true);
                QDate forecastDate = toDate(xml_.attributes().value("date").toString(), false);

                for(int i=0; i<5; i++)
                    if (forecastDate == conditionDate.addDays(i)) {
                        setForecast(xml_, weather_data.forecast[i], i);
                        break;
                    }
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
        if (!is5Day) weather_data.isNight = checkNight();

        QString cityName = weather_data.location.city;
        if(QString(weather_data.location.region).trimmed() != "")
            cityName = cityName + QString(", %1").arg(weather_data.location.region);
        if(QString(weather_data.location.country).trimmed() != "")
            cityName = cityName + QString(", %1").arg(weather_data.location.country);

        setup_city_-> setValue( cityName );

        if (!is5Day && get5Day)
            fetch5Day();
        else
            requestTranslation();
    }
}

void LH_WeatherConnector::setNoForecast(forecastData &forecast)
{
    forecast.day  = "N/A";
    forecast.relativeDay = "N/A";
    forecast.date = "N/A";
    forecast.low  = "?";
    forecast.high = "?";
    forecast.text = "Unknown";
    forecast.code = "3200";
}

void LH_WeatherConnector::setForecast(QXmlStreamReader& xml_, forecastData& forecast, int relativeDay)
{
    if(debugForecast) qDebug() << "LH_WeatherConnector: Adding forecast: " << getWeatherValue(xml_, "day") << getWeatherValue(xml_, "code");

    forecast.day = getWeatherValue(xml_, "day");
    switch (relativeDay)
    {
    case 0:
        forecast.relativeDay = (!weather_data.isNight? "Today" : "Tonight");
        break;
    case 1:
        forecast.relativeDay = "Tomorrow";
        break;
    default:
        forecast.relativeDay = translator.fullDateName(forecast.day);
        break;
    }
    forecast.date = getWeatherValue(xml_, "date");
    forecast.low = getWeatherValue(xml_, "low");
    forecast.high = getWeatherValue(xml_, "high");
    forecast.text = getWeatherValue(xml_, "text");
    forecast.code = getWeatherValue(xml_, "code");
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
        if(connectionId_WOEID!=NULL && connectionId_WOEID->isRunning())
        {
            connectionId_WOEID->abort();
            connectionId_WOEID->deleteLater();
            connectionId_WOEID = NULL;
        }

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

QString LH_WeatherConnector::getWeatherValue(QXmlStreamReader& xml_, QString attrName)
{
    return xml_.attributes().value(attrName).toString();
}
QString LH_WeatherConnector::getWeatherValue(QXmlStreamReader& xml_, QString attrName, QString preText)
{
    return ( preText + xml_.attributes().value(attrName).toString() );
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

bool LH_WeatherConnector::checkNight()
{
    int dawnTime=toTime(weather_data.astronomy.sunrise, false);
    int duskTime=toTime(weather_data.astronomy.sunset,false);
    int currTime=toTime(weather_data.condition.date,true);

    bool nightResult = currTime<dawnTime || currTime>duskTime;

    return nightResult;
}

void LH_WeatherConnector::requestTranslation()
{
    translator.clear();

    translator.addItem(&weather_data.atmosphere.barometricReading);

    translator.addItem(&weather_data.location.city, ttNoun);
    translator.addItem(&weather_data.location.region, ttNoun);
    translator.addItem(&weather_data.location.country, ttNoun);

    translator.addItem(&weather_data.condition.text);
    translator.addItem(&weather_data.condition.date, ttMonthName);
    translator.addItem(&weather_data.condition.date, ttDayName);

    for(int i=0; i<5; i++)
    {
        translator.addItem(&weather_data.forecast[i].day, ttDayName);
        translator.addItem(&weather_data.forecast[i].relativeDay);
        translator.addItem(&weather_data.forecast[i].date, ttMonthName);
        translator.addItem(&weather_data.forecast[i].date, ttDayName);
        translator.addItem(&weather_data.forecast[i].text);
    }

    translator.request();
}

void LH_WeatherConnector::updateLanguagesList()
{
    setup_languages_->list().clear();
    foreach(QString name, translator.languages.names())
        setup_languages_->list().append(name);
    setup_languages_->refreshList();
    setup_languages_->setValue(translator.languages.codes().indexOf(setup_language_->value()));
}

void LH_WeatherConnector::selectLanguage()
{
    QString code = translator.languages.getCode(setup_languages_->valueText());
    setup_language_->setValue(code);
    translator.setTargetLanguage(code);
    fetch2Day();
}

void LH_WeatherConnector::setLanguage()
{
    translator.setTargetLanguage(setup_language_->value());
    setup_languages_->setValue(translator.languages.codes().indexOf(setup_language_->value()));
    updateLanguagesList();
}
