/* API response deserialization declarations for esp32-weather-epd.
 * Copyright (C) 2022-2023  Luke Marzen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef __API_RESPONSE_H__
#define __API_RESPONSE_H__

#include <cstdint>
#include <vector>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>

#define OWM_NUM_MINUTELY       1 // 61
#define OWM_NUM_HOURLY        48 // 48
#define OWM_NUM_DAILY          8 // 8
#define OWM_NUM_ALERTS         8 // OpenWeatherMaps does not specify a limit, but if you need more alerts you are probably doomed.
#define OWM_NUM_AIR_POLLUTION 24 // Depending on AQI scale, hourly concentrations will need to be averaged over a period of 1h to 24h

typedef struct owm_weather
{
  int     id;               // Weather condition id
  String  main;             // Group of weather parameters (Rain, Snow, Extreme etc.)
  String  description;      // Weather condition within the group (full list of weather conditions). Get the output in your language
  String  icon;             // Weather icon id.
} owm_weather_t;

/*
 * Units – default: kelvin, metric: Celsius, imperial: Fahrenheit.
 */
typedef struct owm_temp
{
  float   morn;             // Morning temperature.
  float   day;              // Day temperature.
  float   eve;              // Evening temperature.
  float   night;            // Night temperature.
  float   min;              // Min daily temperature.
  float   max;              // Max daily temperature.
} owm_temp_t;

/*
 * This accounts for the human perception of weather. Units – default: kelvin, metric: Celsius, imperial: Fahrenheit.
 */
typedef struct owm_feels_like
{
  float   morn;             // Morning temperature.
  float   day;              // Day temperature.
  float   eve;              // Evening temperature.
  float   night;            // Night temperature.
} owm_owm_feels_like_t;

/*
 * Current weather data API response
 */
typedef struct owm_current
{
  int64_t dt;               // Current time, Unix, UTC
  int64_t sunrise;          // Sunrise time, Unix, UTC
  int64_t sunset;           // Sunset time, Unix, UTC
  float   temp;             // Temperature. Units - default: kelvin, metric: Celsius, imperial: Fahrenheit.
  float   feels_like;       // Temperature. This temperature parameter accounts for the human perception of weather. Units – default: kelvin, metric: Celsius, imperial: Fahrenheit.
  int     pressure;         // Atmospheric pressure on the sea level, hPa
  int     humidity;         // Humidity, %
  float   dew_point;        // Atmospheric temperature (varying according to pressure and humidity) below which water droplets begin to condense and dew can form. Units – default: kelvin, metric: Celsius, imperial: Fahrenheit.
  int     clouds;           // Cloudiness, %
  float   uvi;              // Current UV index
  int     visibility;       // Average visibility, metres. The maximum value of the visibility is 10km
  float   wind_speed;       // Wind speed. Wind speed. Units – default: metre/sec, metric: metre/sec, imperial: miles/hour.
  float   wind_gust;        // (where available) Wind gust. Units – default: metre/sec, metric: metre/sec, imperial: miles/hour.
  int     wind_deg;         // Wind direction, degrees (meteorological)
  float   rain_1h;          // (where available) Rain volume for last hour, mm
  float   snow_1h;          // (where available) Snow volume for last hour, mm
  owm_weather_t         weather;
} owm_current_t;

/*
 * Minute forecast weather data API response
 */
typedef struct owm_minutely
{
  int64_t dt;               // Time of the forecasted data, unix, UTC
  float   precipitation;    // Precipitation volume, mm
} owm_minutely_t;

/*
 * Hourly forecast weather data API response
 */
typedef struct owm_hourly
{
  int64_t dt;               // Time of the forecasted data, unix, UTC
  float   temp;             // Temperature. Units - default: kelvin, metric: Celsius, imperial: Fahrenheit.
  float   feels_like;       // Temperature. This temperature parameter accounts for the human perception of weather. Units – default: kelvin, metric: Celsius, imperial: Fahrenheit.
  int     pressure;         // Atmospheric pressure on the sea level, hPa
  int     humidity;         // Humidity, %
  float   dew_point;        // Atmospheric temperature (varying according to pressure and humidity) below which water droplets begin to condense and dew can form. Units – default: kelvin, metric: Celsius, imperial: Fahrenheit.
  int     clouds;           // Cloudiness, %
  float   uvi;              // Current UV index
  int     visibility;       // Average visibility, metres. The maximum value of the visibility is 10km
  float   wind_speed;       // Wind speed. Wind speed. Units – default: metre/sec, metric: metre/sec, imperial: miles/hour.
  float   wind_gust;        // (where available) Wind gust. Units – default: metre/sec, metric: metre/sec, imperial: miles/hour.
  int     wind_deg;         // Wind direction, degrees (meteorological)
  float   pop;              // Probability of precipitation. The values of the parameter vary between 0 and 1, where 0 is equal to 0%, 1 is equal to 100%
  float   rain_1h;          // (where available) Rain volume for last hour, mm
  float   snow_1h;          // (where available) Snow volume for last hour, mm
  owm_weather_t         weather;
} owm_hourly_t;

/*
 * Daily forecast weather data API response
 */
