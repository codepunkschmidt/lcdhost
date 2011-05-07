struct locationData
{
    char city[50];
    char region[50];
    char country[50];
};
struct unitsData
{
    char temperature[50];
    char distance[50];
    char pressure[50];
    char speed[50];
};
struct windData
{
    char chill[50];
    char direction[50];
    char speed[50];
};
struct atmosphereData
{
    char humidity[50];
    char visibility[50];
    char pressure[50];
    char rising[50];
};
struct astronomyData
{
    char sunrise[50];
    char sunset[50];
};
struct conditionData
{
    char text[50];
    char code[50];
    char temp[50];
    char date[50];
};
struct forecastData
{
    char day[50];
    char relativeDay[50];
    char date[50];
    char low[50];
    char high[50];
    char text[50];
    char code[50];
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
    forecastData forecastToday;
    forecastData forecastTomorrow;
    forecastData forecastDay3;
    forecastData forecastDay4;
    forecastData forecastDay5;
};
