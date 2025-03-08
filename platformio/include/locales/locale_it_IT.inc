/* Italian locale data for esp32-weather-epd.
 * Copyright (C) 2022-2024  Luke Marzen
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

/* Special thanks to Massimo Saccani (Supergiovane) for contributing this
 * translation.
 *
 * If you have any feedback or suggestions on translations, please open an issue
 * on this project's github page:
 * <https://github.com/lmarzen/esp32-weather-epd/issues>
 */

#include "_locale.h"
#include <vector>
#include <Arduino.h>

// LC_TIME
// locale-based information,
// see https://man7.org/linux/man-pages/man3/nl_langinfo.3.html for more info.
// Note to Translators:
//   The LC_TIME definitions are included in the localedata/locales directory of
//   the glibc source tree, in files named after the locale codes and encoding
//   schemes. For example, the fr_FR locale data is stored in the
//   localedata/locales/fr_FR file, which contains the definitions for date and
//   time formats, month and day names, and other time-related settings for the
//   LC_TIME category.
//   https://www.gnu.org/software/libc/sources.html
// D_T_FMT                 string for formatting date and time
const char *LC_D_T_FMT     = "%a %-d %b %Y, %T";
// LC_D_FMT                date format string
const char *LC_D_FMT       = "%d/%m/%y";
// T_FMT                   time format string
const char *LC_T_FMT       = "%T";
// T_FMT_AMPM              a.m. or p.m. time format string
const char *LC_T_FMT_AMPM  = "";
// AM_STR                  Ante Meridian affix
const char *LC_AM_STR      = "";
// PM_STR                  Post Meridian affix
const char *LC_PM_STR      = "";
// DAY_{1-7}               name of the n-th day of the week
const char *LC_DAY[7]      = {"domenica",   "luned\354",  "marted\354", "mercoled\354", 
                              "gioved\354", "venerd\354", "sabato"};
// ABDAY_{1-7}             abbreviated ame of the n-th day of the week
const char *LC_ABDAY[7]    = {"dom", "lun", "mar", "mer", "gio", "ven", "sab"};
// MON_{1-12}              name of the n-th month of the year
const char *LC_MON[12]     = {"gennaio",   "febbraio", "marzo",    "aprile", 
                              "maggio",    "giugno",   "luglio",   "agosto", 
                              "settembre", "ottobre",  "novembre", "dicembre"};
// ABMON_{1-12}            abbreviated name of the n-th month of the year
const char *LC_ABMON[12]   = {"gen", "feb", "mar", "apr", "mag", "giu", 
                              "lug", "ago", "set", "ott", "nov", "dic"};
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
const String OWM_LANG = "it";

// CURRENT CONDITIONS
const char *TXT_FEELS_LIKE         = "Temp. percepita";
const char *TXT_SUNRISE            = "Alba";
const char *TXT_SUNSET             = "Tramonto";
const char *TXT_WIND               = "Vento";
const char *TXT_HUMIDITY           = "Umidita'";
const char *TXT_UV_INDEX           = "Indice UV";
const char *TXT_PRESSURE           = "Pressionr";
const char *TXT_AIR_QUALITY        = "Qualita' dell'aria";
const char *TXT_AIR_POLLUTION      = "Inquinamento aria";
const char *TXT_VISIBILITY         = "Visibilita'";
const char *TXT_INDOOR_TEMPERATURE = "Temperatura int.";
const char *TXT_INDOOR_HUMIDITY    = "Umidita' int.";

// UV INDEX
const char *TXT_UV_LOW       = "Basso";
const char *TXT_UV_MODERATE  = "Moderato";
const char *TXT_UV_HIGH      = "Alto";
const char *TXT_UV_VERY_HIGH = "Eccessivo";
const char *TXT_UV_EXTREME   = "Estremo";