typedef struct owm_daily
{
  int64_t dt;               // Time of the forecasted data, unix, UTC
  int64_t sunrise;          // Sunrise time, Unix, UTC
  int64_t sunset;           // Sunset time, Unix, UTC
  int64_t moonrise;         // The time of when the moon rises for this day, Unix, UTC
  int64_t moonset;          // The time of when the moon sets for this day, Unix, UTC
  float   moon_phase;       // Moon phase. 0 and 1 are 'new moon', 0.25 is 'first quarter moon', 0.5 is 'full moon' and 0.75 is 'last quarter moon'. The periods in between are called 'waxing crescent', 'waxing gibous', 'waning gibous', and 'waning crescent', respectively.
  owm_temp_t            temp;
  owm_owm_feels_like_t  feels_like;
  int     pressure;         // Atmospheric pressure on the sea level, hPa
  int     humidity;         // Humidity, %
  float   dew_point;        // Atmospheric temperature (varying according to pressure and humidity) below which water droplets begin to condense and dew can form. Units – default: kelvin, metric: Celsius, imperial: Fahrenheit.
  int     clouds;           // Cloudiness, %
  float   uvi;              // Current UV index
  int     visibility;       // Average visibility, metres. The maximum value of the visibility is 10km
  float   wind_speed;       // Wind speed. Wind speed. Units – default: metre/sec, metric: metre/sec, imperial: miles/hour.
  float   wind_gust;        // (where available) Wind gust. Units – default: metre/sec, metric: metre/sec, imperial: miles/hour.
  int     wind_deg;         // Wind direction, degrees (meteorological)
  float   pop;              // Probability of precipitation. The values of the parameter vary between 0 and 1, where 0 is equal to 0%, 1 is equal to 100%
  float   rain;             // (where available) Precipitation volume, mm
  float   snow;             // (where available) Snow volume, mm
  owm_weather_t         weather;
} owm_daily_t;

/*
 * National weather alerts data from major national weather warning systems
 */
typedef struct owm_alerts
{
  String  sender_name;      // Name of the alert source.
  String  event;            // Alert event name
  int64_t start;            // Date and time of the start of the alert, Unix, UTC
  int64_t end;              // Date and time of the end of the alert, Unix, UTC
  String  description;      // Description of the alert
  String  tags;             // Type of severe weather
} owm_alerts_t;

/*
 * Response from OpenWeatherMap's OneCall API
 *
 * https://openweathermap.org/api/one-call-api
 */
typedef struct owm_resp_onecall
{
  float   lat;              // Geographical coordinates of the location (latitude)
  float   lon;              // Geographical coordinates of the location (longitude)
  String  timezone;         // Timezone name for the requested location
  int     timezone_offset;  // Shift in seconds from UTC
  owm_current_t   current;
  // owm_minutely_t  minutely[OWM_NUM_MINUTELY];

  owm_hourly_t    hourly[OWM_NUM_HOURLY];
  owm_daily_t     daily[OWM_NUM_DAILY];
  std::vector<owm_alerts_t> alerts;
} owm_resp_onecall_t;

/*
 * Coordinates from the specified location (latitude, longitude)
 */
typedef struct owm_coord
{
  float   lat;
  float   lon;
} owm_coord_t;

typedef struct owm_components
{
  float   co[OWM_NUM_AIR_POLLUTION];    // Сoncentration of CO (Carbon monoxide), μg/m^3
  float   no[OWM_NUM_AIR_POLLUTION];    // Сoncentration of NO (Nitrogen monoxide), μg/m^3
  float   no2[OWM_NUM_AIR_POLLUTION];   // Сoncentration of NO2 (Nitrogen dioxide), μg/m^3
  float   o3[OWM_NUM_AIR_POLLUTION];    // Сoncentration of O3 (Ozone), μg/m^3
  float   so2[OWM_NUM_AIR_POLLUTION];   // Сoncentration of SO2 (Sulphur dioxide), μg/m^3
  float   pm2_5[OWM_NUM_AIR_POLLUTION]; // Сoncentration of PM2.5 (Fine particles matter), μg/m^3
  float   pm10[OWM_NUM_AIR_POLLUTION];  // Сoncentration of PM10 (Coarse particulate matter), μg/m^3
  float   nh3[OWM_NUM_AIR_POLLUTION];   // Сoncentration of NH3 (Ammonia), μg/m^3
} owm_components_t;

/*
 * Response from OpenWeatherMap's Air Pollution API
 */
typedef struct owm_resp_air_pollution
{
  owm_coord_t      coord;
  int              main_aqi[OWM_NUM_AIR_POLLUTION];   // Air Quality Index. Possible values: 1, 2, 3, 4, 5. Where 1 = Good, 2 = Fair, 3 = Moderate, 4 = Poor, 5 = Very Poor.
  owm_components_t components;
  int64_t          dt[OWM_NUM_AIR_POLLUTION];         // Date and time, Unix, UTC;
} owm_resp_air_pollution_t;

DeserializationError deserializeOneCall(WiFiClient &json,
                                        owm_resp_onecall_t &r);
DeserializationError deserializeAirQuality(WiFiClient &json,
                                           owm_resp_air_pollution_t &r);


#endif

