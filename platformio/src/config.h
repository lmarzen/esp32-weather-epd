#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <cstdint>
#include <vector>
#include <Arduino.h>

// LANGUAGE
// language header to use
#define LANGUAGE_HEADER "languages/lang_en_us.h"

// UNITS
// Select either metric or imperial units
// https://openweathermap.org/api/one-call-api#data
// Uncomment your preferred units. (exactly 1 must be defined)
// #define UNITS_METRIC
#define UNITS_IMPERIAL

// AIR QUALITY INDEX
// Seemingly every country uses a different scale for Air Quality Index (AQI).
// I have written a library to calculate many of the most popular AQI scales.
// Feel free to request the addition of a new AQI scale by opening an Issue.
// https://github.com/lmarzen/pollutant-concentration-to-aqi
// Uncomment your preferred AQI scale. (exactly 1 must be defined)
// #define AUSTRALIA_AQI
// #define CANADA_AQHI
// #define EUROPE_CAQI
// #define HONG_KONG_AQHI
// #define INDIA_AQI
// #define MAINLAND_CHINA_AQI
// #define SINGAPORE_PSI
// #define SOUTH_KOREA_CAI
// #define UNITED_KINGDOM_DAQI
#define UNITED_STATES_AQI

// WIND
// The wind direction icon shown to the left of the wind speed can indicate wind
// direction with a minimum error of ±0.5°. This uses more flash storage because
// 360 24x24 wind direction icons must be stored, totaling ~25kB. For either
// preference or incase flash space becomes a concern there are a handful of
// selectable options listed below. 360 points seems excessive, but the option
// is there.
//
// DIRECTIONS                 #     ERROR  STORAGE  
// Cardinal                   4  ±45.000°     288B  N
// Ordinal                    8  ±22.500°     576B  NE
// Secondary Intercardinal   16  ±11.250°   1,152B  NNE
// Tertiary Intercardinal    32   ±5.625°   2,304B  NbE
// (360)                    360   ±0.500°  25,920B  1°
// Uncomment your preferred wind level direction precision.
// (exactly 1 must be defined)
// #define WIND_DIRECTIONS_CARDINAL
// #define WIND_DIRECTIONS_ORDINAL
#define WIND_DIRECTIONS_SECONDARY_INTERCARDINAL
// #define WIND_DIRECTIONS_TERTIARY_INTERCARDINAL
// #define WIND_DIRECTIONS_360

// Set the below variables in "config.cpp"
extern const uint8_t PIN_BAT_ADC;
extern const uint8_t PIN_EPD_BUSY;
extern const uint8_t PIN_EPD_CS;
extern const uint8_t PIN_EPD_RST;
extern const uint8_t PIN_EPD_DC;
extern const uint8_t PIN_EPD_SCK;
extern const uint8_t PIN_EPD_MISO;
extern const uint8_t PIN_EPD_MOSI;
extern const uint8_t PIN_BME_SDA;
extern const uint8_t PIN_BME_SCL;
extern const uint8_t BME_ADDRESS;
extern const char *WIFI_SSID;
extern const char *WIFI_PASSWORD;
extern const String OWM_APIKEY;
extern const String OWM_ENDPOINT;
extern const String LAT;
extern const String LON;
extern const String CITY_STRING;
extern const char *TIMEZONE;
extern const char *TIME_FORMAT;
extern const char *HOUR_FORMAT;
extern const char *DATE_FORMAT;
extern const char *REFRESH_TIME_FORMAT;
extern const char *NTP_SERVER_1;
extern const char *NTP_SERVER_2;
extern const long SLEEP_DURATION;
extern const int WAKE_TIME;
extern const int BED_TIME;
extern const char UNITS;
extern const int HOURLY_GRAPH_MAX;
extern const float LOW_BATTERY_VOLTAGE;
extern const float CRIT_LOW_BATTERY_VOLTAGE;
extern const unsigned long LOW_BATTERY_SLEEP_INTERVAL;
extern const unsigned long CRIT_LOW_BATTERY_SLEEP_INTERVAL;

#endif