// WIFI
const char *TXT_WIFI_EXCELLENT     = "Eccellente";
const char *TXT_WIFI_GOOD          = "Buono";
const char *TXT_WIFI_FAIR          = "Medio";
const char *TXT_WIFI_WEAK          = "Scarso";
const char *TXT_WIFI_NO_CONNECTION = "Disconnesso";

// UNIT SYMBOLS - TEMPERATURE
const char *TXT_UNITS_TEMP_KELVIN     = "K";
const char *TXT_UNITS_TEMP_CELSIUS    = "\260C";
const char *TXT_UNITS_TEMP_FAHRENHEIT = "\260F";
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
const char *TXT_UNITS_PRES_GRAMSPERSQUARECENTIMETER = "g/cm\262";
const char *TXT_UNITS_PRES_POUNDSPERSQUAREINCH      = "lb/in\262";
// UNITS SYMBOLS - VISIBILITY DISTANCE
const char *TXT_UNITS_DIST_KILOMETERS = "km";
const char *TXT_UNITS_DIST_MILES      = "mi";
// UNITS SYMBOLS - PRECIPITATION
const char *TXT_UNITS_PRECIP_MILLIMETERS = "mm";
const char *TXT_UNITS_PRECIP_CENTIMETERS = "cm";
const char *TXT_UNITS_PRECIP_INCHES      = "in";

// MISCELLANEOUS MESSAGES
// Title Case
const char *TXT_LOW_BATTERY = "Low Battery";
const char *TXT_NETWORK_NOT_AVAILABLE = "Network Not Available";
const char *TXT_TIME_SYNCHRONIZATION_FAILED = "Time Synchronization Failed";
const char *TXT_WIFI_CONNECTION_FAILED = "WiFi Connection Failed";
// First Word Capitalized
const char *TXT_ATTEMPTING_HTTP_REQ = "Attempting HTTP request";
const char *TXT_AWAKE_FOR = "Awake for";
const char *TXT_BATTERY_VOLTAGE = "Battery voltage";
const char *TXT_CONNECTING_TO = "Connecting to";
const char *TXT_COULD_NOT_CONNECT_TO = "Could not connect to";
const char *TXT_ENTERING_DEEP_SLEEP_FOR = "Entering deep sleep for";
const char *TXT_READING_FROM = "Reading from";
const char *TXT_FAILED  = "Failed";
const char *TXT_SUCCESS = "Success";
const char *TXT_UNKNOWN = "Unknown";
// All Lowercase
const char *TXT_NOT_FOUND = "not found";
const char *TXT_READ_FAILED = "read failed";
// Complete Sentences
const char *TXT_FAILED_TO_GET_TIME = "Failed to get the time!";
const char *TXT_HIBERNATING_INDEFINITELY_NOTICE = "Hibernating without wake time!";
const char *TXT_REFERENCING_OLDER_TIME_NOTICE = "Failed to synchronize time before deep-sleep, referencing older time.";
const char *TXT_WAITING_FOR_SNTP = "Waiting for SNTP synchronization.";
const char *TXT_LOW_BATTERY_VOLTAGE = "Low battery voltage!";
const char *TXT_VERY_LOW_BATTERY_VOLTAGE = "Very low battery voltage!";
const char *TXT_CRIT_LOW_BATTERY_VOLTAGE = "Critically low battery voltage!";

// ALERTS
// The display can show up to 2 alerts, but alerts can be unpredictable in
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
//   see English alerts in territories where English is uncommon. To satisfy
//   this, users should set the DISPLAY_ALERTS macro to 0 in config.h.
//
// Here are a few examples, uncomment the array for your region (or create your
// own).
// const std::vector<String> ALERT_URGENCY = {"outlook", "statement", "watch", "advisory", "warning", "emergency"}; // US National Weather Service
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
const std::vector<String> TERM_FIRE =
    {"fire", "red flag"};
const std::vector<String> TERM_HEAT =
    {"heat"};
const std::vector<String> TERM_WINTER =
    {"blizzard", "winter", "ice", "icy", "snow", "sleet", "cold",
     "freezing rain", "wind chill", "freeze", "frost", "hail"};
