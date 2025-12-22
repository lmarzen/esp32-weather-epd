/* Spanish locale data for esp32-weather-epd.
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
const char *LC_D_T_FMT     = "%a %d %b %Y %T %Z";
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
const char *LC_DAY[7]      = {"Domingo", "Lunes", "Martes", "Miercoles",
                              "Jueves", "Viernes", "Sabado"};
// ABDAY_{1-7}             abbreviated name of the n-th day of the week
const char *LC_ABDAY[7]    = {"Dom", "Lun", "Mar", "Mie", "Jue", "Vie", "Sab"};
// MON_{1-12}              name of the n-th month of the year
const char *LC_MON[12]     = { "Enero",   "Febrero",  "Marzo",  "Abril",
                               "Mayo",   "Junio",  "Julio",     "Agosto",
                               "Septiembre", "Octubre",  "Noviembre", "Diciembre"};
// ABMON_{1-12}            abbreviated name of the n-th month of the year
const char *LC_ABMON[12]   = {"Ene", "Feb", "Mar", "Abr", "May", "Jun",
                              "Jul", "Ago", "Sep", "Oct", "Nov", "Dic"};
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
const String OWM_LANG = "es";

// CURRENT CONDITIONS
const char *TXT_FEELS_LIKE         = "Sensaci\363n t\351rmica";
const char *TXT_SUNRISE            = "Amanecer";
const char *TXT_SUNSET             = "Atardecer";
const char *TXT_WIND               = "Viento";
const char *TXT_HUMIDITY           = "Humedad";
const char *TXT_UV_INDEX           = "\315ndice UV";
const char *TXT_PRESSURE           = "Presi\363n";
const char *TXT_AIR_QUALITY        = "Calidad del aire";
const char *TXT_AIR_POLLUTION      = "Contaminaci\363n del aire";
const char *TXT_VISIBILITY         = "Visibilidad";
const char *TXT_INDOOR_TEMPERATURE = "Temperatura";
const char *TXT_INDOOR_HUMIDITY    = "Humedad";

// UV INDEX
const char *TXT_UV_LOW       = "Bajo";
const char *TXT_UV_MODERATE  = "Moderado";
const char *TXT_UV_HIGH      = "Alto";
const char *TXT_UV_VERY_HIGH = "Muy Alto";
const char *TXT_UV_EXTREME   = "Extremo";

// WIFI
const char *TXT_WIFI_EXCELLENT     = "Excelente";
const char *TXT_WIFI_GOOD          = "Bueno";
const char *TXT_WIFI_FAIR          = "Aceptable";
const char *TXT_WIFI_WEAK          = "D\351bil";
const char *TXT_WIFI_NO_CONNECTION = "Sin conexi\363n";

// UNIT SYMBOLS - TEMPERATURE
const char *TXT_UNITS_TEMP_KELVIN     = "K";
const char *TXT_UNITS_TEMP_CELSIUS    = "C";
const char *TXT_UNITS_TEMP_FAHRENHEIT = "F";
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
const char *TXT_UNITS_PRES_GRAMSPERSQUARECENTIMETER = "g/cm";
const char *TXT_UNITS_PRES_POUNDSPERSQUAREINCH      = "lb/in";
// UNITS SYMBOLS - VISIBILITY DISTANCE
const char *TXT_UNITS_DIST_KILOMETERS = "km";
const char *TXT_UNITS_DIST_MILES      = "mi";
// UNITS SYMBOLS - PRECIPITATION
const char *TXT_UNITS_PRECIP_MILLIMETERS = "mm";
const char *TXT_UNITS_PRECIP_CENTIMETERS = "cm";
const char *TXT_UNITS_PRECIP_INCHES      = "in";

// MISCELLANEOUS MESSAGES
// Title Case
const char *TXT_LOW_BATTERY = "Bater\355a Baja";
const char *TXT_NETWORK_NOT_AVAILABLE = "Red No Disponible";
const char *TXT_TIME_SYNCHRONIZATION_FAILED = "Fallo en la Sincronizaci\363n de la Hora";
const char *TXT_WIFI_CONNECTION_FAILED = "Fallo en la Conexi\363n WiFi";
// First Word Capitalized
const char *TXT_ATTEMPTING_HTTP_REQ = "Intentando petici\363n HTTP";
const char *TXT_AWAKE_FOR = "Despierto durante";
const char *TXT_BATTERY_VOLTAGE = "Voltaje de la bater\355a";
const char *TXT_CONNECTING_TO = "Conectando a";
const char *TXT_COULD_NOT_CONNECT_TO = "No se pudo conectar a";
const char *TXT_ENTERING_DEEP_SLEEP_FOR = "Entrando en sue\361o profundo durante";
const char *TXT_READING_FROM = "Leyendo de";
const char *TXT_FAILED  = "Fallo";
const char *TXT_SUCCESS = "\311xitoso";
const char *TXT_UNKNOWN = "Desconocido";
// All Lowercase
const char *TXT_NOT_FOUND = "no encontrado";
const char *TXT_READ_FAILED = "fallo en la lectura";
// Complete Sentences
const char *TXT_FAILED_TO_GET_TIME = "\241Error al obtener la hora!";
const char *TXT_HIBERNATING_INDEFINITELY_NOTICE = "\241Hibernando sin tiempo de despertar!";
const char *TXT_REFERENCING_OLDER_TIME_NOTICE = "Fallo al sincronizar la hora antes del sue\361o profundo, haciendo referencia a la hora anterior.";
const char *TXT_WAITING_FOR_SNTP = "Esperando la sincronizaci\363n SNTP.";
const char *TXT_LOW_BATTERY_VOLTAGE = "\241Voltaje de la bater\355a bajo!";
const char *TXT_VERY_LOW_BATTERY_VOLTAGE = "\241Voltaje de la bater\355a muy bajo!";
const char *TXT_CRIT_LOW_BATTERY_VOLTAGE = "\241Voltaje de la bater\355a cr\355ticamente bajo!";

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
  "Muy Bueno",
  "Bueno",
  "Aceptable",
  "Pobre",
  "Muy Pobre",
  "Peligroso",
};
const char *CANADA_AQHI_TXT[4] =
{
  "Bajo",
  "Moderado",
  "Alto",
  "Muy Alto",
};
const char *EUROPEAN_UNION_CAQI_TXT[5] =
{
  "Muy Bajo",
  "Bajo",
  "Medio",
  "Alto",
  "Muy Alto",
};
const char *HONG_KONG_AQHI_TXT[5] =
{
  "Bajo",
  "Moderado",
  "Alto",
  "Muy Alto",
  "Peligroso",
};
const char *INDIA_AQI_TXT[6] =
{
  "Bueno",
  "Satisfactorio",
  "Moderado",
  "Pobre",
  "Muy Pobre",
  "Severo",
};
const char *CHINA_AQI_TXT[6] =
{
  "Excelente",
  "Bueno",
  "Ligeramente Contaminado",
  "Moderadamente Contaminado",
  "Muy Contaminado",
  "Severamente Contaminado",
};
const char *SINGAPORE_PSI_TXT[5] =
{
  "Bueno",
  "Moderado",
  "Poco Saludable",
  "Muy Poco Saludable",
  "Peligroso",
};
const char *SOUTH_KOREA_CAI_TXT[4] =
{
  "Bueno",
  "Medio",
  "Poco Saludable",
  "Muy Poco Saludable",
};
const char *UNITED_KINGDOM_DAQI_TXT[4] =
{
  "Bajo",
  "Moderado",
  "Alto",
  "Muy Alto",
};
const char *UNITED_STATES_AQI_TXT[6] =
{
  "Bueno",
  "Moderado",
  "Poco Saludable para Grupos Sensibles",
  "Poco Saludable",
  "Muy Poco Saludable",
  "Peligroso",
};
} // end extern "C"

// COMPASS POINT
const char *COMPASS_POINT_NOTATION[32] = {
//   0°   11.25°   22.5°   33.75°   45°   56.25°   67.5°   78.75°
   "N",  "NNO",  "NE",   "ENE",  "E",  "ESE",  "SE",  "SSE",
//  90°  101.25°  112.5°  123.75°  135°  146.25°  157.5°  168.75°
   "S",  "SSO",   "SO",  "OSO",  "O", "ONO",  "NO",  "NNO",
// 180°  191.25°  202.5°  213.75°  225°  236.25°  247.5°  258.75°
   "O",  "ONO",   "NO",  "NNO",  "N", "NNO",  "NE",  "ENE",
// 270°  281.25°  292.5°  303.75°  315°  326.25°  337.5°  348.75°
   "E",  "ESE",  "SE",  "SSE",  "S", "SSO",  "SO",  "OSO",
};

// HTTP CLIENT ERRORS
const char *TXT_HTTPC_ERROR_CONNECTION_REFUSED   = "Conexi\363n Rechazada";
const char *TXT_HTTPC_ERROR_SEND_HEADER_FAILED   = "Fallo al Enviar Encabezado";
const char *TXT_HTTPC_ERROR_SEND_PAYLOAD_FAILED  = "Fallo al Enviar Carga";
const char *TXT_HTTPC_ERROR_NOT_CONNECTED        = "No Conectado";
const char *TXT_HTTPC_ERROR_CONNECTION_LOST      = "Conexi\363n Perdida";
const char *TXT_HTTPC_ERROR_NO_STREAM            = "Ning\372n Flujo";
const char *TXT_HTTPC_ERROR_NO_HTTP_SERVER       = "Ning\372n Servidor HTTP";
const char *TXT_HTTPC_ERROR_TOO_LESS_RAM         = "Poca Memoria RAM";
const char *TXT_HTTPC_ERROR_ENCODING             = "Transfer-Encoding No Soportado";
const char *TXT_HTTPC_ERROR_STREAM_WRITE         = "Error de Escritura de Flujo";
const char *TXT_HTTPC_ERROR_READ_TIMEOUT         = "Tiempo de Espera de Lectura Excedido";

// HTTP RESPONSE STATUS CODES
// 1xx - Informational Responses
const char *TXT_HTTP_RESPONSE_100 = "Continuar";
const char *TXT_HTTP_RESPONSE_101 = "Cambiando Protocolos";
const char *TXT_HTTP_RESPONSE_102 = "Procesando";
const char *TXT_HTTP_RESPONSE_103 = "Pistas Tempranas";
// 2xx - Successful Responses
const char *TXT_HTTP_RESPONSE_200 = "OK";
const char *TXT_HTTP_RESPONSE_201 = "Creado";
const char *TXT_HTTP_RESPONSE_202 = "Aceptado";
const char *TXT_HTTP_RESPONSE_203 = "Informacion No Autorizada";
const char *TXT_HTTP_RESPONSE_204 = "Sin Contenido";
const char *TXT_HTTP_RESPONSE_205 = "Restablecer Contenido";
const char *TXT_HTTP_RESPONSE_206 = "Contenido Parcial";
const char *TXT_HTTP_RESPONSE_207 = "Multi-Estado";
const char *TXT_HTTP_RESPONSE_208 = "Ya Informado";
const char *TXT_HTTP_RESPONSE_226 = "IM Usado";
// 3xx - Redirection Responses
const char *TXT_HTTP_RESPONSE_300 = "Multiples Opciones";
const char *TXT_HTTP_RESPONSE_301 = "Movido Permanentemente";
const char *TXT_HTTP_RESPONSE_302 = "Encontrado";
const char *TXT_HTTP_RESPONSE_303 = "Ver Otros";
const char *TXT_HTTP_RESPONSE_304 = "No Modificado";
const char *TXT_HTTP_RESPONSE_305 = "Usar Proxy";
const char *TXT_HTTP_RESPONSE_307 = "Redireccion Temporal";
const char *TXT_HTTP_RESPONSE_308 = "Redireccion Permanente";
// 4xx - Client Error Responses
const char *TXT_HTTP_RESPONSE_400 = "Solicitud Incorrecta";
const char *TXT_HTTP_RESPONSE_401 = "No Autorizado";
const char *TXT_HTTP_RESPONSE_402 = "Pago Requerido";
const char *TXT_HTTP_RESPONSE_403 = "Prohibido";
const char *TXT_HTTP_RESPONSE_404 = "No Encontrado";
const char *TXT_HTTP_RESPONSE_405 = "Metodo No Permitido";
const char *TXT_HTTP_RESPONSE_406 = "No Aceptable";
const char *TXT_HTTP_RESPONSE_407 = "Se Requiere Autenticacion de Proxy";
const char *TXT_HTTP_RESPONSE_408 = "Tiempo de Espera de Solicitud Agotado";
const char *TXT_HTTP_RESPONSE_409 = "Conflicto";
const char *TXT_HTTP_RESPONSE_410 = "Perdido";
const char *TXT_HTTP_RESPONSE_411 = "Longitud Requerida";
const char *TXT_HTTP_RESPONSE_412 = "Fallo de Precondicion";
const char *TXT_HTTP_RESPONSE_413 = "Entidad de Solicitud Demasiado Grande";
const char *TXT_HTTP_RESPONSE_414 = "URI Demasiado Largo";
const char *TXT_HTTP_RESPONSE_415 = "Tipo de Medio No Soportado";
const char *TXT_HTTP_RESPONSE_416 = "Solicitud de Rango No Satisfactoria";
const char *TXT_HTTP_RESPONSE_417 = "Expectativa Fallida";
const char *TXT_HTTP_RESPONSE_418 = "Soy una tetera"; //TODO
const char *TXT_HTTP_RESPONSE_421 = "Solicitud Mal Dirigida";
const char *TXT_HTTP_RESPONSE_422 = "Entidad No Procesable";
const char *TXT_HTTP_RESPONSE_423 = "Bloqueado";
const char *TXT_HTTP_RESPONSE_424 = "Dependencia Fallida";
const char *TXT_HTTP_RESPONSE_425 = "Demasiado Pronto";
const char *TXT_HTTP_RESPONSE_426 = "Se Requiere Actualizacion";
const char *TXT_HTTP_RESPONSE_428 = "Se Requiere Precondicion";
const char *TXT_HTTP_RESPONSE_429 = "Demasiadas Solicitudes";
const char *TXT_HTTP_RESPONSE_431 = "Campos de Encabezado de Solicitud Demasiado Grandes";
const char *TXT_HTTP_RESPONSE_451 = "No Disponible por Razones Legales";
// 5xx - Server Error Responses
const char *TXT_HTTP_RESPONSE_500 = "Error Interno del Servidor";
const char *TXT_HTTP_RESPONSE_501 = "No Implementado";
const char *TXT_HTTP_RESPONSE_502 = "Puerta de Enlace Incorrecta";
const char *TXT_HTTP_RESPONSE_503 = "Servicio No Disponible";
const char *TXT_HTTP_RESPONSE_504 = "Tiempo de Espera de la Puerta de Enlace";
const char *TXT_HTTP_RESPONSE_505 = "Version HTTP No Soportada";
const char *TXT_HTTP_RESPONSE_506 = "La Variante Tambien Negocia";
const char *TXT_HTTP_RESPONSE_507 = "Almacenamiento Insuficiente";
const char *TXT_HTTP_RESPONSE_508 = "Bucle Detectado";
const char *TXT_HTTP_RESPONSE_510 = "No Extendido";
const char *TXT_HTTP_RESPONSE_511 = "Se Requiere Autenticacion de Red";

// ARDUINOJSON DESERIALIZATION ERROR CODES
const char *TXT_DESERIALIZATION_ERROR_OK               = "Desserializaci\363n OK";
const char *TXT_DESERIALIZATION_ERROR_EMPTY_INPUT      = "Entrada Vacia para Desserializaci\363n";
const char *TXT_DESERIALIZATION_ERROR_INCOMPLETE_INPUT = "Entrada Incompleta para Desserializaci\363n";
const char *TXT_DESERIALIZATION_ERROR_INVALID_INPUT    = "Entrada Invalida para Desserializaci\363n";
const char *TXT_DESERIALIZATION_ERROR_NO_MEMORY        = "Sin Memoria para Desserializaci\363n";
const char *TXT_DESERIALIZATION_ERROR_TOO_DEEP         = "Desserializaci\363n Demasiado Profunda";

// WIFI STATUS
const char *TXT_WL_NO_SHIELD       = "Sin Modulo WiFi";
const char *TXT_WL_IDLE_STATUS     = "Inactivo";
const char *TXT_WL_NO_SSID_AVAIL   = "Ninguna Red Disponible";
const char *TXT_WL_SCAN_COMPLETED  = "Escaneo Completado";
const char *TXT_WL_CONNECTED       = "Conectado";
const char *TXT_WL_CONNECT_FAILED  = "Fallo en la Conexi\363n";
const char *TXT_WL_CONNECTION_LOST = "Conexi\363n Perdida";
const char *TXT_WL_DISCONNECTED    = "Desconectado";
