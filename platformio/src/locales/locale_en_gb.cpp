/* British English locale settings for esp32-weather-epd.
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

#include "config.h"

#ifdef LOCALE_EN_GB

#include <vector>
#include <Arduino.h>
#include "_locale.h"

// TIME/DATE (NL_LANGINFO)
// locale-based information,
// see https://man7.org/linux/man-pages/man3/nl_langinfo.3.html for more info.
const char *LC_ABDAY[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char *LC_DAY[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", 
                         "Thursday", "Friday", "Saturday"};
const char *LC_ABMON[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", 
                            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const char *LC_MON[12] = {"January",    "February", "March",    "April", 
                           "May",       "June",     "July",     "August", 
                           "September", "October",  "November", "December"};
const char *LC_AM_PM[2] = {"AM", "PM"};
const char *LC_D_T_FMT    = "%a %d %b %Y %T";
const char *LC_D_FMT      = "%d/%m/%y";
const char *LC_T_FMT      = "%H:%M:%S";
const char *LC_T_FMT_AMPM = "%I:%M:%S %P";

// OWM LANGUAGE
// For full list of languages, see https://openweathermap.org/api/one-call-api#multi
// Note: "[only] The contents of the 'description' field will be translated."
const String OWM_LANG = "en";

// CURRENT CONDITIONS
const char *TXT_FEELS_LIKE         = "Feels Like";
const char *TXT_SUNRISE            = "Sunrise";
const char *TXT_SUNSET             = "Sunset";
const char *TXT_WIND               = "Wind";
const char *TXT_HUMIDITY           = "Humidity";
const char *TXT_UV_INDEX           = "UV Index";
const char *TXT_PRESSURE           = "Pressure";
const char *TXT_AIR_QUALITY_INDEX  = "Air Quality Index";
const char *TXT_VISIBILITY         = "Visibility";
const char *TXT_INDOOR_TEMPERATURE = "Temperature";
const char *TXT_INDOOR_HUMIDITY    = "Humidity";

// UV INDEX
const char *TXT_UV_LOW       = "Low";
const char *TXT_UV_MODERATE  = "Moderate";
const char *TXT_UV_HIGH      = "High";
const char *TXT_UV_VERY_HIGH = "Very High";
const char *TXT_UV_EXTREME   = "Extreme";

// WIFI
const char *TXT_WIFI_EXCELLENT     = "Excellent";
const char *TXT_WIFI_GOOD          = "Good";
const char *TXT_WIFI_FAIR          = "Fair";
const char *TXT_WIFI_WEAK          = "Weak";
const char *TXT_WIFI_NO_CONNECTION = "No Connection";

// UNIT SYMBOLS - TEMPERATURE
const char *TXT_UNITS_TEMP_KELVIN     = "`K";
const char *TXT_UNITS_TEMP_CELSIUS    = "`C";
const char *TXT_UNITS_TEMP_FAHRENHEIT = "`F";
// UNIT SYMBOLS - WIND SPEED
const char *TXT_UNITS_SPEED_METERSPERSECOND   = "m/s";
const char *TXT_UNITS_SPEED_FEETPERSECOND     = "ft/s";
const char *TXT_UNITS_SPEED_KILOMETERSPERHOUR = "km/h";
const char *TXT_UNITS_SPEED_MILESPERHOUR      = "mph";
const char *TXT_UNITS_SPEED_KNOTS             = "kt";
const char *TXT_UNITS_SPEED_BEAUFORT          = "";
// UNIT SYMBOLS - PRESSURE
const char *TXT_UNITS_PRES_HECTOPASCALS             = "hPa";
const char *TXT_UNITS_PRES_PASCALS                  = "Pa";
const char *TXT_UNITS_PRES_MILLIMETERSOFMERCURY     = "mmHg";
const char *TXT_UNITS_PRES_INCHESOFMERCURY          = "inHg";
const char *TXT_UNITS_PRES_MILLIBARS                = "mbar";
const char *TXT_UNITS_PRES_ATMOSPHERES              = "atm";
const char *TXT_UNITS_PRES_GRAMSPERSQUARECENTIMETER = "g/cm\xB2";
const char *TXT_UNITS_PRES_POUNDSPERSQUAREINCH      = "lb/in\xB2";
// UNITS - VISIBILITY DISTANCE
const char *TXT_UNITS_DIST_KILOMETERS = "km";
const char *TXT_UNITS_DIST_MILES      = "mi";

// LAST REFRESH
const char *TXT_UNKNOWN = "Unknown";

// ALERTS
// The display can show up to 2 alerts, but alerts can be unpredictible in
// severity and number. If more than 2 alerts are active, the esp32 will attempt
// to interpret the urgency of each alert and prefer to display the most urgent 
// and recently issued alerts of each event type. Depending on your region 
// different keywords are used to convey the level of urgency.
//
// A vector array is used to store these keywords. Urgency is ranked from low to
// high where the first index of the vector is the least urgent keyword and the 
// last index is the most urgent keyword. Expected as all lowercase.
//
// Here are a few examples, uncomment the array for your region (or create your 
// own).
// const std::vector<String> ALERT_URGENCY = {"statement", "watch", "advisory", "warning", "emergency"}; // US National Weather Service
const std::vector<String> ALERT_URGENCY = {"yellow", "amber", "red"};                 // United Kingdom's national weather service (MET Office)
// const std::vector<String> ALERT_URGENCY = {"minor", "moderate", "severe", "extreme"}; // METEO
// const std::vector<String> ALERT_URGENCY = {}; // Disable urgency interpretation (algorithm will fallback to only prefer the most recently issued alerts)

// ALERT TERMINOLOGY
// Weather terminology associated with each alert icon
const std::vector<String> TERM_SMOG =
    {"smog"};
const std::vector<String> TERM_SMOKE =
    {"smoke"};
const std::vector<String> TERM_FOG =
    {"fog", "haar"};
const std::vector<String> TERM_METEOR =
    {"meteor", "asteroid"};
const std::vector<String> TERM_NUCLEAR =
    {"nuclear", "ionizing radiation"};
const std::vector<String> TERM_BIOHAZARD =
    {"biohazard", "biological hazard"};
const std::vector<String> TERM_EARTHQUAKE =
    {"earthquake"};
const std::vector<String> TERM_TSUNAMI =
    {"tsunami"};
const std::vector<String> TERM_FIRE =
    {"fire", "red flag"};
const std::vector<String> TERM_HEAT =
    {"heat"};
const std::vector<String> TERM_WINTER =
    {"blizzard", "winter", "ice", "snow", "sleet", "cold", "freezing rain", 
     "wind chill", "freeze", "frost", "hail"};
const std::vector<String> TERM_LIGHTNING =
    {"thunderstorm", "storm cell", "pulse storm", "squall line", "supercell",
     "lightning"};
const std::vector<String> TERM_SANDSTORM =
    {"sandstorm", "blowing dust", "dust storm"};
const std::vector<String> TERM_FLOOD =
    {"flood", "storm surge", "seiche", "swell", "high seas", "high tides",
     "tidal surge"};
const std::vector<String> TERM_VOLCANO =
    {"volcanic", "ash", "volcano", "eruption"};
const std::vector<String> TERM_AIR_QUALITY =
    {"air", "stagnation", "pollution"};
const std::vector<String> TERM_TORNADO =
    {"tornado"};
const std::vector<String> TERM_SMALL_CRAFT_ADVISORY =
    {"small craft", "wind advisory"};
const std::vector<String> TERM_GALE_WARNING =
    {"gale"};
const std::vector<String> TERM_STORM_WARNING =
    {"storm warning"};
const std::vector<String> TERM_HURRICANE_WARNING =
    {"hurricane force wind", "extreme wind", "high wind"};
const std::vector<String> TERM_HURRICANE =
    {"hurricane", "tropical storm", "typhoon", "cyclone"};
const std::vector<String> TERM_DUST =
    {"dust", "sand"};
const std::vector<String> TERM_STRONG_WIND =
    {"wind"};

// AIR QUALITY INDEX
extern "C" {
const char *AUSTRALIA_AQI_TXT[6] =
{
  "Very Good",
  "Good",
  "Fair",
  "Poor",
  "Very Poor",
  "Hazardous",
};
const char *CANADA_AQHI_TXT[4] = 
{
  "Low",
  "Moderate",
  "High",
  "Very High",
};
const char *EUROPE_CAQI_TXT[5] = 
{
  "Very Low",
  "Low",
  "Medium",
  "High",
  "Very High",
};
const char *HONG_KONG_AQHI_TXT[5] = 
{
  "Low",
  "Moderate",
  "High",
  "Very High",
  "Hazardous",
};
const char *INDIA_AQI_TXT[6] = 
{
  "Good",
  "Satisfactory",
  "Moderate",
  "Poor",
  "Very Poor",
  "Severe",
};
const char *MAINLAND_CHINA_AQI_TXT[6] = 
{
  "Excellent",
  "Good",
  "Lightly Polluted",
  "Moderately Polluted",
  "Heavily Polluted",
  "Severely Polluted",
};
const char *SINGAPORE_PSI_TXT[5] = 
{
  "Good",
  "Moderate",
  "Unhealthy",
  "Very Unhealthy",
  "Hazardous",
};
const char *SOUTH_KOREA_CAI_TXT[4] = 
{
  "Good",
  "Medium",
  "Unhealthy",
  "Very Unhealthy",
};
const char *UNITED_KINGDOM_DAQI_TXT[4] = 
{
  "Low",
  "Moderate",
  "High",
  "Very High",
};
const char *UNITED_STATES_AQI_TXT[6] = 
{
  "Good",
  "Moderate",
  "Unhealthy for Sensitive Groups",
  "Unhealthy",
  "Very Unhealthy",
  "Hazardous",
};
} // end extern "C"

#endif
