/* Belgian Dutch locale data for esp32-weather-epd.
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

/* Special thanks to Juan Rial (jrial) for contributing this translation.
 *
 * If you have any feedback or suggestions, please open an issue on this
 * project's github page: <https://github.com/lmarzen/esp32-weather-epd/issues>
 */

#include <vector>
#include <Arduino.h>
#include "_locale.h"

// LC_TIME
// locale-based information,
// see https://man7.org/linux/man-pages/man3/nl_langinfo.3.html for more info.
// Note to Translators:
//   The LC_TIME definitions are included in the localedata/locales directory of
//   the glibc source tree, in files named after the locale codes and encoding
//   schemes. For example, the en_US locale data is stored in the
//   localedata/locales/en_US file, which contains the definitions for date and
//   time formats, month and day names, and other time-related settings for the
//   LC_TIME category.
//   https://www.gnu.org/software/libc/sources.html
// D_T_FMT                 string for formatting date and time
const char *LC_D_T_FMT     = "%a %d %b %Y %T";
// LC_D_FMT                date format string
const char *LC_D_FMT       = "%d-%m-%y";
// T_FMT                   time format string
const char *LC_T_FMT       = "%T";
// T_FMT_AMPM              a.m. or p.m. time format string
const char *LC_T_FMT_AMPM  = "";
// AM_STR                  Ante Meridian affix
const char *LC_AM_STR      = "";
// PM_STR                  Post Meridian affix
const char *LC_PM_STR      = "";
// DAY_{1-7}               name of the n-th day of the week
const char *LC_DAY[7]      = {"Zondag", "Maandag", "Dinsdag", "Woensdag",
                              "Donderdag", "Vrijdag", "Zaterdag"};
// ABDAY_{1-7}             abbreviated name of the n-th day of the week
const char *LC_ABDAY[7]    = {"Zo", "Ma", "Di", "Wo", "Do", "Vr", "Za"};
// MON_{1-12}              name of the n-th month of the year
const char *LC_MON[12]     = {"Januari",   "Februari", "Maart",    "April",
                              "Mei",       "Juni",     "Juli",     "Augustus",
                              "September", "Oktober",  "November", "December"};
// ABMON_{1-12}            abbreviated name of the n-th month of the year
const char *LC_ABMON[12]   = {"Jan", "Feb", "Mrt", "Apr", "Mei", "Jun",
                              "Jul", "Aug", "Sep", "Okt", "Nov", "Dec"};
// ERA                     era description segments
const char *LC_ERA         = "";
// ERA_D_FMT               era date format string
const char *LC_ERA_D_FMT   = "";
// ERA_D_T_FMT             era date and time format string
const char *LC_ERA_D_T_FMT = "";
// ERA_T_FMT               era time format string
const char *LC_ERA_T_FMT   = "";

// OWM LANGUAGE
// For full list of languages, see
// https://openweathermap.org/api/one-call-api#multi
// Note: "[only] The contents of the 'description' field will be translated."
const String OWM_LANG = "nl";

// CURRENT CONDITIONS
const char *TXT_FEELS_LIKE         = "Voelt als";
const char *TXT_SUNRISE            = "Zonsopgang";
const char *TXT_SUNSET             = "Zonsondergang";
const char *TXT_WIND               = "Wind";
const char *TXT_HUMIDITY           = "Vochtigheid";
const char *TXT_UV_INDEX           = "UV Index";
const char *TXT_PRESSURE           = "Luchtdruk";
const char *TXT_AIR_QUALITY_INDEX  = "Luchtvervuiling";
const char *TXT_VISIBILITY         = "Zichtbaarheid";
const char *TXT_INDOOR_TEMPERATURE = "Temperatuur";
const char *TXT_INDOOR_HUMIDITY    = "Vochtigheid";

// UV INDEX
const char *TXT_UV_LOW       = "Laag";
const char *TXT_UV_MODERATE  = "Gematigd";
const char *TXT_UV_HIGH      = "Hoog";
const char *TXT_UV_VERY_HIGH = "Zeer Hoog";
const char *TXT_UV_EXTREME   = "Extreem";

// WIFI
const char *TXT_WIFI_EXCELLENT     = "Uitstekend";
const char *TXT_WIFI_GOOD          = "Goed";
const char *TXT_WIFI_FAIR          = "Redelijk";
const char *TXT_WIFI_WEAK          = "Zwak";
const char *TXT_WIFI_NO_CONNECTION = "Geen Verbinding";

// UNIT SYMBOLS - TEMPERATURE
const char *TXT_UNITS_TEMP_KELVIN     = "K";
const char *TXT_UNITS_TEMP_CELSIUS    = "\xB0""C";
const char *TXT_UNITS_TEMP_FAHRENHEIT = "\xB0""F";
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
const char *TXT_UNKNOWN = "Onbekend";

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
// Note to Translators:
//   OpenWeatherMap returns alerts in English regardless of the OWM LANGUAGE
//   option or territory. For this reason it is preferred that you do not
//   translate text related to Alerts. Understandably, it may be undesirable to
//   see English alerts in territories where English is uncommon. To satisify
//   this, users should set the DISPLAY_ALERTS macro to 0 in config.h.
//
// Here are a few examples, uncomment the array for your region (or create your
// own).
// const std::vector<String> ALERT_URGENCY = {"statement", "watch", "advisory", "warning", "emergency"}; // US National Weather Service
// const std::vector<String> ALERT_URGENCY = {"yellow", "amber", "red"};                 // United Kingdom's national weather service (MET Office)
const std::vector<String> ALERT_URGENCY = {"minor", "moderate", "severe", "extreme"}; // METEO
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
  "Zeer Goed",
  "Goed",
  "Redelijk",
  "Slecht",
  "Zeer Slecht",
  "Gevaarlijk",
};
const char *CANADA_AQHI_TXT[4] =
{
  "Laag",
  "Gematigd",
  "Hoog",
  "Zeer Hoog",
};
const char *EUROPE_CAQI_TXT[5] =
{
  "Zeer Laag",
  "Laag",
  "Middelmatig",
  "Hoog",
  "Zeer Hoog",
};
const char *HONG_KONG_AQHI_TXT[5] =
{
  "Laag",
  "Gematigd",
  "Hoog",
  "Zeer Hoog",
  "Gevaarlijk",
};
const char *INDIA_AQI_TXT[6] =
{
  "Goed",
  "Bevredigend",
  "Gematigd",
  "Slecht",
  "Zeer Slecht",
  "Ernstig",
};
const char *MAINLAND_CHINA_AQI_TXT[6] =
{
  "Uitstekend",
  "Goed",
  "Licht Vervuild",
  "Middelmatig Vervuild",
  "Zwaar Vervuild",
  "Ernstig Vervuild",
};
const char *SINGAPORE_PSI_TXT[5] =
{
  "Goed",
  "Gematigd",
  "Ongezond",
  "Zeer Ongezond",
  "Gevaarlijk",
};
const char *SOUTH_KOREA_CAI_TXT[4] =
{
  "Goed",
  "Middelmatig",
  "Ongezond",
  "Zeer Ongezond",
};
const char *UNITED_KINGDOM_DAQI_TXT[4] =
{
  "Laag",
  "Gematigd",
  "Hoog",
  "Zeer Hoog",
};
const char *UNITED_STATES_AQI_TXT[6] =
{
  "Goed",
  "Gematigd",
  "Ongezond voor Gevoelige Groepen",
  "Ongezond",
  "Zeer Ongezond",
  "Gevaarlijk",
};
} // end extern "C"

