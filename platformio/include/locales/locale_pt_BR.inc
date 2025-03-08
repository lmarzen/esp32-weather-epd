/* Brazilian Portuguese locale data for esp32-weather-epd.
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

/* Special thanks to Apiasi Piasi (apiasi) for contributing this translation.
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
//   schemes. For example, the en_US locale data is stored in the
//   localedata/locales/en_US file, which contains the definitions for date and
//   time formats, month and day names, and other time-related settings for the
//   LC_TIME category.
//   https://www.gnu.org/software/libc/sources.html
// D_T_FMT                 string for formatting date and time
const char *LC_D_T_FMT     = "%a %d %b %Y %T";
// D_FMT                   date format string
const char *LC_D_FMT       = "%d/%m/%Y";
// T_FMT                   time format string
const char *LC_T_FMT       = "%T";
// T_FMT_AMPM              a.m. or p.m. time format string
const char *LC_T_FMT_AMPM  = "%I:%M:%S %p";
// AM_STR                  Ante Meridian affix
const char *LC_AM_STR      = "AM";
// PM_STR                  Post Meridian affix
const char *LC_PM_STR      = "PM";
// DAY_{1-7}               name of the n-th day of the week
const char *LC_DAY[7]      = {"Domingo",      "Segunda-feira", "Ter\347a-feira", "Quarta-feira",
                              "Quinta-feira", "Sexta-feira",   "S\341bado"};
// ABDAY_{1-7}             abbreviated name of the n-th day of the week
const char *LC_ABDAY[7]    = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "S\341b"};
// MON_{1-12}              name of the n-th month of the year
const char *LC_MON[12]     = {"Janeiro",   "Fevereiro", "Mar\347o", "Abril",
                              "Maio",      "Junho",     "Julho",    "Agosto",
                              "Setembro",  "Outubro",   "Novembro", "Dezembro"};
// ABMON_{1-12}            abbreviated name of the n-th month of the year
const char *LC_ABMON[12]   = {"Jan", "Fev", "Mar", "Abr", "Mai", "Jun",
                              "Jul", "Ago", "Set", "Out", "Nov", "Dez"};
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
const String OWM_LANG = "pt_br";

// CURRENT CONDITIONS
const char *TXT_FEELS_LIKE         = "Sensa\347\343o t\351rmica";
const char *TXT_SUNRISE            = "Nascer do Sol";
const char *TXT_SUNSET             = "P\364r do Sol";
const char *TXT_WIND               = "Vento";
const char *TXT_HUMIDITY           = "Umidade";
const char *TXT_UV_INDEX           = "\315ndice UV";
const char *TXT_PRESSURE           = "Press\343o";
const char *TXT_AIR_QUALITY        = "Qualidade do Ar";
const char *TXT_AIR_POLLUTION      = "Polui\347\343o do Ar";
const char *TXT_VISIBILITY         = "Visibilidade";
const char *TXT_INDOOR_TEMPERATURE = "Temperatura Interna";
const char *TXT_INDOOR_HUMIDITY    = "Umidade Interna";

// UV INDEX
const char *TXT_UV_LOW       = "Baixo";
const char *TXT_UV_MODERATE  = "Moderado";
const char *TXT_UV_HIGH      = "Alto";
const char *TXT_UV_VERY_HIGH = "Muito Alto";
const char *TXT_UV_EXTREME   = "Extremo";

// WIFI
const char *TXT_WIFI_EXCELLENT     = "Excelente";
const char *TXT_WIFI_GOOD          = "Bom";
const char *TXT_WIFI_FAIR          = "Regular";
const char *TXT_WIFI_WEAK          = "Fraco";
const char *TXT_WIFI_NO_CONNECTION = "Sem Conex\343o";

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
const char *TXT_UNITS_PRES_GRAMSPERSQUARECENTIMETER = "g/cm\xB2";
const char *TXT_UNITS_PRES_POUNDSPERSQUAREINCH      = "lb/in\xB2";
// UNITS SYMBOLS - VISIBILITY DISTANCE
const char *TXT_UNITS_DIST_KILOMETERS = "km";
const char *TXT_UNITS_DIST_MILES      = "mi";
// UNITS SYMBOLS - PRECIPITATION
const char *TXT_UNITS_PRECIP_MILLIMETERS = "mm";
const char *TXT_UNITS_PRECIP_CENTIMETERS = "cm";
const char *TXT_UNITS_PRECIP_INCHES      = "in";

// MISCELLANEOUS MESSAGES
// Title Case
const char *TXT_LOW_BATTERY = "Bateria Baixa";
const char *TXT_NETWORK_NOT_AVAILABLE = "Rede N\343o Dispon\355vel";
const char *TXT_TIME_SYNCHRONIZATION_FAILED = "Falha na Sincroniza\347\343o do Tempo";
const char *TXT_WIFI_CONNECTION_FAILED = "Falha na Conex\343o WiFi";
// First Word Capitalized
const char *TXT_ATTEMPTING_HTTP_REQ = "Tentando Requisi\347\343o HTTP";
const char *TXT_AWAKE_FOR = "Acordado por";
const char *TXT_BATTERY_VOLTAGE = "Voltagem da Bateria";
const char *TXT_CONNECTING_TO = "Conectando a";
const char *TXT_COULD_NOT_CONNECT_TO = "N\343o foi poss\355vel conectar a";
const char *TXT_ENTERING_DEEP_SLEEP_FOR = "Entrando em hiberna\347\343o por";
const char *TXT_READING_FROM = "Lendo de";
const char *TXT_FAILED  = "Falhou";
const char *TXT_SUCCESS = "Sucesso";
const char *TXT_UNKNOWN = "Desconhecido";
// All Lowercase
const char *TXT_NOT_FOUND = "n\343o encontrado";
const char *TXT_READ_FAILED = "falha na leitura";
// Complete Sentences
const char *TXT_FAILED_TO_GET_TIME = "Falha ao obter o hor\341rio!";
const char *TXT_HIBERNATING_INDEFINITELY_NOTICE = "Hibernando sem hora de despertar!";
const char *TXT_REFERENCING_OLDER_TIME_NOTICE = "Falha ao sincronizar o tempo antes da hiberna\347\343o, referenciando tempo anterior.";
const char *TXT_WAITING_FOR_SNTP = "Aguardando sincroniza\347\343o SNTP.";
const char *TXT_LOW_BATTERY_VOLTAGE = "Baixa voltagem da bateria!";
const char *TXT_VERY_LOW_BATTERY_VOLTAGE = "Voltagem da bateria muito baixa!";
const char *TXT_CRIT_LOW_BATTERY_VOLTAGE = "Voltagem da bateria criticamente baixa!";

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
const std::vector<String> ALERT_URGENCY = {"outlook", "statement", "watch", "advisory", "warning", "emergency"}; // US National Weather Service
// const std::vector<String> ALERT_URGENCY = {"yellow", "amber", "red"};                 // United Kingdom's national weather service (MET Office)
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
const aqi_scale_t AQI_SCALE = UNITED_STATES_AQI;

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
const char *EUROPEAN_UNION_CAQI_TXT[5] =
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
const char *CHINA_AQI_TXT[6] =
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

// COMPASS POINT
const char *COMPASS_POINT_NOTATION[32] = {
//   0°   11.25°   22.5°   33.75°   45°   56.25°   67.5°   78.75°
   "N",  "NbE",  "NNE",   "NEbN",  "NE", "NEbE",  "ENE",  "EbN",
//  90°  101.25°  112.5°  123.75°  135°  146.25°  157.5°  168.75°
   "E",  "EbS",   "ESE",  "SEbE",  "SE", "SEbS",  "SSE",  "SbE",
// 180°  191.25°  202.5°  213.75°  225°  236.25°  247.5°  258.75°
   "S",  "SbW",   "SSW",  "SWbS",  "SW", "SWbW",  "WSW",  "WbS",
// 270°  281.25°  292.5°  303.75°  315°  326.25°  337.5°  348.75°
   "W",  "WbN",   "WNW",  "NWbW",  "NW", "NWbN",  "NNW",  "NbW",
};

// HTTP CLIENT ERRORS
const char *TXT_HTTPC_ERROR_CONNECTION_REFUSED   = "Conex\343o Recusada";
const char *TXT_HTTPC_ERROR_SEND_HEADER_FAILED   = "Falha ao Enviar Cabe\347alho";
const char *TXT_HTTPC_ERROR_SEND_PAYLOAD_FAILED  = "Falha ao Enviar Carga";
const char *TXT_HTTPC_ERROR_NOT_CONNECTED        = "N\343o Conectado";
const char *TXT_HTTPC_ERROR_CONNECTION_LOST      = "Conex\343o Perdida";
const char *TXT_HTTPC_ERROR_NO_STREAM            = "Nenhum Fluxo";
const char *TXT_HTTPC_ERROR_NO_HTTP_SERVER       = "Nenhum Servidor HTTP";
const char *TXT_HTTPC_ERROR_TOO_LESS_RAM         = "Pouca Mem\363ria RAM";
const char *TXT_HTTPC_ERROR_ENCODING             = "Transfer-Encoding N\343o Suportado";
const char *TXT_HTTPC_ERROR_STREAM_WRITE         = "Erro na Escrita do Fluxo";
const char *TXT_HTTPC_ERROR_READ_TIMEOUT         = "Tempo de Leitura Excedido";

// HTTP RESPONSE STATUS CODES
// 1xx - Informational Responses
const char *TXT_HTTP_RESPONSE_100 = "Continuar";
const char *TXT_HTTP_RESPONSE_101 = "Mudando Protocolos";
const char *TXT_HTTP_RESPONSE_102 = "Processando";
const char *TXT_HTTP_RESPONSE_103 = "Dicas Iniciais";
// 2xx - Successful Responses
const char *TXT_HTTP_RESPONSE_200 = "OK";
const char *TXT_HTTP_RESPONSE_201 = "Criado";
const char *TXT_HTTP_RESPONSE_202 = "Aceito";
const char *TXT_HTTP_RESPONSE_203 = "Informa\347\365es N\343o Autorizadas";
const char *TXT_HTTP_RESPONSE_204 = "Nenhum Conte\372do";
const char *TXT_HTTP_RESPONSE_205 = "Redefinir Conte\372do";
const char *TXT_HTTP_RESPONSE_206 = "Conte\372do Parcial";
const char *TXT_HTTP_RESPONSE_207 = "Status Multi";
const char *TXT_HTTP_RESPONSE_208 = "J\341 Reportado";
const char *TXT_HTTP_RESPONSE_226 = "IM Usado";
// 3xx - Redirection Responses
const char *TXT_HTTP_RESPONSE_300 = "M\372ltiplas Escolhas";
const char *TXT_HTTP_RESPONSE_301 = "Movido Permanentemente";
const char *TXT_HTTP_RESPONSE_302 = "Encontrado";
const char *TXT_HTTP_RESPONSE_303 = "Veja Outros";
const char *TXT_HTTP_RESPONSE_304 = "N\343o Modificado";
const char *TXT_HTTP_RESPONSE_305 = "Use Proxy";
const char *TXT_HTTP_RESPONSE_306 = "Proxy M\363vel";
const char *TXT_HTTP_RESPONSE_307 = "Redirecionamento Tempor\341rio";
const char *TXT_HTTP_RESPONSE_308 = "Redirecionamento Permanente";
// 4xx - Client Error Responses
const char *TXT_HTTP_RESPONSE_400 = "Solicita\347\343o Incorreta";
const char *TXT_HTTP_RESPONSE_401 = "N\343o Autorizado";
const char *TXT_HTTP_RESPONSE_402 = "Pagamento Necess\341rio";
const char *TXT_HTTP_RESPONSE_403 = "Proibido";
const char *TXT_HTTP_RESPONSE_404 = "N\343o Encontrado";
const char *TXT_HTTP_RESPONSE_405 = "M\351todo N\343o Permitido";
const char *TXT_HTTP_RESPONSE_406 = "N\343o Aceit\341vel";
const char *TXT_HTTP_RESPONSE_407 = "Autentica\347\343o de Proxy Necess\341ria";
const char *TXT_HTTP_RESPONSE_408 = "Tempo Limite de Solicita\347\343o";
const char *TXT_HTTP_RESPONSE_409 = "Conflito";
const char *TXT_HTTP_RESPONSE_410 = "Desaparecido";
const char *TXT_HTTP_RESPONSE_411 = "Comprimento Necess\341rio";
const char *TXT_HTTP_RESPONSE_412 = "Falha na Pr\351-condi\347\343o";
const char *TXT_HTTP_RESPONSE_413 = "Solicita\347\343o de Entidade Grande Demais";
const char *TXT_HTTP_RESPONSE_414 = "URI Muito Longo";
const char *TXT_HTTP_RESPONSE_415 = "Tipo de M\355dia N\343o Suportado";
const char *TXT_HTTP_RESPONSE_416 = "Solicita\347\343o de Faixa N\343o Satisfat\363ria";
const char *TXT_HTTP_RESPONSE_417 = "Falha na Expectativa";
const char *TXT_HTTP_RESPONSE_418 = "I'm a teapot"; //TODO
const char *TXT_HTTP_RESPONSE_421 = "Solicita\347\343o Direcionada para Servi\347os";
const char *TXT_HTTP_RESPONSE_422 = "Entidade N\343o Process\341vel";
const char *TXT_HTTP_RESPONSE_423 = "Bloqueado";
const char *TXT_HTTP_RESPONSE_424 = "Falha na Depend\352ncia";
const char *TXT_HTTP_RESPONSE_425 = "Cole\347\343o N\343o Ordenada";
const char *TXT_HTTP_RESPONSE_426 = "Atualiza\347\343o Obrigat\363ria";
const char *TXT_HTTP_RESPONSE_428 = "Pr\351-condi\347\343o Requerida";
const char *TXT_HTTP_RESPONSE_429 = "Muitas Solicita\347\365es";
const char *TXT_HTTP_RESPONSE_431 = "Campo de Cabe\347alho de Solicita\347\343o Muito Grande";
const char *TXT_HTTP_RESPONSE_451 = "Indispon\355vel por Motivos Legais";
// 5xx - Server Error Responses
const char *TXT_HTTP_RESPONSE_500 = "Erro Interno do Servidor";
const char *TXT_HTTP_RESPONSE_501 = "N\343o Implementado";
const char *TXT_HTTP_RESPONSE_502 = "Porta de Entrada Ruim";
const char *TXT_HTTP_RESPONSE_503 = "Servi\347o Indispon\355vel";
const char *TXT_HTTP_RESPONSE_504 = "Tempo Limite do Gateway";
const char *TXT_HTTP_RESPONSE_505 = "Vers\343o HTTP N\343o Suportada";
const char *TXT_HTTP_RESPONSE_506 = "Varia\347\343o de Variante Tamb\351m Negocia";
const char *TXT_HTTP_RESPONSE_507 = "Armazenamento Insuficiente";
const char *TXT_HTTP_RESPONSE_508 = "Loop Detectado";
const char *TXT_HTTP_RESPONSE_510 = "N\343o Estendido";
const char *TXT_HTTP_RESPONSE_511 = "Autentica\347\343o de Rede Necess\341ria";

// ARDUINOJSON DESERIALIZATION ERROR CODES
const char *TXT_DESERIALIZATION_ERROR_OK               = "Desserializa\347\343o OK";
const char *TXT_DESERIALIZATION_ERROR_EMPTY_INPUT      = "Entrada Vazia para Desserializa\347\343o";
const char *TXT_DESERIALIZATION_ERROR_INCOMPLETE_INPUT = "Entrada Incompleta para Desserializa\347\343o";
const char *TXT_DESERIALIZATION_ERROR_INVALID_INPUT    = "Entrada Inv\341lida para Desserializa\347\343o";
const char *TXT_DESERIALIZATION_ERROR_NO_MEMORY        = "Sem Mem\363ria para Desserializa\347\343o";
const char *TXT_DESERIALIZATION_ERROR_TOO_DEEP         = "Desserializa\347\343o Muito Profunda";

// WIFI STATUS
const char *TXT_WL_NO_SHIELD       = "Sem M\363dulo WiFi";
const char *TXT_WL_IDLE_STATUS     = "Em Repouso";
const char *TXT_WL_NO_SSID_AVAIL   = "Nenhuma Rede Dispon\355vel";
const char *TXT_WL_SCAN_COMPLETED  = "Varredura Completa";
const char *TXT_WL_CONNECTED       = "Conectado";
const char *TXT_WL_CONNECT_FAILED  = "Falha na Conex\343o";
const char *TXT_WL_CONNECTION_LOST = "Conex\343o Perdida";
const char *TXT_WL_DISCONNECTED    = "Desconectado";