const std::vector<String> TERM_TSUNAMI =
    {"tsunami", "surf"};
const std::vector<String> TERM_LIGHTNING =
    {"thunderstorm", "storm cell", "pulse storm", "squall line", "supercell",
     "lightning"};
const std::vector<String> TERM_SANDSTORM =
    {"sandstorm", "blowing dust", "dust storm"};
const std::vector<String> TERM_FLOOD =
    {"flood", "storm surge", "seiche", "swell", "high seas", "high tides",
     "tidal surge", "hydrologic"};
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
// Seemingly every country uses a different scale for Air Quality Index (AQI).
// I have written a library to calculate many of the most popular AQI scales.
// Feel free to request the addition of a new AQI scale by opening an Issue.
// https://github.com/lmarzen/pollutant-concentration-to-aqi
// Please choose this region's preferred AQI scale.
//   AUSTRALIA_AQI        CANADA_AQHI          EUROPEAN_UNION_CAQI
//   HONG_KONG_AQHI       INDIA_AQI            CHINA_AQI
//   SINGAPORE_PSI        SOUTH_KOREA_CAI      UNITED_KINGDOM_DAQI
//   UNITED_STATES_AQI
const aqi_scale_t AQI_SCALE = EUROPEAN_UNION_CAQI;

const char *AUSTRALIA_AQI_TXT[6] =
{
  "Tr\350s bon",
  "Bon",
  "Juste",
  "Mauvais",
  "Tr\350s mauvais",
  "Dangereux",
};
const char *CANADA_AQHI_TXT[4] =
{
  "Bas",
  "Mod\351r\351",
  "Haut",
  "Tr\350s haut",
};
const char *EUROPEAN_UNION_CAQI_TXT[5] =
{
  "Molto basso",
  "Baso",
  "Medio",
  "Alto",
  "Molto alto",
};
const char *HONG_KONG_AQHI_TXT[5] =
{
  "Bas",
  "Mod\351r\351",
  "Haut",
  "Tr\350s haut",
  "Dangereux",
};
const char *INDIA_AQI_TXT[6] =
{
  "Bon",
  "Satisfaisant",
  "Mod\351r\351",
  "Mauvais",
  "Tr\350s mauvais",
  "S\351v\350re",
};
const char *CHINA_AQI_TXT[6] =
{
  "Excellent",
  "Bon",
  "L\351g\350rement pollu\351",
  "Mod\351r\351ment pollu\351",
  "Tr\350s pollu\351",
  "S\351v\350rement pollu\351",
};
const char *SINGAPORE_PSI_TXT[5] =
{
  "Bon",
  "Mod\351r\351",
  "Malsain",
  "Tr\350s malsain",
  "Dangereux",
};
const char *SOUTH_KOREA_CAI_TXT[4] =
{
  "Bon",
  "Moyen",
  "Malsain",
  "Tr\350s malsain",
};
const char *UNITED_KINGDOM_DAQI_TXT[4] =
{
  "Bas",
  "Mod\351r\351",
  "Haut",
  "Tr\350s haut",
};
const char *UNITED_STATES_AQI_TXT[6] =
{
  "Bon",
  "Mod\351r\351",
  "Malsain pour les groupes sensibles",
  "Malsain",
  "Tr\350s malsain",
  "Dangereux",
};
} // end extern "C"

// COMPASS POINT
const char *COMPASS_POINT_NOTATION[32] = {
//   0°   11.25°   22.5°   33.75°   45°   56.25°   67.5°   78.75°
   "N",  "NbE",  "NNE",   "NEbN",  "NE", "NEbE",  "ENE",  "EbN",
//  90°  101.25°  112.5°  123.75°  135°  146.25°  157.5°  168.75°
   "E",  "EbS",   "ESE",  "SEbE",  "SE", "SEbS",  "SSE",  "SbE",
// 180°  191.25°  202.5°  213.75°  225°  236.25°  247.5°  258.75°
   "S",  "SbO",   "SSO",  "SObS",  "SO", "SObO",  "OSO",  "ObS",
// 270°  281.25°  292.5°  303.75°  315°  326.25°  337.5°  348.75°
   "O",  "ObN",   "ONO",  "NObO",  "NO", "NObN",  "NNO",  "NbO",
};

