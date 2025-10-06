/* Slovak Slovakia locale data for esp32-weather-epd.
 * Copyright (C) 2022-2025  Martin Sima
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

#include "_locale.h"
#include <vector>
#include <Arduino.h>

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
const char *LC_D_T_FMT     = "%a %d %b %Y %r %Z";
// LC_D_FMT                date format string
const char *LC_D_FMT       = "%m/%d/%y";
// T_FMT                   time format string
const char *LC_T_FMT       = "%r";
// T_FMT_AMPM              a.m. or p.m. time format string
const char *LC_T_FMT_AMPM  = "%I:%M:%S %p";
// AM_STR                  Ante Meridian affix
const char *LC_AM_STR      = "AM";
// PM_STR                  Post Meridian affix
const char *LC_PM_STR      = "PM";
// DAY_{1-7}               name of the n-th day of the week
const char *LC_DAY[7]      = {"Nede\265a", "Pondelok", "Utorok", "Streda",
                              "\251tvrtok", "Piatok", "Sobota"};
// ABDAY_{1-7}             abbreviated name of the n-th day of the week
const char *LC_ABDAY[7]    = {"Ned", "Pon", "Uto", "Str", "\251tv", "Pia", "Sob"};
// MON_{1-12}              name of the n-th month of the year
const char *LC_MON[12]     = {"Janu\341r",    "Febru\341r", "Marec",    "Apr\355l",
                               "M\341j",       "J\372n",     "J\372l",     "August",
                               "September", "Okt\363ber",  "November", "December"};
// ABMON_{1-12}            abbreviated name of the n-th month of the year
const char *LC_ABMON[12]   = {"Jan", "Feb", "Mar", "Apr", "Maj", "Jun",
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
const String OWM_LANG = "sk";

// CURRENT CONDITIONS
const char *TXT_FEELS_LIKE         = "Pocitovo";
const char *TXT_SUNRISE            = "V\375chod slnka";
const char *TXT_SUNSET             = "Z\341pad slnka";
const char *TXT_WIND               = "Vietor";
const char *TXT_HUMIDITY           = "Vlhkos\273";
const char *TXT_UV_INDEX           = "UV Index";
const char *TXT_PRESSURE           = "Tlak";
const char *TXT_AIR_QUALITY        = "Kvalita ovzdu\271ia";
const char *TXT_AIR_POLLUTION      = "Zne\350istenie ovzdu\271ia";
const char *TXT_VISIBILITY         = "Vidite\265nos\273";
const char *TXT_INDOOR_TEMPERATURE = "Teplota";
const char *TXT_INDOOR_HUMIDITY    = "Vlhkos\273";


// UV INDEX
const char *TXT_UV_LOW       = "N\355zky";
const char *TXT_UV_MODERATE  = "Mierny";
const char *TXT_UV_HIGH      = "Vysok\375";
const char *TXT_UV_VERY_HIGH = "Ve\265mi vysok\375";
const char *TXT_UV_EXTREME   = "Extr\351mny";

// WIFI
const char *TXT_WIFI_EXCELLENT     = "Vynikaj\372ci";
const char *TXT_WIFI_GOOD          = "Dobr\375";
const char *TXT_WIFI_FAIR          = "Priemern\375";
const char *TXT_WIFI_WEAK          = "Slab\375";
const char *TXT_WIFI_NO_CONNECTION = "\256iadne pripojenie";


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
const char *TXT_LOW_BATTERY = "Slab\341 bat\351ria";
const char *TXT_NETWORK_NOT_AVAILABLE = "Sie\273 nie je dostupn\341";
const char *TXT_TIME_SYNCHRONIZATION_FAILED = "Synchroniz\341cia \350asu zlyhala";
const char *TXT_WIFI_CONNECTION_FAILED = "Pripojenie na WiFi zlyhalo";
// First Word Capitalized
const char *TXT_ATTEMPTING_HTTP_REQ = "Pok\372\271am sa o HTTP po\276iadavku";
const char *TXT_AWAKE_FOR = "Preb\372den\375 na";
const char *TXT_BATTERY_VOLTAGE = "Nap\344tie bat\351rie";
const char *TXT_CONNECTING_TO = "Prip\341jam sa na";
const char *TXT_COULD_NOT_CONNECT_TO = "Nepodarilo sa pripoji\273 na";
const char *TXT_ENTERING_DEEP_SLEEP_FOR = "Vstup do hlbok\351ho sp\341nku na";
const char *TXT_READING_FROM = "\310itam z";
const char *TXT_FAILED  = "Zlyhalo";
const char *TXT_SUCCESS = "\332spech";
const char *TXT_UNKNOWN = "Nezn\341me";
// All Lowercase
const char *TXT_NOT_FOUND = "nenajdene";
const char *TXT_READ_FAILED = "\350itanie zlyhalo";
// Complete Sentences
const char *TXT_FAILED_TO_GET_TIME = "Nepodarilo sa z\355ska\273 \350as!";
const char *TXT_HIBERNATING_INDEFINITELY_NOTICE = "Hibern\341cia bez \350asu prebudenia!";
const char *TXT_REFERENCING_OLDER_TIME_NOTICE = "Nepodarilo sa synchronizova\273 \350as pred hlbokym sp\341nkom, pou\276\355vam star\271\355 \350as.";
const char *TXT_WAITING_FOR_SNTP = "\310akam na synchroniz\341ciu SNTP.";
const char *TXT_LOW_BATTERY_VOLTAGE = "N\355zke nap\344tie bat\351rie!";
const char *TXT_VERY_LOW_BATTERY_VOLTAGE = "Ve\265mi n\355zke nap\344tie bat\351rie!";
const char *TXT_CRIT_LOW_BATTERY_VOLTAGE = "Kriticky n\355zke nap\344tie bat\351rie!";


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
const std::vector<String> ALERT_URGENCY = {"v\375h\265ad", "oznamenie", "sledovanie", "poradenstvo", "v\375straha", "n\372dzov\375 stav"}; // US National Weather Service
// const std::vector<String> ALERT_URGENCY = {"zlt\341", "oranzov\341", "\350erven\341"};                 // United Kingdom's national weather service (MET Office)
// const std::vector<String> ALERT_URGENCY = {"men\271ie", "mierne", "v\341\276ne", "extr\351mne"}; // METEO
// const std::vector<String> ALERT_URGENCY = {}; // Disable urgency interpretation (algorithm will fallback to only prefer the most recently issued alerts)

// TERMINOLOGIA UPOZORNENI
// Meteorologicka terminologia spojena s jednotlivymi ikonami upozorneni
const std::vector<String> TERM_SMOG =
    {"smog"};
const std::vector<String> TERM_SMOKE =
    {"dym"};
const std::vector<String> TERM_FOG =
    {"hmla", "hmlovina"};
const std::vector<String> TERM_METEOR =
    {"meteorit", "asteroid"};
const std::vector<String> TERM_NUCLEAR =
    {"jadrov\375", "ionizuj\372ce \276iarenie"};
const std::vector<String> TERM_BIOHAZARD =
    {"biologick\341 hrozba", "biologick\351 nebezpe\350enstvo"};
const std::vector<String> TERM_EARTHQUAKE =
    {"zemetrasenie"};
const std::vector<String> TERM_FIRE =
    {"po\276iar", "\350erven\341 vlajka"};
const std::vector<String> TERM_HEAT =
    {"teplo"};
const std::vector<String> TERM_WINTER =
    {"metelica", "zima", "\265ad", "sneh", "d\341\276d so snehom", "chlad", "\265adov\375 d\341\276\357",
     "mraziv\375 vietor", "mr\341z", "n\341mraza", "kr\372py"};
const std::vector<String> TERM_TSUNAMI =
    {"tsunami", "vlny"};
const std::vector<String> TERM_LIGHTNING =
    {"b\372rka", "b\372rkov\341 bunka", "pulzuj\372ca b\372rka", "pasmo burky", "superb\372rka",
     "blesk"};
const std::vector<String> TERM_SANDSTORM =
    {"piesocna b\372rka", "vietor s prachom", "prasn\341 b\372rka"};
const std::vector<String> TERM_FLOOD =
    {"povoden", "b\371rkovy pr\355liv", "zaplavenie", "vlnenie", "vysok\351 moria", "vysok\351 pr\355livy",
     "pr\355livova vlna", "hydrologick\375"};
const std::vector<String> TERM_VOLCANO =
    {"sope\350n\375", "popol", "sopka", "erupcia"};
const std::vector<String> TERM_AIR_QUALITY =
    {"ovzdu\271ie", "stagn\341cia", "zne\350istenie"};
const std::vector<String> TERM_TORNADO =
    {"torn\341do"};
const std::vector<String> TERM_SMALL_CRAFT_ADVISORY =
    {"mal\351 plavidlo", "v\375straha pred vetrom"};
const std::vector<String> TERM_GALE_WARNING =
    {"siln\375 vietor"};
const std::vector<String> TERM_STORM_WARNING =
    {"v\375straha pred b\372rkou"};
const std::vector<String> TERM_HURRICANE_WARNING =
    {"v\375straha pred hurikanovym vetrom", "extr\351mny vietor", "siln\375 vietor"};
const std::vector<String> TERM_HURRICANE =
    {"hurik\341n", "tropick\341 b\372rka", "tajf\372n", "cykl\363n"};
const std::vector<String> TERM_DUST =
    {"prach", "piesok"};
const std::vector<String> TERM_STRONG_WIND =
    {"vietor"};


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
  "Ve\265mi dobr\351",
  "Dobr\351",
  "Priemern\351",
  "Slab\351",
  "Ve\265mi slab\351",
  "Nebezpe\350n\351",
};
const char *CANADA_AQHI_TXT[4] =
{
  "N\355zke",
  "Mierne",
  "Vysok\351",
  "Ve\265mi vysok\351",
};
const char *EUROPEAN_UNION_CAQI_TXT[5] =
{
  "Ve\265mi n\355zke",
  "N\355zke",
  "Stredn\351",
  "Vysok\351",
  "Ve\265mi vysok\351",
};
const char *HONG_KONG_AQHI_TXT[5] =
{
  "N\355zke",
  "Miern\351",
  "Vysok\351",
  "Ve\265mi vysok\351",
  "Nebezpe\350n\351",
};
const char *INDIA_AQI_TXT[6] =
{
  "Dobr\351",
  "Uspokojiv\351",
  "Miern\351",
  "Slab\351",
  "Ve\265mi slab\351",
  "V\341zne",
};
const char *CHINA_AQI_TXT[6] =
{
  "Vynikaj\372ce",
  "Dobr\351",
  "\245ahko zne\350sten\351",
  "Stredne zne\350sten\351",
  "Silno zne\350sten\351",
  "Ve\265mi silno zne\350sten\351",
};
const char *SINGAPORE_PSI_TXT[5] =
{
  "Dobr\351",
  "Miern\351",
  "Nezdrav\351",
  "Ve\265mi nezdrav\351",
  "Nebezpe\350n\351",
};
const char *SOUTH_KOREA_CAI_TXT[4] =
{
  "Dobr\351",
  "Stredn\351",
  "Nezdrav\351",
  "Ve\265mi nezdrav\351",
};
const char *UNITED_KINGDOM_DAQI_TXT[4] =
{
  "N\355zke",
  "Miern\351",
  "Vysok\351",
  "Ve\265mi vysok\351",
};
const char *UNITED_STATES_AQI_TXT[6] =
{
  "Dobr\351",
  "Miern\351",
  "Nezdrav\351 pre citlive skupiny",
  "Nezdrav\351",
  "Ve\265mi nezdrav\351",
  "Nebezpe\350n\351",
};

} // end extern "C"

// COMPASS POINT
const char *COMPASS_POINT_NOTATION[32] = {
//  0°   11.25°   22.5°   33.75°   45°   56.25°   67.5°   78.75°
   "S",  "SpV",   "SJV",  "VpS",   "V",  "VpV",   "VSV",  "VpS",
//  90°  101.25°  112.5°  123.75°  135°  146.25°  157.5°  168.75°
   "V",  "VpJ",   "VJV",  "JVpV",  "J",  "JpJ",   "JJV",  "JpV",
// 180°  191.25°  202.5°  213.75°  225°  236.25°  247.5°  258.75°
   "J",  "JpZ",   "JJZ",  "JZpJ",  "JZ", "JZpZ",  "ZJZ",  "ZpJ",
// 270°  281.25°  292.5°  303.75°  315°  326.25°  337.5°  348.75°
   "Z",  "ZpS",   "ZSZ",  "SZpZ",  "SZ", "SZpS",  "SZS",  "SpZ",
};


// HTTP CLIENT ERRORS
const char *TXT_HTTPC_ERROR_CONNECTION_REFUSED   = "Pripojenie odmietnut\351";
const char *TXT_HTTPC_ERROR_SEND_HEADER_FAILED   = "Zlyhanie pri odosielani hlavi\350ky";
const char *TXT_HTTPC_ERROR_SEND_PAYLOAD_FAILED  = "Zlyhanie pri odosielani \372dajov";
const char *TXT_HTTPC_ERROR_NOT_CONNECTED        = "Nie je pripojen\351";
const char *TXT_HTTPC_ERROR_CONNECTION_LOST      = "Spojenie prerusen\351";
const char *TXT_HTTPC_ERROR_NO_STREAM            = "\256iadn\375 pr\372d";
const char *TXT_HTTPC_ERROR_NO_HTTP_SERVER       = "\256iadn\375 HTTP server";
const char *TXT_HTTPC_ERROR_TOO_LESS_RAM         = "Nedostatok pam\344te";
const char *TXT_HTTPC_ERROR_ENCODING             = "Transfer-Encoding nie je podporovan\375";
const char *TXT_HTTPC_ERROR_STREAM_WRITE         = "Chyba pri z\341pise pr\372du";
const char *TXT_HTTPC_ERROR_READ_TIMEOUT         = "\310asov\375 limit \350itania";

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
const char *TXT_DESERIALIZATION_ERROR_OK               = "Deserializ\341cia OK";
const char *TXT_DESERIALIZATION_ERROR_EMPTY_INPUT      = "Pr\341zdny vstup deserializ\341cie";
const char *TXT_DESERIALIZATION_ERROR_INCOMPLETE_INPUT = "Neuplny vstup deserializ\341cie";
const char *TXT_DESERIALIZATION_ERROR_INVALID_INPUT    = "Neplatny vstup deserializ\341cie";
const char *TXT_DESERIALIZATION_ERROR_NO_MEMORY        = "Nedostatok pam\344te pre deserializ\341ciu";
const char *TXT_DESERIALIZATION_ERROR_TOO_DEEP         = "Pr\355li\271 hlbok\341 deserializ\341cia";

// WIFI STATUS
const char *TXT_WL_NO_SHIELD       = "Bez pokrytia";
const char *TXT_WL_IDLE_STATUS     = "Ne\350inn\375";
const char *TXT_WL_NO_SSID_AVAIL   = "SSID nedostupn\351";
const char *TXT_WL_SCAN_COMPLETED  = "Skenovanie dokon\350en\351";
const char *TXT_WL_CONNECTED       = "Pripojen\351";
const char *TXT_WL_CONNECT_FAILED  = "Pripojenie zlyhalo";
const char *TXT_WL_CONNECTION_LOST = "Spojenie preru\271en\351";
const char *TXT_WL_DISCONNECTED    = "Odpojen\351";
