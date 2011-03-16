/**
  \file     LH_WeatherText.cpp
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

#include <QFont>
#include <QFontMetrics>
#include <QTime>
#include <QRegExp>

#include <stdio.h>
#include <windows.h>

#include "LH_WeatherText.h"

LH_PLUGIN_CLASS(LH_WeatherText)

lh_class *LH_WeatherText::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "Weather",
        "WeatherText",
        "Weather Text",
        -1, -1,
        lh_instance_calltable_NULL
    };

    if( classInfo.width == -1 )
    {
        QFont font;
        QFontMetrics fm( font );
        classInfo.height = fm.height();
        classInfo.width = fm.width("100%");
    }

    return &classInfo;
}

LH_WeatherText::LH_WeatherText(const char *name) : LH_Text( name )
{
    //setup_text_->setName( "Text" );
    setup_text_->setFlag( LH_FLAG_READONLY, true );
    setText("...");

    valueTypes = QStringList();
    valueTypes.append("Location: City");
    valueTypes.append("Location: Region");
    valueTypes.append("Location: Country");
    valueTypes.append("Current Conditions: Description");
    valueTypes.append("Current Conditions: Image Code");
    valueTypes.append("Current Conditions: Temperature");
    valueTypes.append("Current Conditions: Last Updated");
    valueTypes.append("Wind: Chill");
    valueTypes.append("Wind: Direction");
    valueTypes.append("Wind: Speed");
    valueTypes.append("Atmosphere: Humidity");
    valueTypes.append("Atmosphere: Visibility");
    valueTypes.append("Atmosphere: Pressure");
    valueTypes.append("Atmosphere: Rising");
    valueTypes.append("Astronomy: Sunrise");
    valueTypes.append("Astronomy: Sunset");
    valueTypes.append("Forecast Day 1 (Today): Day");
    valueTypes.append("Forecast Day 1 (Today): \"Today\"/\"Tonight\"");
    valueTypes.append("Forecast Day 1 (Today): Date");
    valueTypes.append("Forecast Day 1 (Today): Low");
    valueTypes.append("Forecast Day 1 (Today): High");
    valueTypes.append("Forecast Day 1 (Today): Description");
    //valueTypes.append("Forecast Day 1 (Today): Image Code");
    valueTypes.append("Forecast Day 2 (Tomorrow): Day");
    valueTypes.append("Forecast Day 2 (Tomorrow): \"Tomorrow\"");
    valueTypes.append("Forecast Day 2 (Tomorrow): Date");
    valueTypes.append("Forecast Day 2 (Tomorrow): Low");
    valueTypes.append("Forecast Day 2 (Tomorrow): High");
    valueTypes.append("Forecast Day 2 (Tomorrow): Description");
    //valueTypes.append("Forecast Day 2 (Tomorrow): Image Code");
    valueTypes.append("Forecast Day 3: Day");
    valueTypes.append("Forecast Day 3: Date");
    valueTypes.append("Forecast Day 3: Low");
    valueTypes.append("Forecast Day 3: High");
    valueTypes.append("Forecast Day 3: Description");
    //valueTypes.append("Forecast Day 3: Image Code");
    valueTypes.append("Forecast Day 4: Day");
    valueTypes.append("Forecast Day 4: Date");
    valueTypes.append("Forecast Day 4: Low");
    valueTypes.append("Forecast Day 4: High");
    valueTypes.append("Forecast Day 4: Description");
    //valueTypes.append("Forecast Day 4: Image Code");
    valueTypes.append("Forecast Day 5: Day");
    valueTypes.append("Forecast Day 5: Date");
    valueTypes.append("Forecast Day 5: Low");
    valueTypes.append("Forecast Day 5: High");
    valueTypes.append("Forecast Day 5: Description");
    //valueTypes.append("Forecast Day 5: Image Code");

    setup_value_type_ = new LH_Qt_QStringList(this, "Value Type", valueTypes, 0);
    setup_value_type_->setHelp( "<p>The type of value to display.</p>");
    setup_value_type_->setOrder(-4);
    connect( setup_value_type_, SIGNAL(changed()), this, SLOT(updateText()) );

    setup_append_units_ = new LH_Qt_bool(this, "Append Units", true, 0);
    setup_append_units_->setHelp( "<p>Append the units to the text.</p>");
    setup_append_units_->setOrder(-3);
    connect( setup_append_units_, SIGNAL(changed()), this, SLOT(updateText()) );

    setup_pre_text_ = new LH_Qt_QString( this, "Pre-Text", "", LH_FLAG_AUTORENDER);
    setup_pre_text_->setHelp( "<p>Text to be displayed before the sensor value.</p>");
    setup_pre_text_->setOrder(-3);
    connect( setup_pre_text_, SIGNAL(changed()), this, SLOT(updateText()) );

    setup_post_text_ = new LH_Qt_QString(this, "Post-Text", "", LH_FLAG_AUTORENDER);
    setup_post_text_->setHelp( "<p>Text to be displayed after the sensor value.</p>");
    setup_post_text_->setOrder(-3);
    connect( setup_post_text_, SIGNAL(changed()), this, SLOT(updateText()) );

    return;
}

int LH_WeatherText::notify(int n,void* p)
{
    if( !n || n&LH_NOTE_SECOND )
    {
        updateText();
    }
    return LH_Text::notify(n,p) | LH_NOTE_SECOND;
};

void LH_WeatherText::updateText()
{
    const char* mapname  = "LHWeatherSharedMemory";
    // Create file mapping
    HANDLE weatherMap = OpenFileMappingA(FILE_MAP_READ, FALSE, mapname);
    if(weatherMap)
    {
        weatherData* weather = (weatherData*)MapViewOfFile(weatherMap, FILE_MAP_READ, 0, 0, sizeof(weatherData));

        if (weather) {
            if( setText( getSelectedValueText(weather)  ) )
                callback(lh_cb_render,NULL); // only render if the text changed
            UnmapViewOfFile(weather);
        } else {
            if( setText( "Failed to open shared memory (1)." ) )
                callback(lh_cb_render,NULL); // only render if the text changed
        }

        CloseHandle(weatherMap);
    } else {
        if( setText( "Failed to open shared memory (2)." ) )
            callback(lh_cb_render,NULL); // only render if the text changed
    }
}

QString LH_WeatherText::getSelectedValueText(weatherData* weather)
{
    //return QString("%1:\r\n%2%5\r\n%3 (%4)").arg(weather->location.city, weather->condition.temp, weather->condition.text, weather->condition.code, weather->units.temperature);
    QString selValue = valueTypes.at( setup_value_type_->value() );
    const char* valueText;

    const int unit_none = 0;
    const int unit_dist = 1;
    const int unit_press = 2;
    const int unit_speed = 3;
    const int unit_temp = 4;
    const int unit_perc = 5;
    int units;

    valueText = "<not implimented>";

    if(selValue == "Location: City")                    {valueText = weather->location.city; units = unit_none;}
    if(selValue == "Location: Region")                  {valueText = weather->location.region; units = unit_none;}
    if(selValue == "Location: Country")                 {valueText = weather->location.country; units = unit_none;}

    if(selValue == "Current Conditions: Description")   {valueText = weather->condition.text; units = unit_none;}
    if(selValue == "Current Conditions: Image Code")    {valueText = weather->condition.code; units = unit_none;}
    if(selValue == "Current Conditions: Temperature")   {valueText = weather->condition.temp; units = unit_temp;}
    if(selValue == "Current Conditions: Last Updated")  {valueText = weather->condition.date; units = unit_none;}

    if(selValue == "Wind: Chill")                       {valueText = weather->wind.chill; units = unit_temp;}
    if(selValue == "Wind: Direction")                   {valueText = weather->wind.direction; units = unit_none;}
    if(selValue == "Wind: Speed")                       {valueText = weather->wind.speed; units = unit_speed;}

    if(selValue == "Atmosphere: Humidity")              {valueText = weather->atmosphere.humidity; units = unit_perc;}
    if(selValue == "Atmosphere: Visibility")            {valueText = weather->atmosphere.visibility; units = unit_dist;}
    if(selValue == "Atmosphere: Pressure")              {valueText = weather->atmosphere.pressure; units = unit_press;}
    if(selValue == "Atmosphere: Rising")                {
        int risingState = QString(weather->atmosphere.rising).toInt();
        switch(risingState)
        {
        case 0:
            valueText = "steady";
            break;
        case 1:
            valueText = "rising";
            break;
        case 2:
            valueText = "falling";
            break;
        }
        units = unit_none;
    }

    if(selValue == "Astronomy: Sunrise")                {valueText = weather->astronomy.sunrise; units = unit_none;}
    if(selValue == "Astronomy: Sunset")                 {valueText = weather->astronomy.sunset; units = unit_none;}

    if(selValue == "Forecast Day 1 (Today): Day")              {valueText = weather->forecastToday.day; units = unit_none;}
    if(selValue == "Forecast Day 1 (Today): \"Today\"/\"Tonight\"") {valueText = weather->forecastToday.relativeDay; units = unit_none;}
    if(selValue == "Forecast Day 1 (Today): Date")             {valueText = weather->forecastToday.date; units = unit_none;}
    if(selValue == "Forecast Day 1 (Today): Low")              {valueText = weather->forecastToday.low; units = unit_temp;}
    if(selValue == "Forecast Day 1 (Today): High")             {valueText = weather->forecastToday.high; units = unit_temp;}
    if(selValue == "Forecast Day 1 (Today): Description")      {valueText = weather->forecastToday.text; units = unit_none;}
    if(selValue == "Forecast Day 1 (Today): Image Code")       {valueText = weather->forecastToday.code; units = unit_none;}

    if(selValue == "Forecast Day 2 (Tomorrow): Day")           {valueText = weather->forecastTomorrow.day; units = unit_none;}
    if(selValue == "Forecast Day 2 (Tomorrow): \"Tomorrow\"")  {valueText = weather->forecastTomorrow.relativeDay; units = unit_none;}
    if(selValue == "Forecast Day 2 (Tomorrow): Date")          {valueText = weather->forecastTomorrow.date; units = unit_none;}
    if(selValue == "Forecast Day 2 (Tomorrow): Low")           {valueText = weather->forecastTomorrow.low; units = unit_temp;}
    if(selValue == "Forecast Day 2 (Tomorrow): High")          {valueText = weather->forecastTomorrow.high; units = unit_temp;}
    if(selValue == "Forecast Day 2 (Tomorrow): Description")   {valueText = weather->forecastTomorrow.text; units = unit_none;}
    if(selValue == "Forecast Day 2 (Tomorrow): Image Code")    {valueText = weather->forecastTomorrow.code; units = unit_none;}

    if(selValue == "Forecast Day 3: Day")           {valueText = weather->forecastDay3.day; units = unit_none;}
    if(selValue == "Forecast Day 3: Date")          {valueText = weather->forecastDay3.date; units = unit_none;}
    if(selValue == "Forecast Day 3: Low")           {valueText = weather->forecastDay3.low; units = unit_temp;}
    if(selValue == "Forecast Day 3: High")          {valueText = weather->forecastDay3.high; units = unit_temp;}
    if(selValue == "Forecast Day 3: Description")   {valueText = weather->forecastDay3.text; units = unit_none;}
    if(selValue == "Forecast Day 3: Image Code")    {valueText = weather->forecastDay3.code; units = unit_none;}

    if(selValue == "Forecast Day 4: Day")           {valueText = weather->forecastDay4.day; units = unit_none;}
    if(selValue == "Forecast Day 4: Date")          {valueText = weather->forecastDay4.date; units = unit_none;}
    if(selValue == "Forecast Day 4: Low")           {valueText = weather->forecastDay4.low; units = unit_temp;}
    if(selValue == "Forecast Day 4: High")          {valueText = weather->forecastDay4.high; units = unit_temp;}
    if(selValue == "Forecast Day 4: Description")   {valueText = weather->forecastDay4.text; units = unit_none;}
    if(selValue == "Forecast Day 4: Image Code")    {valueText = weather->forecastDay4.code; units = unit_none;}

    if(selValue == "Forecast Day 5: Day")           {valueText = weather->forecastDay5.day; units = unit_none;}
    if(selValue == "Forecast Day 5: Date")          {valueText = weather->forecastDay5.date; units = unit_none;}
    if(selValue == "Forecast Day 5: Low")           {valueText = weather->forecastDay5.low; units = unit_temp;}
    if(selValue == "Forecast Day 5: High")          {valueText = weather->forecastDay5.high; units = unit_temp;}
    if(selValue == "Forecast Day 5: Description")   {valueText = weather->forecastDay5.text; units = unit_none;}
    if(selValue == "Forecast Day 5: Image Code")    {valueText = weather->forecastDay5.code; units = unit_none;}


    QString resultText = QString(valueText);
    if (resultText=="") {
        resultText = " ";
    } else
        if (setup_append_units_->value())
            switch(units)
            {
            case unit_dist:
                resultText = resultText + QString(weather->units.distance);
                break;
            case unit_press:
                resultText = resultText + QString(weather->units.pressure);
                break;
            case unit_speed:
                resultText = resultText + QString(weather->units.speed);
                break;
            case unit_temp:
                resultText = resultText + QString(weather->units.temperature);
                break;
            case unit_perc:
                resultText = resultText + "%";
                break;
            }
    resultText = setup_pre_text_->value() + resultText + setup_post_text_->value();
    return resultText;
}