// HTTP CLIENT ERRORS
const char *TXT_HTTPC_ERROR_CONNECTION_REFUSED   = "Connection Refused";
const char *TXT_HTTPC_ERROR_SEND_HEADER_FAILED   = "Send Header Failed";
const char *TXT_HTTPC_ERROR_SEND_PAYLOAD_FAILED  = "Send Payload Failed";
const char *TXT_HTTPC_ERROR_NOT_CONNECTED        = "Not Connected";
const char *TXT_HTTPC_ERROR_CONNECTION_LOST      = "Connection Lost";
const char *TXT_HTTPC_ERROR_NO_STREAM            = "No Stream";
const char *TXT_HTTPC_ERROR_NO_HTTP_SERVER       = "No HTTP Server";
const char *TXT_HTTPC_ERROR_TOO_LESS_RAM         = "Too Less Ram";
const char *TXT_HTTPC_ERROR_ENCODING             = "Transfer-Encoding Not Supported";
const char *TXT_HTTPC_ERROR_STREAM_WRITE         = "Stream Write Error";
const char *TXT_HTTPC_ERROR_READ_TIMEOUT         = "Read Timeout";

// HTTP RESPONSE STATUS CODES
// 1xx - Informational Responses
const char *TXT_HTTP_RESPONSE_100 = "Continue";
const char *TXT_HTTP_RESPONSE_101 = "Switching Protocols";
const char *TXT_HTTP_RESPONSE_102 = "Processing";
const char *TXT_HTTP_RESPONSE_103 = "Early Hints";
// 2xx - Successful Responses
const char *TXT_HTTP_RESPONSE_200 = "OK";
const char *TXT_HTTP_RESPONSE_201 = "Created";
const char *TXT_HTTP_RESPONSE_202 = "Accepted";
const char *TXT_HTTP_RESPONSE_203 = "Non-Authoritative Information";
const char *TXT_HTTP_RESPONSE_204 = "No Content";
const char *TXT_HTTP_RESPONSE_205 = "Reset Content";
const char *TXT_HTTP_RESPONSE_206 = "Partial Content";
const char *TXT_HTTP_RESPONSE_207 = "Multi-Status";
const char *TXT_HTTP_RESPONSE_208 = "Already Reported";
const char *TXT_HTTP_RESPONSE_226 = "IM Used";
// 3xx - Redirection Responses
const char *TXT_HTTP_RESPONSE_300 = "Multiple Choices";
const char *TXT_HTTP_RESPONSE_301 = "Moved Permanently";
const char *TXT_HTTP_RESPONSE_302 = "Found";
const char *TXT_HTTP_RESPONSE_303 = "See Other";
const char *TXT_HTTP_RESPONSE_304 = "Not Modified";
const char *TXT_HTTP_RESPONSE_305 = "Use Proxy";
const char *TXT_HTTP_RESPONSE_307 = "Temporary Redirect";
const char *TXT_HTTP_RESPONSE_308 = "Permanent Redirect";
// 4xx - Client Error Responses
const char *TXT_HTTP_RESPONSE_400 = "Bad Request";
const char *TXT_HTTP_RESPONSE_401 = "Unauthorized";
const char *TXT_HTTP_RESPONSE_402 = "Payment Required";
const char *TXT_HTTP_RESPONSE_403 = "Forbidden";
const char *TXT_HTTP_RESPONSE_404 = "Not Found";
const char *TXT_HTTP_RESPONSE_405 = "Method Not Allowed";
const char *TXT_HTTP_RESPONSE_406 = "Not Acceptable";
const char *TXT_HTTP_RESPONSE_407 = "Proxy Authentication Required";
const char *TXT_HTTP_RESPONSE_408 = "Request Timeout";
const char *TXT_HTTP_RESPONSE_409 = "Conflict";
const char *TXT_HTTP_RESPONSE_410 = "Gone";
const char *TXT_HTTP_RESPONSE_411 = "Length Required";
const char *TXT_HTTP_RESPONSE_412 = "Precondition Failed";
const char *TXT_HTTP_RESPONSE_413 = "Content Too Large";
const char *TXT_HTTP_RESPONSE_414 = "URI Too Long";
const char *TXT_HTTP_RESPONSE_415 = "Unsupported Media Type";
const char *TXT_HTTP_RESPONSE_416 = "Range Not Satisfiable";
const char *TXT_HTTP_RESPONSE_417 = "Expectation Failed";
const char *TXT_HTTP_RESPONSE_418 = "I'm a teapot";
const char *TXT_HTTP_RESPONSE_421 = "Misdirected Request";
const char *TXT_HTTP_RESPONSE_422 = "Unprocessable Content";
const char *TXT_HTTP_RESPONSE_423 = "Locked";
const char *TXT_HTTP_RESPONSE_424 = "Failed Dependency";
const char *TXT_HTTP_RESPONSE_425 = "Too Early";
const char *TXT_HTTP_RESPONSE_426 = "Upgrade Required";
const char *TXT_HTTP_RESPONSE_428 = "Precondition Required";
const char *TXT_HTTP_RESPONSE_429 = "Too Many Requests";
const char *TXT_HTTP_RESPONSE_431 = "Request Header Fields Too Large";
const char *TXT_HTTP_RESPONSE_451 = "Unavailable For Legal Reasons";
// 5xx - Server Error Responses
const char *TXT_HTTP_RESPONSE_500 = "Internal Server Error";
const char *TXT_HTTP_RESPONSE_501 = "Not Implemented";
const char *TXT_HTTP_RESPONSE_502 = "Bad Gateway";
const char *TXT_HTTP_RESPONSE_503 = "Service Unavailable";
const char *TXT_HTTP_RESPONSE_504 = "Gateway Timeout";
const char *TXT_HTTP_RESPONSE_505 = "HTTP Version Not Supported";
const char *TXT_HTTP_RESPONSE_506 = "Variant Also Negotiates";
const char *TXT_HTTP_RESPONSE_507 = "Insufficient Storage";
const char *TXT_HTTP_RESPONSE_508 = "Loop Detected";
const char *TXT_HTTP_RESPONSE_510 = "Not Extended";
const char *TXT_HTTP_RESPONSE_511 = "Network Authentication Required";

