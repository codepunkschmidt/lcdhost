#ifndef LH_WEATHERDATA_H
#define LH_WEATHERDATA_H

#include <QString>

struct locationData
{
    QString city;
    QString region;
    QString country;
};
struct unitsData
{
    QString temperature;
    QString distance;
    QString pressure;
    QString speed;
};
struct windData
{
    QString chill;
    QString direction;
    QString speed;
};
struct atmosphereData
{
    QString humidity;
    QString visibility;
    QString pressure;
    QString barometricReading;
};
struct astronomyData
{
    QString sunrise;
    QString sunset;
};
struct conditionData
{
    QString text;
    QString code;
    QString temp;
    QString date;
};
struct forecastData
{
    QString day;
    QString relativeDay;
    QString date;
    QString low;
    QString high;
    QString text;
    QString code;
};
struct weatherData
{
    bool isNight;
    locationData location;
    unitsData units;
    windData wind;
    atmosphereData atmosphere;
    astronomyData astronomy;
    conditionData condition;
    int forecastDays;
    forecastData forecast[5];
    QString url;
};

extern weatherData weather_data;


#endif // LH_WEATHERDATA_H
