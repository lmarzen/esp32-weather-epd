/* Locale data declarations for esp32-weather-epd.
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

#ifndef ___LOCALE_H__
#define ___LOCALE_H__

#include <vector>
#include <Arduino.h>

// LC_TIME
extern const char *LC_D_T_FMT;
extern const char *LC_D_FMT;
extern const char *LC_T_FMT;
extern const char *LC_T_FMT_AMPM;
extern const char *LC_AM_STR;
extern const char *LC_PM_STR;
extern const char *LC_DAY[7];
extern const char *LC_ABDAY[7];
extern const char *LC_MON[12];
extern const char *LC_ABMON[12];
extern const char *LC_ERA;
extern const char *LC_ERA_D_FMT;
extern const char *LC_ERA_D_T_FMT;
extern const char *LC_ERA_T_FMT;

// OWM LANGUAGE
extern const String OWM_LANG;

// CURRENT CONDITIONS
extern const char *TXT_FEELS_LIKE;
extern const char *TXT_SUNRISE;
extern const char *TXT_SUNSET;
extern const char *TXT_WIND;
extern const char *TXT_HUMIDITY;
extern const char *TXT_UV_INDEX;
extern const char *TXT_PRESSURE;
extern const char *TXT_AIR_QUALITY_INDEX;
extern const char *TXT_VISIBILITY;
extern const char *TXT_INDOOR_TEMPERATURE;
extern const char *TXT_INDOOR_HUMIDITY;

// UV INDEX
extern const char *TXT_UV_LOW;
extern const char *TXT_UV_MODERATE;
extern const char *TXT_UV_HIGH;
extern const char *TXT_UV_VERY_HIGH;
extern const char *TXT_UV_EXTREME;

// WIFI
extern const char *TXT_WIFI_EXCELLENT;
extern const char *TXT_WIFI_GOOD;
extern const char *TXT_WIFI_FAIR;
extern const char *TXT_WIFI_WEAK;
extern const char *TXT_WIFI_NO_CONNECTION;

// UNIT SYMBOLS - TEMPERATURE
extern const char *TXT_UNITS_TEMP_KELVIN;
extern const char *TXT_UNITS_TEMP_CELSIUS;
extern const char *TXT_UNITS_TEMP_FAHRENHEIT;
// UNIT SYMBOLS - WIND SPEED
extern const char *TXT_UNITS_SPEED_METERSPERSECOND;
extern const char *TXT_UNITS_SPEED_FEETPERSECOND;
extern const char *TXT_UNITS_SPEED_KILOMETERSPERHOUR;
extern const char *TXT_UNITS_SPEED_MILESPERHOUR;
extern const char *TXT_UNITS_SPEED_KNOTS;
extern const char *TXT_UNITS_SPEED_BEAUFORT;
// UNIT SYMBOLS - PRESSURE
extern const char *TXT_UNITS_PRES_HECTOPASCALS;
extern const char *TXT_UNITS_PRES_PASCALS;
extern const char *TXT_UNITS_PRES_MILLIMETERSOFMERCURY;
extern const char *TXT_UNITS_PRES_INCHESOFMERCURY;
extern const char *TXT_UNITS_PRES_MILLIBARS;
extern const char *TXT_UNITS_PRES_ATMOSPHERES;
extern const char *TXT_UNITS_PRES_GRAMSPERSQUARECENTIMETER;
extern const char *TXT_UNITS_PRES_POUNDSPERSQUAREINCH;
// UNITS - VISIBILITY DISTANCE
extern const char *TXT_UNITS_DIST_KILOMETERS;
extern const char *TXT_UNITS_DIST_MILES;

// LAST REFRESH
extern const char *TXT_UNKNOWN;

// ALERTS
extern const std::vector<String> ALERT_URGENCY;
// ALERT TERMINOLOGY
extern const std::vector<String> TERM_SMOG;
extern const std::vector<String> TERM_SMOKE;
extern const std::vector<String> TERM_FOG;
extern const std::vector<String> TERM_METEOR;
extern const std::vector<String> TERM_NUCLEAR;
extern const std::vector<String> TERM_BIOHAZARD;
extern const std::vector<String> TERM_EARTHQUAKE;
extern const std::vector<String> TERM_TSUNAMI;
extern const std::vector<String> TERM_FIRE;
extern const std::vector<String> TERM_HEAT;
extern const std::vector<String> TERM_WINTER;
extern const std::vector<String> TERM_LIGHTNING;
extern const std::vector<String> TERM_SANDSTORM;
extern const std::vector<String> TERM_FLOOD;
extern const std::vector<String> TERM_VOLCANO;
extern const std::vector<String> TERM_AIR_QUALITY;
extern const std::vector<String> TERM_TORNADO;
extern const std::vector<String> TERM_SMALL_CRAFT_ADVISORY;
extern const std::vector<String> TERM_GALE_WARNING;
extern const std::vector<String> TERM_STORM_WARNING;
extern const std::vector<String> TERM_HURRICANE_WARNING;
extern const std::vector<String> TERM_HURRICANE;
extern const std::vector<String> TERM_DUST;
extern const std::vector<String> TERM_STRONG_WIND;

// AIR QUALITY INDEX
extern "C" {
extern const char *AUSTRALIA_AQI_TXT[6];
extern const char *CANADA_AQHI_TXT[4]; 
extern const char *EUROPE_CAQI_TXT[5]; 
extern const char *HONG_KONG_AQHI_TXT[5]; 
extern const char *INDIA_AQI_TXT[6]; 
extern const char *MAINLAND_CHINA_AQI_TXT[6]; 
extern const char *SINGAPORE_PSI_TXT[5]; 
extern const char *SOUTH_KOREA_CAI_TXT[4]; 
extern const char *UNITED_KINGDOM_DAQI_TXT[4]; 
extern const char *UNITED_STATES_AQI_TXT[6]; 
}

#endif