// ARDUINOJSON DESERIALIZATION ERROR CODES
const char *TXT_DESERIALIZATION_ERROR_OK               = "Deserialization OK";
const char *TXT_DESERIALIZATION_ERROR_EMPTY_INPUT      = "Deserialization Empty Input";
const char *TXT_DESERIALIZATION_ERROR_INCOMPLETE_INPUT = "Deserialization Incomplete Input";
const char *TXT_DESERIALIZATION_ERROR_INVALID_INPUT    = "Deserialization Invalid Input";
const char *TXT_DESERIALIZATION_ERROR_NO_MEMORY        = "Deserialization No Memory";
const char *TXT_DESERIALIZATION_ERROR_TOO_DEEP         = "Deserialization Too Deep";

// WIFI STATUS
const char *TXT_WL_NO_SHIELD       = "No Shield";
const char *TXT_WL_IDLE_STATUS     = "Idle";
const char *TXT_WL_NO_SSID_AVAIL   = "No SSID Available";
const char *TXT_WL_SCAN_COMPLETED  = "Scan Complete";
const char *TXT_WL_CONNECTED       = "Connected";
const char *TXT_WL_CONNECT_FAILED  = "Connection Failed";
const char *TXT_WL_CONNECTION_LOST = "Connection Lost";
const char *TXT_WL_DISCONNECTED    = "Disconnected";
