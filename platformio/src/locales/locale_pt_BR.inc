/* American English locale data for esp32-weather-epd.
 * Copyright (C) 2022-2024  Artur Piasi
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
const char *LC_DAY[7]      = {"Domingo", "Segunda-feira", "Terça-feira", "Quarta-feira",
                              "Quinta-feira", "Sexta-feira", "Sábado"};
// ABDAY_{1-7}             abbreviated name of the n-th day of the week
const char *LC_ABDAY[7]    = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sáb"};
// MON_{1-12}              name of the n-th month of the year
const char *LC_MON[12]     = {"Janeiro",    "Fevereiro", "Março",    "Abril",
                               "Maio",       "Junho",     "Julho",     "Agosto",
                               "Setembro", "Outubro",  "Novembro", "Dezembro"};
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
const char *TXT_FEELS_LIKE         = "Sensação térmica";
const char *TXT_SUNRISE            = "Nascer do Sol";
const char *TXT_SUNSET             = "Pôr do Sol";
const char *TXT_WIND               = "Vento";
const char *TXT_HUMIDITY           = "Umidade";
const char *TXT_UV_INDEX           = "Índice UV";
const char *TXT_PRESSURE           = "Pressão";
const char *TXT_AIR_QUALITY_INDEX  = "Índice de Qualidade do Ar";
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
const char *TXT_WIFI_NO_CONNECTION = "Sem Conexão";

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

// MISCELLANEOUS MESSAGES
// Title Case
const char *TXT_LOW_BATTERY = "Bateria Baixa";
const char *TXT_NETWORK_NOT_AVAILABLE = "Rede Não Disponível";
const char *TXT_TIME_SYNCHRONIZATION_FAILED = "Falha na Sincronização do Tempo";
const char *TXT_WIFI_CONNECTION_FAILED = "Falha na Conexão WiFi";
// First Word Capitalized
const char *TXT_ATTEMPTING_HTTP_REQ = "Tentando Requisição HTTP";
const char *TXT_AWAKE_FOR = "Acordado por";
const char *TXT_BATTERY_VOLTAGE = "Voltagem da Bateria";
const char *TXT_CONNECTING_TO = "Conectando a";
const char *TXT_COULD_NOT_CONNECT_TO = "Não foi possível conectar a";
const char *TXT_ENTERING_DEEP_SLEEP_FOR = "Entrando em hibernação por";
const char *TXT_READING_FROM = "Lendo de";
const char *TXT_FAILED  = "Falhou";
const char *TXT_SUCCESS = "Sucesso";
const char *TXT_UNKNOWN = "Desconhecido";
// All Lowercase
const char *TXT_NOT_FOUND = "não encontrado";
const char *TXT_READ_FAILED = "falha na leitura";
// Complete Sentences
const char *TXT_FAILED_TO_GET_TIME = "Falha ao obter o horário!";
const char *TXT_HIBERNATING_INDEFINITELY_NOTICE = "Hibernando sem hora de despertar!";
const char *TXT_REFERENCING_OLDER_TIME_NOTICE = "Falha ao sincronizar o tempo antes da hibernação, referenciando tempo anterior.";
const char *TXT_WAITING_FOR_SNTP = "Aguardando sincronização SNTP.";
const char *TXT_LOW_BATTERY_VOLTAGE = "Baixa voltagem da bateria!";
const char *TXT_VERY_LOW_BATTERY_VOLTAGE = "Voltagem da bateria muito baixa!";
const char *TXT_CRIT_LOW_BATTERY_VOLTAGE = "Voltagem da bateria criticamente baixa!";

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
const std::vector<String> ALERT_URGENCY = {"statement", "watch", "advisory", "warning", "emergency"}; // US National Weather Service
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
const char *TXT_HTTPC_ERROR_CONNECTION_REFUSED   = "Conexão Recusada";
const char *TXT_HTTPC_ERROR_SEND_HEADER_FAILED   = "Falha ao Enviar Cabeçalho";
const char *TXT_HTTPC_ERROR_SEND_PAYLOAD_FAILED  = "Falha ao Enviar Carga";
const char *TXT_HTTPC_ERROR_NOT_CONNECTED        = "Não Conectado";
const char *TXT_HTTPC_ERROR_CONNECTION_LOST      = "Conexão Perdida";
const char *TXT_HTTPC_ERROR_NO_STREAM            = "Nenhum Fluxo";
const char *TXT_HTTPC_ERROR_NO_HTTP_SERVER       = "Nenhum Servidor HTTP";
const char *TXT_HTTPC_ERROR_TOO_LESS_RAM         = "Pouca Memória RAM";
const char *TXT_HTTPC_ERROR_ENCODING             = "Transfer-Encoding Não Suportado";
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
const char *TXT_HTTP_RESPONSE_203 = "Informações Não Autorizadas";
const char *TXT_HTTP_RESPONSE_204 = "Nenhum Conteúdo";
const char *TXT_HTTP_RESPONSE_205 = "Redefinir Conteúdo";
const char *TXT_HTTP_RESPONSE_206 = "Conteúdo Parcial";
const char *TXT_HTTP_RESPONSE_207 = "Status Multi";
const char *TXT_HTTP_RESPONSE_208 = "Já Reportado";
const char *TXT_HTTP_RESPONSE_226 = "IM Usado";
// 3xx - Redirection Responses
const char *TXT_HTTP_RESPONSE_300 = "Múltiplas Escolhas";
const char *TXT_HTTP_RESPONSE_301 = "Movido Permanentemente";
const char *TXT_HTTP_RESPONSE_302 = "Encontrado";
const char *TXT_HTTP_RESPONSE_303 = "Veja Outros";
const char *TXT_HTTP_RESPONSE_304 = "Não Modificado";
const char *TXT_HTTP_RESPONSE_305 = "Use Proxy";
const char *TXT_HTTP_RESPONSE_306 = "Proxy Móvel";
const char *TXT_HTTP_RESPONSE_307 = "Redirecionamento Temporário";
const char *TXT_HTTP_RESPONSE_308 = "Redirecionamento Permanente";
// 4xx - Client Error Responses
const char *TXT_HTTP_RESPONSE_400 = "Solicitação Incorreta";
const char *TXT_HTTP_RESPONSE_401 = "Não Autorizado";
const char *TXT_HTTP_RESPONSE_402 = "Pagamento Necessário";
const char *TXT_HTTP_RESPONSE_403 = "Proibido";
const char *TXT_HTTP_RESPONSE_404 = "Não Encontrado";
const char *TXT_HTTP_RESPONSE_405 = "Método Não Permitido";
const char *TXT_HTTP_RESPONSE_406 = "Não Aceitável";
const char *TXT_HTTP_RESPONSE_407 = "Autenticação de Proxy Necessária";
const char *TXT_HTTP_RESPONSE_408 = "Tempo Limite de Solicitação";
const char *TXT_HTTP_RESPONSE_409 = "Conflito";
const char *TXT_HTTP_RESPONSE_410 = "Desaparecido";
const char *TXT_HTTP_RESPONSE_411 = "Comprimento Necessário";
const char *TXT_HTTP_RESPONSE_412 = "Falha na Pré-condição";
const char *TXT_HTTP_RESPONSE_413 = "Solicitação de Entidade Grande Demais";
const char *TXT_HTTP_RESPONSE_414 = "URI Muito Longo";
const char *TXT_HTTP_RESPONSE_415 = "Tipo de Mídia Não Suportado";
const char *TXT_HTTP_RESPONSE_416 = "Solicitação de Faixa Não Satisfatória";
const char *TXT_HTTP_RESPONSE_417 = "Falha na Expectativa";
const char *TXT_HTTP_RESPONSE_418 = "I'm a teapot"; //TODO
const char *TXT_HTTP_RESPONSE_421 = "Solicitação Direcionada para Serviços";
const char *TXT_HTTP_RESPONSE_422 = "Entidade Não Processável";
const char *TXT_HTTP_RESPONSE_423 = "Bloqueado";
const char *TXT_HTTP_RESPONSE_424 = "Falha na Dependência";
const char *TXT_HTTP_RESPONSE_426 = "Atualização Obrigatória";
const char *TXT_HTTP_RESPONSE_428 = "Pré-condição Requerida";
const char *TXT_HTTP_RESPONSE_429 = "Muitas Solicitações";
const char *TXT_HTTP_RESPONSE_431 = "Campo de Cabeçalho de Solicitação Muito Grande";
const char *TXT_HTTP_RESPONSE_451 = "Indisponível por Motivos Legais";
// 5xx - Server Error Responses
const char *TXT_HTTP_RESPONSE_500 = "Erro Interno do Servidor";
const char *TXT_HTTP_RESPONSE_501 = "Não Implementado";
const char *TXT_HTTP_RESPONSE_502 = "Porta de Entrada Ruim";
const char *TXT_HTTP_RESPONSE_503 = "Serviço Indisponível";
const char *TXT_HTTP_RESPONSE_504 = "Tempo Limite do Gateway";
const char *TXT_HTTP_RESPONSE_505 = "Versão HTTP Não Suportada";
const char *TXT_HTTP_RESPONSE_506 = "Variação de Variante Também Negocia";
const char *TXT_HTTP_RESPONSE_507 = "Armazenamento Insuficiente";
const char *TXT_HTTP_RESPONSE_508 = "Loop Detectado";
const char *TXT_HTTP_RESPONSE_510 = "Não Estendido";
const char *TXT_HTTP_RESPONSE_511 = "Autenticação de Rede Necessária";

// ARDUINOJSON DESERIALIZATION ERROR CODES
const char *TXT_DESERIALIZATION_ERROR_OK               = "Desserialização OK";
const char *TXT_DESERIALIZATION_ERROR_EMPTY_INPUT      = "Entrada Vazia para Desserialização";
const char *TXT_DESERIALIZATION_ERROR_INCOMPLETE_INPUT = "Entrada Incompleta para Desserialização";
const char *TXT_DESERIALIZATION_ERROR_INVALID_INPUT    = "Entrada Inválida para Desserialização";
const char *TXT_DESERIALIZATION_ERROR_NO_MEMORY        = "Sem Memória para Desserialização";
const char *TXT_DESERIALIZATION_ERROR_TOO_DEEP         = "Desserialização Muito Profunda";

// WIFI STATUS
const char *TXT_WL_NO_SHIELD       = "Sem Módulo WiFi";
const char *TXT_WL_IDLE_STATUS     = "Em Repouso";
const char *TXT_WL_NO_SSID_AVAIL   = "Nenhuma Rede Disponível";
const char *TXT_WL_SCAN_COMPLETED  = "Varredura Completa";
const char *TXT_WL_CONNECTED       = "Conectado";
const char *TXT_WL_CONNECT_FAILED  = "Falha na Conexão";
const char *TXT_WL_CONNECTION_LOST = "Conexão Perdida";
const char *TXT_WL_DISCONNECTED    = "Desconectado";
