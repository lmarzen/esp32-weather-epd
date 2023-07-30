/* Configuration option declarations for esp32-weather-epd.
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

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <cstdint>
#include <Arduino.h>

// E-PAPER PANEL
// This project currently supports the following E-Paper panels:
//   DISP_BW - WaveShare 800x480, 7.5inch E-Ink display, Black/White
//             https://www.waveshare.com/product/7.5inch-e-paper-hat.htm
//   DISP_3C - Waveshare 800x480, 7.5inch E-Ink display, Red/Black/White
//             https://www.waveshare.com/product/7.5inch-e-paper-hat-b.htm
// Uncomment the macro that identifies your physical panel.
#define DISP_BW
// #define DISP_3C

// 3 COLOR E-INK ACCENT COLOR
// Defines the 3rd color to be used when a 3 color display is selected.
#ifdef DISP_3C
  // #define ACCENT_COLOR GxEPD_BLACK
  #define ACCENT_COLOR GxEPD_RED
#endif

// LOCALE
// If your locale is not here, you can add it by copying and modifying one of
// the files in src/locales. Please feel free to create a pull request to add
// official support for your locale.
//   Language (Territory)            code
//   German (Germany)                de_DE
//   English (United Kingdom)        en_GB
//   English (United States)         en_US
//   French (France)                 fr_FR
//   Dutch (Belgium)                 nl_BE
#define LOCALE en_US

// UNITS
// Define exactly one macro for each unit below.

// UNITS - TEMPERATURE
//   Metric   : Celsius
//   Imperial : Fahrenheit
// #define UNITS_TEMP_KELVIN
// #define UNITS_TEMP_CELSIUS
#define UNITS_TEMP_FAHRENHEIT

// UNITS - WIND SPEED
//   Metric   : Kilometers per Hour
//   Imperial : Miles per Hour
// #define UNITS_SPEED_METERSPERSECOND
// #define UNITS_SPEED_FEETPERSECOND
// #define UNITS_SPEED_KILOMETERSPERHOUR
#define UNITS_SPEED_MILESPERHOUR
// #define UNITS_SPEED_KNOTS
// #define UNITS_SPEED_BEAUFORT

// UNITS - PRESSURE
//   Metric   : Millibars
//   Imperial : Inches of Mercury
// #define UNITS_PRES_HECTOPASCALS
// #define UNITS_PRES_PASCALS
// #define UNITS_PRES_MILLIMETERSOFMERCURY
#define UNITS_PRES_INCHESOFMERCURY
// #define UNITS_PRES_MILLIBARS
// #define UNITS_PRES_ATMOSPHERES
// #define UNITS_PRES_GRAMSPERSQUARECENTIMETER
// #define UNITS_PRES_POUNDSPERSQUAREINCH

// UNITS - VISIBILITY DISTANCE
//   Metric   : Kilometers
//   Imperial : Miles
// #define UNITS_DIST_KILOMETERS
#define UNITS_DIST_MILES

// AIR QUALITY INDEX
// Seemingly every country uses a different scale for Air Quality Index (AQI).
// I have written a library to calculate many of the most popular AQI scales.
// Feel free to request the addition of a new AQI scale by opening an Issue.
// https://github.com/lmarzen/pollutant-concentration-to-aqi
// Define your preferred AQI scale.
//   AUSTRALIA_AQI        CANADA_AQHI          EUROPE_CAQI
//   HONG_KONG_AQHI       INDIA_AQI            MAINLAND_CHINA_AQI
//   SINGAPORE_PSI        SOUTH_KOREA_CAI      UNITED_KINGDOM_DAQI
//   UNITED_STATES_AQI
#define UNITED_STATES_AQI

// Hypertext Transfer Protocol (HTTP)
// HTTP
//   HTTP does not provide encryption or any security measures, making it highly
//   vulnerable to eavesdropping and data tampering. Has the advantage of using
//   less power.
// HTTPS_NO_CERT_VERIF
//   HTTPS without X.509 certificate verification provides encryption but lacks
//   authentication and is susceptible to man-in-the-middle attacks.
// HTTPS_WITH_CERT_VERIF
//   HTTPS with X.509 certificate verification offers the highest level of
//   security by providing encryption and verifying the identity of the server.
//
//   HTTPS with X.509 certificate verification comes with the draw back that
//   eventually the certificates on the esp32 will expire, requiring you to
//   update the certificates in cert.h and reflash this software.
//   Running cert.py will generate an updated cert.h file.
//   The current certificate for api.openweathermap.org is valid until
//   2030-12-31 23:59:59.
// (uncomment exactly one)
// #define USE_HTTP
#define USE_HTTPS_NO_CERT_VERIF
// #define USE_HTTPS_WITH_CERT_VERIF

// WIND ICON PRECISION
// The wind direction icon shown to the left of the wind speed can indicate wind
// direction with a minimum error of ±0.5°. This uses more flash storage because
// 360 24x24 wind direction icons must be stored, totaling ~25kB. For either
// preference or in case flash space becomes a concern there are a handful of
// selectable options listed below. 360 points seems excessive, but the option
// is there.
//
//   DIRECTIONS                 #     ERROR  STORAGE
//   Cardinal                   4  ±45.000°     288B  E
//   Ordinal                    8  ±22.500°     576B  NE
//   Secondary Intercardinal   16  ±11.250°   1,152B  NNE
//   Tertiary Intercardinal    32   ±5.625°   2,304B  NbE
//   (360)                    360   ±0.500°  25,920B  1°
// Uncomment your preferred wind level direction precision.
// #define WIND_DIRECTIONS_CARDINAL
// #define WIND_DIRECTIONS_ORDINAL
#define WIND_DIRECTIONS_SECONDARY_INTERCARDINAL
// #define WIND_DIRECTIONS_TERTIARY_INTERCARDINAL
// #define WIND_DIRECTIONS_360

// FONTS
// A handful of popular Open Source typefaces have been included with this
// project for your convenience. Change the font by selecting its corresponding
// header file.
//
//   FONT           HEADER FILE                    FAMILY          LICENSE
//   FreeMono       "fonts/FreeMono.h"             GNU FreeFont    GNU GPL v3.0
//   FreeSans       "fonts/FreeSans.h"             GNU FreeFont    GNU GPL v3.0
//   FreeSerif      "fonts/FreeSerif.h"            GNU FreeFont    GNU GPL v3.0
//   Lato           "fonts/Lato_Regular.h          Lato            SIL OFL 1.1
//   Montserrat     "fonts/Montserrat_Regular.h    Montserrat      SIL OFL 1.1
//   Open Sans      "fonts/OpenSans_Regular.h      Open Sans       SIL OFL 1.1
//   Poppins        "fonts/Poppins_Regular.h       Poppins         SIL OFL 1.1
//   Quicksand      "fonts/Quicksand_Regular.h     Quicksand       SIL OFL 1.1
//   Raleway        "fonts/Raleway_Regular.h       Raleway         SIL OFL 1.1
//   Roboto         "fonts/Roboto_Regular.h        Roboto          Apache v2.0
//   Roboto Mono    "fonts/RobotoMono_Regular.h    Roboto Mono     Apache v2.0
//   Roboto Slab    "fonts/RobotoSlab_Regular.h    Roboto Slab     Apache v2.0
//   Ubuntu         "fonts/Ubuntu_R.h              Ubuntu font     UFL v1.0
//   Ubuntu Mono    "fonts/UbuntuMono_R.h          Ubuntu font     UFL v1.0
//
// Adding new fonts is relatively straightforward, see fonts/README.
//
// Note:
//   The layout of the display was designed around spacing and size of the GNU
//   FreeSans font, but this project supports the ability to modularly swap
//   fonts. Using a font other than FreeSans may result in undesired spacing or
//   other artifacts.
#define FONT_HEADER "fonts/FreeSans.h"

// DISABLE ALERTS
//   The handling of alerts is complex. Each country has a unique national alert
//   system that receives alerts from many different government agencies. This
//   results is huge variance in the formatting of alerts. OpenWeatherMap
//   provides alerts in English only. Any combination of these factors may make
//   it undesirable to display alerts in some regions.
//   Disable alerts by defining the DISABLE_ALERTS macro.
// #define DISABLE_ALERTS

// Set the below constants in "config.cpp"
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
extern const uint8_t PIN_CONFIGURE_WIFI;
extern const char *WIFI_SSID;
extern const char *WIFI_PASSWORD;
extern const char *WIFI_AP_SSID;
extern const String OWM_APIKEY;
extern const String OWM_ENDPOINT;
extern const String OWM_ONECALL_VERSION;
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
extern const int BED_TIME;
extern const int WAKE_TIME;
extern const char UNITS;
extern const int HOURLY_GRAPH_MAX;
extern const float BATTERY_WARN_VOLTAGE;
extern const float LOW_BATTERY_VOLTAGE;
extern const float VERY_LOW_BATTERY_VOLTAGE;
extern const float CRIT_LOW_BATTERY_VOLTAGE;
extern const unsigned long LOW_BATTERY_SLEEP_INTERVAL;
extern const unsigned long VERY_LOW_BATTERY_SLEEP_INTERVAL;

#endif

