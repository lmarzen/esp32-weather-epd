/* Display helper utilities for esp32-weather-epd.
 * Copyright (C) 2022-2025  Luke Marzen
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

#include <cmath>
#include <vector>
#include <Arduino.h>
#include <driver/adc.h>
#include <esp_adc_cal.h>

#include <aqi.h>

#include "_locale.h"
#include "_strftime.h"
#include "api_response.h"
#include "config.h"
#include "display_utils.h"

// icon header files
#include "icons/icons.h"

/* Returns battery voltage in millivolts (mv).
 */
uint32_t readBatteryVoltage()
{
  esp_adc_cal_characteristics_t adc_chars;
  // __attribute__((unused)) disables compiler warnings about this variable
  // being unused (Clang, GCC) which is the case when DEBUG_LEVEL == 0.
  esp_adc_cal_value_t val_type __attribute__((unused));
  adc_power_acquire();
  uint16_t adc_val = analogRead(PIN_BAT_ADC);
  adc_power_release();

  // We will use the eFuse ADC calibration bits, to get accurate voltage
  // readings. The DFRobot FireBeetle Esp32-E V1.0's ADC is 12 bit, and uses
  // 11db attenuation, which gives it a measurable input voltage range of 150mV
  // to 2450mV.
  val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_11db,
                                      ADC_WIDTH_BIT_12, 1100, &adc_chars);

#if DEBUG_LEVEL >= 1
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
  {
    Serial.println("[debug] ADC Cal eFuse Vref");
  }
  else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP)
  {
    Serial.println("[debug] ADC Cal Two Point");
  }
  else
  {
    Serial.println("[debug] ADC Cal Default");
  }
#endif

  uint32_t batteryVoltage = esp_adc_cal_raw_to_voltage(adc_val, &adc_chars);
  // DFRobot FireBeetle Esp32-E V1.0 voltage divider (1M+1M), so readings are
  // multiplied by 2.
  batteryVoltage *= 2;
  return batteryVoltage;
} // end readBatteryVoltage

/* Returns battery percentage, rounded to the nearest integer.
 * Takes a voltage in millivolts and uses a sigmoidal approximation to find an
 * approximation of the battery life percentage remaining.
 * 
 * This function contains LGPLv3 code from 
 * <https://github.com/rlogiacco/BatterySense>.
 * 
 * Symmetric sigmoidal approximation
 * <https://www.desmos.com/calculator/7m9lu26vpy>
 *
 * c - c / (1 + k*x/v)^3
 */
uint32_t calcBatPercent(uint32_t v, uint32_t minv, uint32_t maxv)
{
  // slow
  //uint32_t p = 110 - (110 / (1 + pow(1.468 * (v - minv)/(maxv - minv), 6)));

  // steep
  //uint32_t p = 102 - (102 / (1 + pow(1.621 * (v - minv)/(maxv - minv), 8.1)));

  // normal
  uint32_t p = 105 - (105 / (1 + pow(1.724 * (v - minv)/(maxv - minv), 5.5)));
  return p >= 100 ? 100 : p;
} // end calcBatPercent

/* Returns 24x24 bitmap incidcating battery status.
 */
const uint8_t *getBatBitmap24(uint32_t batPercent)
{
  if (batPercent >= 93)
  {
    return battery_full_90deg_24x24;
  }
  else if (batPercent >= 79)
  {
    return battery_6_bar_90deg_24x24;
  }
  else if (batPercent >= 65)
  {
    return battery_5_bar_90deg_24x24;
  }
  else if (batPercent >= 50)
  {
    return battery_4_bar_90deg_24x24;
  }
  else if (batPercent >= 36)
  {
    return battery_3_bar_90deg_24x24;
  }
  else if (batPercent >= 22)
  {
    return battery_2_bar_90deg_24x24;
  }
  else if (batPercent >= 8)
  {
    return battery_1_bar_90deg_24x24;
  }
  else
  {  // batPercent < 8
    return battery_0_bar_90deg_24x24;
  }
} // end getBatBitmap24

/* Gets string with the current date.
 */
void getDateStr(String &s, tm *timeInfo)
{
  char buf[48] = {};
  _strftime(buf, sizeof(buf), DATE_FORMAT, timeInfo);
  s = buf;

  // remove double spaces. %e will add an extra space, ie. " 1" instead of "1"
  s.replace("  ", " ");
  return;
} // end getDateStr

/* Gets string with the current date and time of the current refresh attempt.
 */
void getRefreshTimeStr(String &s, bool timeSuccess, tm *timeInfo)
{
  if (timeSuccess == false)
  {
    s = TXT_UNKNOWN;
    return;
  }

  char buf[48] = {};
  _strftime(buf, sizeof(buf), REFRESH_TIME_FORMAT, timeInfo);
  s = buf;

  // remove double spaces.
  s.replace("  ", " ");
  return;
} // end getRefreshTimeStr

/* Takes a String and capitalizes the first letter of every word.
 *
 * Ex:
 *   input   : "severe thunderstorm warning" or "SEVERE THUNDERSTORM WARNING"
 *   becomes : "Severe Thunderstorm Warning"
 */
void toTitleCase(String &text)
{
  text.setCharAt(0, toUpperCase(text.charAt(0)));

  for (int i = 1; i < text.length(); ++i)
  {
    if (text.charAt(i - 1) == ' '
     || text.charAt(i - 1) == '-'
     || text.charAt(i - 1) == '(')
    {
      text.setCharAt(i, toUpperCase(text.charAt(i)));
    }
    else
    {
      text.setCharAt(i, toLowerCase(text.charAt(i)));
    }
  }

  return;
} // end toTitleCase

/* Takes a String and truncates at any of these characters ,.( and trims any
 * trailing whitespace.
 *
 * Ex:
 *   input   : "Severe Thunderstorm Warning, (Starting At 10 Pm)"
 *   becomes : "Severe Thunderstorm Warning"
 */
void truncateExtraAlertInfo(String &text)
{
  if (text.isEmpty())
  {
    return;
  }

  int i = 1;
  int lastChar = i;
  while (i < text.length()
    && text.charAt(i) != ','
    && text.charAt(i) != '.'
    && text.charAt(i) != '(')
  {
    if (text.charAt(i) != ' ')
    {
      lastChar = i + 1;
    }
    ++i;
  }

  text = text.substring(0, lastChar);
  return;
} // end truncateExtraAlertInfo

/* Returns the urgency of an event based by checking if the event String
 * contains any indicator keywords.
 *
 * Urgency keywords are defined in config.h because they are very regional.
 *   ex: United States - (Watch < Advisory < Warning)
 *
 * The index in vector<String> ALERT_URGENCY indicates the urgency level.
 * If an event string matches none of these keywords the urgency is unknown, -1
 * is returned.
 * In the United States example, Watch = 0, Advisory = 1, Warning = 2
 */
int eventUrgency(const String &event)
{
  int urgency_lvl = -1;
  for (int i = 0; i < ALERT_URGENCY.size(); ++i)
  {
    if (event.indexOf(ALERT_URGENCY[i]) >= 0)
    {
      urgency_lvl = i;
    }
  }
  return urgency_lvl;
} // end eventUrgency

/* This algorithm filters alerts from the API responses to be displayed by
 * marking the corresponding index in the ignore list.
 *
 * Background:
 * The display layout is setup to show up to 2 alerts, but alerts can be
 * unpredictible in severity and number. If more than 2 alerts are active, this
 * algorithm will attempt to interpret the urgency of each alert and prefer to
 * display the most urgent and recently issued alerts of each event type.
 * Depending on the region different keywords are used to convey the level of
 * urgency.
 *
 * A vector array is used to store these keywords. (defined in config.h) Urgency
 * is ranked from low to high where the first index of the vector is the least
 * urgent keyword and the last index is the most urgent keyword. Expected as all
 * lowercase.
 *
 *
 * Pseudo Code:
 * Convert all event text and tags to lowercase.
 *
 * // Deduplicate alerts of the same type
 * Dedup alerts with the same first tag. (ie. tag 0) Keeping only the most
 *   urgent alerts of each tag and alerts who's urgency cannot be determined.
 * Note: urgency keywords are defined in config.h because they are very
 *       regional. ex: United States - (Watch < Advisory < Warning)
 *
 * // Save only the 2 most recent alerts
 * If (more than 2 weather alerts remain)
 *   Keep only the 2 most recently issued alerts (aka greatest "start" time)
 *   OpenWeatherMap provides this order, so we can just take index 0 and 1.
 *
 * Truncate Extraneous Info (anything that follows a comma, period, or open
 *   parentheses)
 */
void filterAlerts(std::vector<owm_alerts_t> &resp, int *ignore_list)
{
  // Convert all event text and tags to lowercase.
  for (auto &alert : resp)
  {
    alert.event.toLowerCase();
    alert.tags.toLowerCase();
  }

  // Deduplicate alerts with the same first tag. Keeping only the most urgent
  // alerts of each tag and alerts who's urgency cannot be determined.
  for (int i = 0; i < resp.size(); ++i)
  {
    if (ignore_list[i] == 1)
    {
      continue;
    }
    if (resp[i].tags.isEmpty())
    {
      continue; // urgency can not be determined so it remains in the list
    }

    for (int j = 0; j < resp.size(); ++j)
    {
      if (i != j && resp[i].tags == resp[j].tags)
      {
        // comparing alerts of the same tag, removing the less urgent alert
        if (eventUrgency(resp[i].event) >= eventUrgency(resp[j].event))
        {
          ignore_list[j] = 1;
        }
      }
    }
  }

  // Save only the 2 most recent alerts
  int valid_cnt = 0;
  for (int i = 0; i < resp.size(); ++i)
  {
    if (valid_cnt < 2 && !ignore_list[i])
    {
      ++valid_cnt;
    }
    else
    {
      ignore_list[i] = 1;
    }
  }

  // Remove trailing/extraneous information
  for (auto &alert : resp)
  {
    truncateExtraAlertInfo(alert.event);
  }

  return;
} // end filterAlerts

/* Returns the descriptor text for the given UV index.
 */
const char *getUVIdesc(unsigned int uvi)
{
  if (uvi <= 2)
  {
    return TXT_UV_LOW;
  }
  else if (uvi <= 5)
  {
    return TXT_UV_MODERATE;
  }
  else if (uvi <= 7)
  {
    return TXT_UV_HIGH;
  }
  else if (uvi <= 10)
  {
    return TXT_UV_VERY_HIGH;
  }
  else // uvi >= 11
  {
    return TXT_UV_EXTREME;
  }
} // end getUVIdesc

/* Returns the wifi signal strength descriptor text for the given RSSI.
 */
const char *getWiFidesc(int rssi)
{
  if (rssi == 0)
  {
    return TXT_WIFI_NO_CONNECTION;
  }
  else if (rssi >= -50)
  {
    return TXT_WIFI_EXCELLENT;
  }
  else if (rssi >= -60)
  {
    return TXT_WIFI_GOOD;
  }
  else if (rssi >= -70)
  {
    return TXT_WIFI_FAIR;
  }
  else
  {  // rssi < -70
    return TXT_WIFI_WEAK;
  }
} // end getWiFidesc

/* Returns 16x16 bitmap incidcating wifi status.
 */
const uint8_t *getWiFiBitmap16(int rssi)
{
  if (rssi == 0)
  {
    return wifi_x_16x16;
  }
  else if (rssi >= -50)
  {
    return wifi_16x16;
  }
  else if (rssi >= -60)
  {
    return wifi_3_bar_16x16;
  }
  else if (rssi >= -70)
  {
    return wifi_2_bar_16x16;
  }
  else
  {  // rssi < -70
    return wifi_1_bar_16x16;
  }
} // end getWiFiBitmap24

/* Returns true if icon is a daytime icon, false otherwise.
 */
bool isDay(String icon) 
{
  // OpenWeatherMap indicates sun is up with d otherwise n for night
  return icon.endsWith("d");
}

/* Returns true if the moon is currently in the sky above, false otherwise.
 */
bool isMoonInSky(int64_t current_dt, int64_t moonrise_dt, int64_t moonset_dt,
                 float moon_phase)
{
  // (moon is out if current time is after moonrise but before moonset
  //   OR if moonrises after moonset and the current time is after moonrise)
  // AND (moon phase is not a new moon)
  return ((current_dt >= moonrise_dt && current_dt < moonset_dt)
          || (moonrise_dt > moonset_dt && current_dt >= moonrise_dt))
         && (moon_phase != 0.f && moon_phase != 1.f);
}

/* Takes cloudiness (%) and returns true if it is at least partially cloudy,
 * false otherwise.
 *
 * References:
 *   https://www.weather.gov/ajk/ForecastTerms
 */
bool isCloudy(int clouds) {
  return clouds > 60.25; // partly cloudy / partly sunny
}

/* Takes wind speed and wind gust speed and returns true if it is windy, false
 * otherwise.
 *
 * References:
 *   https://www.weather.gov/ajk/ForecastTerms
 */
bool isWindy(float wind_speed, float wind_gust) {
 return (wind_speed >= 32.2 /*m/s*/
      || wind_gust  >= 40.2 /*m/s*/);
}

/* Takes the current weather and today's daily weather forcast (from
 * OpenWeatherMap API response) and returns a pointer to the icon's 196x196
 * bitmap.
 *
 * Uses multiple factors to return more detailed icons than the simple icon
 * catagories that OpenWeatherMap provides.
 *
 * Last Updated: June 26, 2022
 *
 * References:
 *   https://openweathermap.org/weather-conditions
 */
template <int BitmapSize>
const uint8_t *getConditionsBitmap(int id, bool day, bool moon, bool cloudy,
                                   bool windy)
{
  switch (id)
  {
  // Group 2xx: Thunderstorm
  case 200: // Thunderstorm  thunderstorm with light rain     11d
  case 201: // Thunderstorm  thunderstorm with rain           11d
  case 202: // Thunderstorm  thunderstorm with heavy rain     11d
  case 210: // Thunderstorm  light thunderstorm               11d
  case 211: // Thunderstorm  thunderstorm                     11d
  case 212: // Thunderstorm  heavy thunderstorm               11d
  case 221: // Thunderstorm  ragged thunderstorm              11d
    if (!cloudy && day)          {return getBitmap(wi_day_thunderstorm, BitmapSize);}
    if (!cloudy && !day && moon) {return getBitmap(wi_night_alt_thunderstorm, BitmapSize);}
    return getBitmap(wi_thunderstorm, BitmapSize);
  case 230: // Thunderstorm  thunderstorm with light drizzle  11d
  case 231: // Thunderstorm  thunderstorm with drizzle        11d
  case 232: // Thunderstorm  thunderstorm with heavy drizzle  11d
    if (!cloudy && day)          {return getBitmap(wi_day_storm_showers, BitmapSize);}
    if (!cloudy && !day && moon) {return getBitmap(wi_night_alt_storm_showers, BitmapSize);}
    return getBitmap(wi_storm_showers, BitmapSize);
  // Group 3xx: Drizzle
  case 300: // Drizzle       light intensity drizzle          09d
  case 301: // Drizzle       drizzle                          09d
  case 302: // Drizzle       heavy intensity drizzle          09d
  case 310: // Drizzle       light intensity drizzle rain     09d
  case 311: // Drizzle       drizzle rain                     09d
  case 312: // Drizzle       heavy intensity drizzle rain     09d
  case 313: // Drizzle       shower rain and drizzle          09d
  case 314: // Drizzle       heavy shower rain and drizzle    09d
  case 321: // Drizzle       shower drizzle                   09d
    if (!cloudy && day)          {return getBitmap(wi_day_showers, BitmapSize);}
    if (!cloudy && !day && moon) {return getBitmap(wi_night_alt_showers, BitmapSize);}
    return getBitmap(wi_showers, BitmapSize);
  // Group 5xx: Rain
  case 500: // Rain          light rain                       10d
  case 501: // Rain          moderate rain                    10d
  case 502: // Rain          heavy intensity rain             10d
  case 503: // Rain          very heavy rain                  10d
  case 504: // Rain          extreme rain                     10d
    if (!cloudy && day && windy)          {return getBitmap(wi_day_rain_wind, BitmapSize);}
    if (!cloudy && day)                   {return getBitmap(wi_day_rain, BitmapSize);}
    if (!cloudy && !day && moon && windy) {return getBitmap(wi_night_alt_rain_wind, BitmapSize);}
    if (!cloudy && !day && moon)          {return getBitmap(wi_night_alt_rain, BitmapSize);}
    if (windy)                            {return getBitmap(wi_rain_wind, BitmapSize);}
    return getBitmap(wi_rain, BitmapSize);
  case 511: // Rain          freezing rain                    13d
    if (!cloudy && day)          {return getBitmap(wi_day_rain_mix, BitmapSize);}
    if (!cloudy && !day && moon) {return getBitmap(wi_night_alt_rain_mix, BitmapSize);}
    return getBitmap(wi_rain_mix, BitmapSize);
  case 520: // Rain          light intensity shower rain      09d
  case 521: // Rain          shower rain                      09d
  case 522: // Rain          heavy intensity shower rain      09d
  case 531: // Rain          ragged shower rain               09d
    if (!cloudy && day)          {return getBitmap(wi_day_showers, BitmapSize);}
    if (!cloudy && !day && moon) {return getBitmap(wi_night_alt_showers, BitmapSize);}
    return getBitmap(wi_showers, BitmapSize);
  // Group 6xx: Snow
  case 600: // Snow          light snow                       13d
  case 601: // Snow          Snow                             13d
  case 602: // Snow          Heavy snow                       13d
    if (!cloudy && day && windy)          {return getBitmap(wi_day_snow_wind, BitmapSize);}
    if (!cloudy && day)                   {return getBitmap(wi_day_snow, BitmapSize);}
    if (!cloudy && !day && moon && windy) {return getBitmap(wi_night_alt_snow_wind, BitmapSize);}
    if (!cloudy && !day && moon)          {return getBitmap(wi_night_alt_snow, BitmapSize);}
    if (windy)                            {return getBitmap(wi_snow_wind, BitmapSize);}
    return getBitmap(wi_snow, BitmapSize);
  case 611: // Snow          Sleet                            13d
  case 612: // Snow          Light shower sleet               13d
  case 613: // Snow          Shower sleet                     13d
    if (!cloudy && day)          {return getBitmap(wi_day_sleet, BitmapSize);}
    if (!cloudy && !day && moon) {return getBitmap(wi_night_alt_sleet, BitmapSize);}
    return getBitmap(wi_sleet, BitmapSize);
  case 615: // Snow          Light rain and snow              13d
  case 616: // Snow          Rain and snow                    13d
  case 620: // Snow          Light shower snow                13d
  case 621: // Snow          Shower snow                      13d
  case 622: // Snow          Heavy shower snow                13d
    if (!cloudy && day)          {return getBitmap(wi_day_rain_mix, BitmapSize);}
    if (!cloudy && !day && moon) {return getBitmap(wi_night_alt_rain_mix, BitmapSize);}
    return getBitmap(wi_rain_mix, BitmapSize);
  // Group 7xx: Atmosphere
  case 701: // Mist          mist                             50d
    if (!cloudy && day)          {return getBitmap(wi_day_fog, BitmapSize);}
    if (!cloudy && !day && moon) {return getBitmap(wi_night_fog, BitmapSize);}
    return getBitmap(wi_fog, BitmapSize);
  case 711: // Smoke         Smoke                            50d
    return getBitmap(wi_smoke, BitmapSize);
  case 721: // Haze          Haze                             50d
    if (day && !cloudy) {return getBitmap(wi_day_haze, BitmapSize);}
    return getBitmap(wi_dust, BitmapSize);
  case 731: // Dust          sand/dust whirls                 50d
    return getBitmap(wi_sandstorm, BitmapSize);
  case 741: // Fog           fog                              50d
    if (!cloudy && day)          {return getBitmap(wi_day_fog, BitmapSize);}
    if (!cloudy && !day && moon) {return getBitmap(wi_night_fog, BitmapSize);}
    return getBitmap(wi_fog, BitmapSize);
  case 751: // Sand          sand                             50d
    return getBitmap(wi_sandstorm, BitmapSize);
  case 761: // Dust          dust                             50d
    return getBitmap(wi_dust, BitmapSize);
  case 762: // Ash           volcanic ash                     50d
    return getBitmap(wi_volcano, BitmapSize);
  case 771: // Squall        squalls                          50d
    return getBitmap(wi_cloudy_gusts, BitmapSize);
  case 781: // Tornado       tornado                          50d
    return getBitmap(wi_tornado, BitmapSize);
  // Group 800: Clear
  case 800: // Clear         clear sky                        01d 01n
    if (windy)         {return getBitmap(wi_strong_wind, BitmapSize);}
    if (!day && moon)  {return getBitmap(wi_night_clear, BitmapSize);}
    if (!day && !moon) {return getBitmap(wi_stars, BitmapSize);}
    return getBitmap(wi_day_sunny, BitmapSize);
  // Group 80x: Clouds
  case 801: // Clouds        few clouds: 11-25%               02d 02n
    if (windy)         {return getBitmap(wi_strong_wind, BitmapSize);}
    if (!day && moon)  {return getBitmap(wi_night_alt_partly_cloudy, BitmapSize);}
    if (!day && !moon) {return getBitmap(wi_stars, BitmapSize);}
    return getBitmap(wi_day_sunny_overcast, BitmapSize);
  case 802: // Clouds        scattered clouds: 25-50%         03d 03n
  case 803: // Clouds        broken clouds: 51-84%            04d 04n
    if (windy && day)           {return getBitmap(wi_day_cloudy_gusts, BitmapSize);}
    if (windy && !day && moon)  {return getBitmap(wi_night_alt_cloudy_gusts, BitmapSize);}
    if (windy && !day && !moon) {return getBitmap(wi_cloudy_gusts, BitmapSize);}
    if (!day && moon)           {return getBitmap(wi_night_alt_cloudy, BitmapSize);}
    if (!day && !moon)          {return getBitmap(wi_cloud, BitmapSize);}
    return getBitmap(wi_day_cloudy, BitmapSize);
  case 804: // Clouds        overcast clouds: 85-100%         04d 04n
    if (windy) {return getBitmap(wi_cloudy_gusts, BitmapSize);}
    return getBitmap(wi_cloudy, BitmapSize);
  default:
    // maybe this is a new getBitmap in one of the existing groups
    if (id >= 200 && id < 300) {return getBitmap(wi_thunderstorm, BitmapSize);}
    if (id >= 300 && id < 400) {return getBitmap(wi_showers, BitmapSize);}
    if (id >= 500 && id < 600) {return getBitmap(wi_rain, BitmapSize);}
    if (id >= 600 && id < 700) {return getBitmap(wi_snow, BitmapSize);}
    if (id >= 700 && id < 800) {return getBitmap(wi_fog, BitmapSize);}
    if (id >= 800 && id < 900) {return getBitmap(wi_cloudy, BitmapSize);}
    return getBitmap(wi_na, BitmapSize);
  }
} // end getConditionsBitmap

/* Takes the daily weather forecast (from OpenWeatherMap API response) and
 * returns a pointer to the icon's 32x32 bitmap.
 *
 * The daily weather forcast of today is needed for moonrise and moonset times.
 */
const uint8_t *getHourlyForecastBitmap32(const owm_hourly_t &hourly,
                                         const owm_daily_t  &today)
{
  const int id = hourly.weather.id;
  const bool day = isDay(hourly.weather.icon);
  const bool moon = isMoonInSky(hourly.dt, today.moonrise, today.moonset,
                                today.moon_phase);
  const bool cloudy = isCloudy(hourly.clouds);
  const bool windy = isWindy(hourly.wind_speed, hourly.wind_gust);
  return getConditionsBitmap<32>(id, day, moon, cloudy, windy);
}

/* Takes the daily weather forecast (from OpenWeatherMap API response) and
 * returns a pointer to the icon's 64x64 bitmap.
 */
const uint8_t *getDailyForecastBitmap64(const owm_daily_t &daily)
{
  const int id = daily.weather.id;
  // always show daytime icon for daily forecast
  const bool day = true;
  const bool moon = false;
  const bool cloudy = isCloudy(daily.clouds);
  const bool windy = isWindy(daily.wind_speed, daily.wind_gust);
  return getConditionsBitmap<64>(id, day, moon, cloudy, windy);
} // end getForecastBitmap64

/* Takes the current weather and today's daily weather forcast (from
 * OpenWeatherMap API response) and returns a pointer to the icon's 196x196
 * bitmap.
 * 
 * The daily weather forcast of today is needed for moonrise and moonset times.
 */
const uint8_t *getCurrentConditionsBitmap196(const owm_current_t &current,
                                             const owm_daily_t   &today)
{
  const int id = current.weather.id;
  const bool day = isDay(current.weather.icon);
  const bool moon = isMoonInSky(current.dt, today.moonrise, today.moonset,
                                today.moon_phase);
  const bool cloudy = isCloudy(current.clouds);
  const bool windy = isWindy(current.wind_speed, current.wind_gust);
  return getConditionsBitmap<196>(id, day, moon, cloudy, windy);
} // end getCurrentConditionsBitmap196

/* Returns a 32x32 bitmap for a given alert.
 *
 * The purpose of this function is to return a relevant bitmap for an alert.
 * This is done by searching the event text for key terminology defined in the
 * included locale header.
 * If a relevant category can not be determined, the default alert bitmap will
 * be returned. (warning triangle icon)
 */
const uint8_t *getAlertBitmap32(const owm_alerts_t &alert)
{
  enum alert_category c = getAlertCategory(alert);
  switch (c)
  {
  // this is the default if an alert wasn't associated with a catagory
  case NOT_FOUND:            return warning_icon_32x32;

  case SMOG:                 return wi_smog_32x32;
  case SMOKE:                return wi_smoke_32x32;
  case FOG:                  return wi_fog_32x32;
  case METEOR:               return wi_meteor_32x32;
  case NUCLEAR:              return ionizing_radiation_symbol_32x32;
  case BIOHAZARD:            return biological_hazard_symbol_32x32;
  case EARTHQUAKE:           return wi_earthquake_32x32;
  case TSUNAMI:              return wi_tsunami_32x32;
  case FIRE:                 return wi_fire_32x32;
  case HEAT:                 return wi_thermometer_32x32;
  case WINTER:               return wi_snowflake_cold_32x32;
  case LIGHTNING:            return wi_lightning_32x32;
  case SANDSTORM:            return wi_sandstorm_32x32;
  case FLOOD:                return wi_flood_32x32;
  case VOLCANO:              return wi_volcano_32x32;
  case AIR_QUALITY:          return wi_dust_32x32;
  case TORNADO:              return wi_tornado_32x32;
  case SMALL_CRAFT_ADVISORY: return wi_small_craft_advisory_32x32;
  case GALE_WARNING:         return wi_gale_warning_32x32;
  case STORM_WARNING:        return wi_storm_warning_32x32;
  case HURRICANE_WARNING:    return wi_hurricane_warning_32x32;
  case HURRICANE:            return wi_hurricane_32x32;
  case DUST:                 return wi_dust_32x32;
  case STRONG_WIND:          return wi_strong_wind_32x32;

  // this code will never be reached
  default:                   return wi_na_48x48;
  }
} // end getAlertBitmap32

/* Returns a 48x48 bitmap for a given alert.
 *
 * The purpose of this function is to return a relevant bitmap for an alert.
 * This is done by searching the event text for key terminology defined in the
 * included locale header.
 * If a relevant category can not be determined, the default alert bitmap will
 * be returned. (warning triangle icon)
 */
const uint8_t *getAlertBitmap48(const owm_alerts_t &alert)
{
  enum alert_category c = getAlertCategory(alert);
  switch (c)
  {
  // this is the default if an alert wasn't associated with a catagory
  case NOT_FOUND:            return warning_icon_48x48;

  case SMOG:                 return wi_smog_48x48;
  case SMOKE:                return wi_smoke_48x48;
  case FOG:                  return wi_fog_48x48;
  case METEOR:               return wi_meteor_48x48;
  case NUCLEAR:              return ionizing_radiation_symbol_48x48;
  case BIOHAZARD:            return biological_hazard_symbol_48x48;
  case EARTHQUAKE:           return wi_earthquake_48x48;
  case TSUNAMI:              return wi_tsunami_48x48;
  case FIRE:                 return wi_fire_48x48;
  case HEAT:                 return wi_thermometer_48x48;
  case WINTER:               return wi_snowflake_cold_48x48;
  case LIGHTNING:            return wi_lightning_48x48;
  case SANDSTORM:            return wi_sandstorm_48x48;
  case FLOOD:                return wi_flood_48x48;
  case VOLCANO:              return wi_volcano_48x48;
  case AIR_QUALITY:          return wi_dust_48x48;
  case TORNADO:              return wi_tornado_48x48;
  case SMALL_CRAFT_ADVISORY: return wi_small_craft_advisory_48x48;
  case GALE_WARNING:         return wi_gale_warning_48x48;
  case STORM_WARNING:        return wi_storm_warning_48x48;
  case HURRICANE_WARNING:    return wi_hurricane_warning_48x48;
  case HURRICANE:            return wi_hurricane_48x48;
  case DUST:                 return wi_dust_48x48;
  case STRONG_WIND:          return wi_strong_wind_48x48;

  // this code will never be reached
  default:                   return wi_na_48x48;
  }
} // end getAlertBitmap48

/* Returns true of a String, s, contains any of the strings in the terminology
 * vector.
 *
 * Note: This function is case sensitive.
 */
bool containsTerminology(const String s, const std::vector<String> &terminology)
{
  for (const String &term : terminology)
  {
    if (s.indexOf(term) >= 0)
    {
      return true;
    }
  }
  return false;
} // end containsTerminology

/* Returns the category of an alert based on the terminology found in the event
 * name.
 *
 * Weather alert terminology is defined in the included locale header.
 */
enum alert_category getAlertCategory(const owm_alerts_t &alert)
{
  if (containsTerminology(alert.event, TERM_SMOG))
  {
    return alert_category::SMOG;
  }
  if (containsTerminology(alert.event, TERM_SMOKE))
  {
    return alert_category::SMOKE;
  }
  if (containsTerminology(alert.event, TERM_FOG))
  {
    return alert_category::FOG;
  }
  if (containsTerminology(alert.event, TERM_METEOR))
  {
    return alert_category::METEOR;
  }
  if (containsTerminology(alert.event, TERM_NUCLEAR))
  {
    return alert_category::NUCLEAR;
  }
  if (containsTerminology(alert.event, TERM_BIOHAZARD))
  {
    return alert_category::BIOHAZARD;
  }
  if (containsTerminology(alert.event, TERM_EARTHQUAKE))
  {
    return alert_category::EARTHQUAKE;
  }
  if (containsTerminology(alert.event, TERM_FIRE))
  {
    return alert_category::FIRE;
  }
  if (containsTerminology(alert.event, TERM_HEAT))
  {
    return alert_category::HEAT;
  }
  if (containsTerminology(alert.event, TERM_WINTER))
  {
    return alert_category::WINTER;
  }
  if (containsTerminology(alert.event, TERM_TSUNAMI))
  {
    return alert_category::TSUNAMI;
  }
  if (containsTerminology(alert.event, TERM_LIGHTNING))
  {
    return alert_category::LIGHTNING;
  }
  if (containsTerminology(alert.event, TERM_SANDSTORM))
  {
    return alert_category::SANDSTORM;
  }
  if (containsTerminology(alert.event, TERM_FLOOD))
  {
    return alert_category::FLOOD;
  }
  if (containsTerminology(alert.event, TERM_VOLCANO))
  {
    return alert_category::VOLCANO;
  }
  if (containsTerminology(alert.event, TERM_AIR_QUALITY))
  {
    return alert_category::AIR_QUALITY;
  }
  if (containsTerminology(alert.event, TERM_TORNADO))
  {
    return alert_category::TORNADO;
  }
  if (containsTerminology(alert.event, TERM_SMALL_CRAFT_ADVISORY))
  {
    return alert_category::SMALL_CRAFT_ADVISORY;
  }
  if (containsTerminology(alert.event, TERM_GALE_WARNING))
  {
    return alert_category::GALE_WARNING;
  }
  if (containsTerminology(alert.event, TERM_STORM_WARNING))
  {
    return alert_category::STORM_WARNING;
  }
  if (containsTerminology(alert.event, TERM_HURRICANE_WARNING))
  {
    return alert_category::HURRICANE_WARNING;
  }
  if (containsTerminology(alert.event, TERM_HURRICANE))
  {
    return alert_category::HURRICANE;
  }
  if (containsTerminology(alert.event, TERM_DUST))
  {
    return alert_category::DUST;
  }
  if (containsTerminology(alert.event, TERM_STRONG_WIND))
  {
    return alert_category::STRONG_WIND;
  }
  return alert_category::NOT_FOUND;
} // end getAlertCategory

#ifdef WIND_ICONS_CARDINAL
static const unsigned char *wind_direction_icon_arr[] = {
  wind_direction_meteorological_0deg_24x24,    // N
  wind_direction_meteorological_90deg_24x24,   // E
  wind_direction_meteorological_180deg_24x24,  // S
  wind_direction_meteorological_270deg_24x24}; // W
#endif // end WIND_ICONS_CARDINAL
#ifdef WIND_ICONS_INTERCARDINAL
static const unsigned char *wind_direction_icon_arr[] = {
  wind_direction_meteorological_0deg_24x24,    // N
  wind_direction_meteorological_45deg_24x24,   // NE
  wind_direction_meteorological_90deg_24x24,   // E
  wind_direction_meteorological_135deg_24x24,  // SE
  wind_direction_meteorological_180deg_24x24,  // S
  wind_direction_meteorological_225deg_24x24,  // SW
  wind_direction_meteorological_270deg_24x24,  // W
  wind_direction_meteorological_315deg_24x24}; // NW
#endif // end WIND_ICONS_INTERCARDINAL
#ifdef WIND_ICONS_SECONDARY_INTERCARDINAL
static const unsigned char *wind_direction_icon_arr[] = {
  wind_direction_meteorological_0deg_24x24,      // N
  wind_direction_meteorological_22_5deg_24x24,   // NNE
  wind_direction_meteorological_45deg_24x24,     // NE
  wind_direction_meteorological_67_5deg_24x24,   // ENE
  wind_direction_meteorological_90deg_24x24,     // E
  wind_direction_meteorological_112_5deg_24x24,  // ESE
  wind_direction_meteorological_135deg_24x24,    // SE
  wind_direction_meteorological_157_5deg_24x24,  // SSE
  wind_direction_meteorological_180deg_24x24,    // S
  wind_direction_meteorological_202_5deg_24x24,  // SSW
  wind_direction_meteorological_225deg_24x24,    // SW
  wind_direction_meteorological_247_5deg_24x24,  // WSW
  wind_direction_meteorological_270deg_24x24,    // W
  wind_direction_meteorological_292_5deg_24x24,  // WNW
  wind_direction_meteorological_315deg_24x24,    // NW
  wind_direction_meteorological_337_5deg_24x24}; // NNW
#endif // end WIND_ICONS_SECONDARY_INTERCARDINAL
#ifdef WIND_ICONS_TERTIARY_INTERCARDINAL
static const unsigned char *wind_direction_icon_arr[] = {
  wind_direction_meteorological_0deg_24x24,       // N
  wind_direction_meteorological_11_25deg_24x24,   // NbE
  wind_direction_meteorological_22_5deg_24x24,    // NNE
  wind_direction_meteorological_33_75deg_24x24,   // NEbN
  wind_direction_meteorological_45deg_24x24,      // NE
  wind_direction_meteorological_56_25deg_24x24,   // NEbE
  wind_direction_meteorological_67_5deg_24x24,    // ENE
  wind_direction_meteorological_78_75deg_24x24,   // EbN
  wind_direction_meteorological_90deg_24x24,      // E
  wind_direction_meteorological_101_25deg_24x24,  // EbS
  wind_direction_meteorological_112_5deg_24x24,   // ESE
  wind_direction_meteorological_123_75deg_24x24,  // SEbE
  wind_direction_meteorological_135deg_24x24,     // SE
  wind_direction_meteorological_146_25deg_24x24,  // SEbS
  wind_direction_meteorological_157_5deg_24x24,   // SSE
  wind_direction_meteorological_168_75deg_24x24,  // SbE
  wind_direction_meteorological_180deg_24x24,     // S
  wind_direction_meteorological_191_25deg_24x24,  // SbW
  wind_direction_meteorological_202_5deg_24x24,   // SSW
  wind_direction_meteorological_213_75deg_24x24,  // SWbS
  wind_direction_meteorological_225deg_24x24,     // SW
  wind_direction_meteorological_236_25deg_24x24,  // SWbW
  wind_direction_meteorological_247_5deg_24x24,   // WSW
  wind_direction_meteorological_258_75deg_24x24,  // WbS
  wind_direction_meteorological_270deg_24x24,     // W
  wind_direction_meteorological_281_25deg_24x24,  // WbN
  wind_direction_meteorological_292_5deg_24x24,   // WNW
  wind_direction_meteorological_303_75deg_24x24,  // NWbW
  wind_direction_meteorological_315deg_24x24,     // NW
  wind_direction_meteorological_326_25deg_24x24,  // NWbN
  wind_direction_meteorological_337_5deg_24x24,   // NNW
  wind_direction_meteorological_348_75deg_24x24}; // NbW
#endif // end WIND_ICONS_TERTIARY_INTERCARDINAL
#ifdef WIND_ICONS_360
static const unsigned char *wind_direction_icon_arr[] = {
  wind_direction_meteorological_0deg_24x24,
  wind_direction_meteorological_1deg_24x24,
  wind_direction_meteorological_2deg_24x24,
  wind_direction_meteorological_3deg_24x24,
  wind_direction_meteorological_4deg_24x24,
  wind_direction_meteorological_5deg_24x24,
  wind_direction_meteorological_6deg_24x24,
  wind_direction_meteorological_7deg_24x24,
  wind_direction_meteorological_8deg_24x24,
  wind_direction_meteorological_9deg_24x24,
  wind_direction_meteorological_10deg_24x24,
  wind_direction_meteorological_11deg_24x24,
  wind_direction_meteorological_12deg_24x24,
  wind_direction_meteorological_13deg_24x24,
  wind_direction_meteorological_14deg_24x24,
  wind_direction_meteorological_15deg_24x24,
  wind_direction_meteorological_16deg_24x24,
  wind_direction_meteorological_17deg_24x24,
  wind_direction_meteorological_18deg_24x24,
  wind_direction_meteorological_19deg_24x24,
  wind_direction_meteorological_20deg_24x24,
  wind_direction_meteorological_21deg_24x24,
  wind_direction_meteorological_22deg_24x24,
  wind_direction_meteorological_23deg_24x24,
  wind_direction_meteorological_24deg_24x24,
  wind_direction_meteorological_25deg_24x24,
  wind_direction_meteorological_26deg_24x24,
  wind_direction_meteorological_27deg_24x24,
  wind_direction_meteorological_28deg_24x24,
  wind_direction_meteorological_29deg_24x24,
  wind_direction_meteorological_30deg_24x24,
  wind_direction_meteorological_31deg_24x24,
  wind_direction_meteorological_32deg_24x24,
  wind_direction_meteorological_33deg_24x24,
  wind_direction_meteorological_34deg_24x24,
  wind_direction_meteorological_35deg_24x24,
  wind_direction_meteorological_36deg_24x24,
  wind_direction_meteorological_37deg_24x24,
  wind_direction_meteorological_38deg_24x24,
  wind_direction_meteorological_39deg_24x24,
  wind_direction_meteorological_40deg_24x24,
  wind_direction_meteorological_41deg_24x24,
  wind_direction_meteorological_42deg_24x24,
  wind_direction_meteorological_43deg_24x24,
  wind_direction_meteorological_44deg_24x24,
  wind_direction_meteorological_45deg_24x24,
  wind_direction_meteorological_46deg_24x24,
  wind_direction_meteorological_47deg_24x24,
  wind_direction_meteorological_48deg_24x24,
  wind_direction_meteorological_49deg_24x24,
  wind_direction_meteorological_50deg_24x24,
  wind_direction_meteorological_51deg_24x24,
  wind_direction_meteorological_52deg_24x24,
  wind_direction_meteorological_53deg_24x24,
  wind_direction_meteorological_54deg_24x24,
  wind_direction_meteorological_55deg_24x24,
  wind_direction_meteorological_56deg_24x24,
  wind_direction_meteorological_57deg_24x24,
  wind_direction_meteorological_58deg_24x24,
  wind_direction_meteorological_59deg_24x24,
  wind_direction_meteorological_60deg_24x24,
  wind_direction_meteorological_61deg_24x24,
  wind_direction_meteorological_62deg_24x24,
  wind_direction_meteorological_63deg_24x24,
  wind_direction_meteorological_64deg_24x24,
  wind_direction_meteorological_65deg_24x24,
  wind_direction_meteorological_66deg_24x24,
  wind_direction_meteorological_67deg_24x24,
  wind_direction_meteorological_68deg_24x24,
  wind_direction_meteorological_69deg_24x24,
  wind_direction_meteorological_70deg_24x24,
  wind_direction_meteorological_71deg_24x24,
  wind_direction_meteorological_72deg_24x24,
  wind_direction_meteorological_73deg_24x24,
  wind_direction_meteorological_74deg_24x24,
  wind_direction_meteorological_75deg_24x24,
  wind_direction_meteorological_76deg_24x24,
  wind_direction_meteorological_77deg_24x24,
  wind_direction_meteorological_78deg_24x24,
  wind_direction_meteorological_79deg_24x24,
  wind_direction_meteorological_80deg_24x24,
  wind_direction_meteorological_81deg_24x24,
  wind_direction_meteorological_82deg_24x24,
  wind_direction_meteorological_83deg_24x24,
  wind_direction_meteorological_84deg_24x24,
  wind_direction_meteorological_85deg_24x24,
  wind_direction_meteorological_86deg_24x24,
  wind_direction_meteorological_87deg_24x24,
  wind_direction_meteorological_88deg_24x24,
  wind_direction_meteorological_89deg_24x24,
  wind_direction_meteorological_90deg_24x24,
  wind_direction_meteorological_91deg_24x24,
  wind_direction_meteorological_92deg_24x24,
  wind_direction_meteorological_93deg_24x24,
  wind_direction_meteorological_94deg_24x24,
  wind_direction_meteorological_95deg_24x24,
  wind_direction_meteorological_96deg_24x24,
  wind_direction_meteorological_97deg_24x24,
  wind_direction_meteorological_98deg_24x24,
  wind_direction_meteorological_99deg_24x24,
  wind_direction_meteorological_100deg_24x24,
  wind_direction_meteorological_101deg_24x24,
  wind_direction_meteorological_102deg_24x24,
  wind_direction_meteorological_103deg_24x24,
  wind_direction_meteorological_104deg_24x24,
  wind_direction_meteorological_105deg_24x24,
  wind_direction_meteorological_106deg_24x24,
  wind_direction_meteorological_107deg_24x24,
  wind_direction_meteorological_108deg_24x24,
  wind_direction_meteorological_109deg_24x24,
  wind_direction_meteorological_110deg_24x24,
  wind_direction_meteorological_111deg_24x24,
  wind_direction_meteorological_112deg_24x24,
  wind_direction_meteorological_113deg_24x24,
  wind_direction_meteorological_114deg_24x24,
  wind_direction_meteorological_115deg_24x24,
  wind_direction_meteorological_116deg_24x24,
  wind_direction_meteorological_117deg_24x24,
  wind_direction_meteorological_118deg_24x24,
  wind_direction_meteorological_119deg_24x24,
  wind_direction_meteorological_120deg_24x24,
  wind_direction_meteorological_121deg_24x24,
  wind_direction_meteorological_122deg_24x24,
  wind_direction_meteorological_123deg_24x24,
  wind_direction_meteorological_124deg_24x24,
  wind_direction_meteorological_125deg_24x24,
  wind_direction_meteorological_126deg_24x24,
  wind_direction_meteorological_127deg_24x24,
  wind_direction_meteorological_128deg_24x24,
  wind_direction_meteorological_129deg_24x24,
  wind_direction_meteorological_130deg_24x24,
  wind_direction_meteorological_131deg_24x24,
  wind_direction_meteorological_132deg_24x24,
  wind_direction_meteorological_133deg_24x24,
  wind_direction_meteorological_134deg_24x24,
  wind_direction_meteorological_135deg_24x24,
  wind_direction_meteorological_136deg_24x24,
  wind_direction_meteorological_137deg_24x24,
  wind_direction_meteorological_138deg_24x24,
  wind_direction_meteorological_139deg_24x24,
  wind_direction_meteorological_140deg_24x24,
  wind_direction_meteorological_141deg_24x24,
  wind_direction_meteorological_142deg_24x24,
  wind_direction_meteorological_143deg_24x24,
  wind_direction_meteorological_144deg_24x24,
  wind_direction_meteorological_145deg_24x24,
  wind_direction_meteorological_146deg_24x24,
  wind_direction_meteorological_147deg_24x24,
  wind_direction_meteorological_148deg_24x24,
  wind_direction_meteorological_149deg_24x24,
  wind_direction_meteorological_150deg_24x24,
  wind_direction_meteorological_151deg_24x24,
  wind_direction_meteorological_152deg_24x24,
  wind_direction_meteorological_153deg_24x24,
  wind_direction_meteorological_154deg_24x24,
  wind_direction_meteorological_155deg_24x24,
  wind_direction_meteorological_156deg_24x24,
  wind_direction_meteorological_157deg_24x24,
  wind_direction_meteorological_158deg_24x24,
  wind_direction_meteorological_159deg_24x24,
  wind_direction_meteorological_160deg_24x24,
  wind_direction_meteorological_161deg_24x24,
  wind_direction_meteorological_162deg_24x24,
  wind_direction_meteorological_163deg_24x24,
  wind_direction_meteorological_164deg_24x24,
  wind_direction_meteorological_165deg_24x24,
  wind_direction_meteorological_166deg_24x24,
  wind_direction_meteorological_167deg_24x24,
  wind_direction_meteorological_168deg_24x24,
  wind_direction_meteorological_169deg_24x24,
  wind_direction_meteorological_170deg_24x24,
  wind_direction_meteorological_171deg_24x24,
  wind_direction_meteorological_172deg_24x24,
  wind_direction_meteorological_173deg_24x24,
  wind_direction_meteorological_174deg_24x24,
  wind_direction_meteorological_175deg_24x24,
  wind_direction_meteorological_176deg_24x24,
  wind_direction_meteorological_177deg_24x24,
  wind_direction_meteorological_178deg_24x24,
  wind_direction_meteorological_179deg_24x24,
  wind_direction_meteorological_180deg_24x24,
  wind_direction_meteorological_181deg_24x24,
  wind_direction_meteorological_182deg_24x24,
  wind_direction_meteorological_183deg_24x24,
  wind_direction_meteorological_184deg_24x24,
  wind_direction_meteorological_185deg_24x24,
  wind_direction_meteorological_186deg_24x24,
  wind_direction_meteorological_187deg_24x24,
  wind_direction_meteorological_188deg_24x24,
  wind_direction_meteorological_189deg_24x24,
  wind_direction_meteorological_190deg_24x24,
  wind_direction_meteorological_191deg_24x24,
  wind_direction_meteorological_192deg_24x24,
  wind_direction_meteorological_193deg_24x24,
  wind_direction_meteorological_194deg_24x24,
  wind_direction_meteorological_195deg_24x24,
  wind_direction_meteorological_196deg_24x24,
  wind_direction_meteorological_197deg_24x24,
  wind_direction_meteorological_198deg_24x24,
  wind_direction_meteorological_199deg_24x24,
  wind_direction_meteorological_200deg_24x24,
  wind_direction_meteorological_201deg_24x24,
  wind_direction_meteorological_202deg_24x24,
  wind_direction_meteorological_203deg_24x24,
  wind_direction_meteorological_204deg_24x24,
  wind_direction_meteorological_205deg_24x24,
  wind_direction_meteorological_206deg_24x24,
  wind_direction_meteorological_207deg_24x24,
  wind_direction_meteorological_208deg_24x24,
  wind_direction_meteorological_209deg_24x24,
  wind_direction_meteorological_210deg_24x24,
  wind_direction_meteorological_211deg_24x24,
  wind_direction_meteorological_212deg_24x24,
  wind_direction_meteorological_213deg_24x24,
  wind_direction_meteorological_214deg_24x24,
  wind_direction_meteorological_215deg_24x24,
  wind_direction_meteorological_216deg_24x24,
  wind_direction_meteorological_217deg_24x24,
  wind_direction_meteorological_218deg_24x24,
  wind_direction_meteorological_219deg_24x24,
  wind_direction_meteorological_220deg_24x24,
  wind_direction_meteorological_221deg_24x24,
  wind_direction_meteorological_222deg_24x24,
  wind_direction_meteorological_223deg_24x24,
  wind_direction_meteorological_224deg_24x24,
  wind_direction_meteorological_225deg_24x24,
  wind_direction_meteorological_226deg_24x24,
  wind_direction_meteorological_227deg_24x24,
  wind_direction_meteorological_228deg_24x24,
  wind_direction_meteorological_229deg_24x24,
  wind_direction_meteorological_230deg_24x24,
  wind_direction_meteorological_231deg_24x24,
  wind_direction_meteorological_232deg_24x24,
  wind_direction_meteorological_233deg_24x24,
  wind_direction_meteorological_234deg_24x24,
  wind_direction_meteorological_235deg_24x24,
  wind_direction_meteorological_236deg_24x24,
  wind_direction_meteorological_237deg_24x24,
  wind_direction_meteorological_238deg_24x24,
  wind_direction_meteorological_239deg_24x24,
  wind_direction_meteorological_240deg_24x24,
  wind_direction_meteorological_241deg_24x24,
  wind_direction_meteorological_242deg_24x24,
  wind_direction_meteorological_243deg_24x24,
  wind_direction_meteorological_244deg_24x24,
  wind_direction_meteorological_245deg_24x24,
  wind_direction_meteorological_246deg_24x24,
  wind_direction_meteorological_247deg_24x24,
  wind_direction_meteorological_248deg_24x24,
  wind_direction_meteorological_249deg_24x24,
  wind_direction_meteorological_250deg_24x24,
  wind_direction_meteorological_251deg_24x24,
  wind_direction_meteorological_252deg_24x24,
  wind_direction_meteorological_253deg_24x24,
  wind_direction_meteorological_254deg_24x24,
  wind_direction_meteorological_255deg_24x24,
  wind_direction_meteorological_256deg_24x24,
  wind_direction_meteorological_257deg_24x24,
  wind_direction_meteorological_258deg_24x24,
  wind_direction_meteorological_259deg_24x24,
  wind_direction_meteorological_260deg_24x24,
  wind_direction_meteorological_261deg_24x24,
  wind_direction_meteorological_262deg_24x24,
  wind_direction_meteorological_263deg_24x24,
  wind_direction_meteorological_264deg_24x24,
  wind_direction_meteorological_265deg_24x24,
  wind_direction_meteorological_266deg_24x24,
  wind_direction_meteorological_267deg_24x24,
  wind_direction_meteorological_268deg_24x24,
  wind_direction_meteorological_269deg_24x24,
  wind_direction_meteorological_270deg_24x24,
  wind_direction_meteorological_271deg_24x24,
  wind_direction_meteorological_272deg_24x24,
  wind_direction_meteorological_273deg_24x24,
  wind_direction_meteorological_274deg_24x24,
  wind_direction_meteorological_275deg_24x24,
  wind_direction_meteorological_276deg_24x24,
  wind_direction_meteorological_277deg_24x24,
  wind_direction_meteorological_278deg_24x24,
  wind_direction_meteorological_279deg_24x24,
  wind_direction_meteorological_280deg_24x24,
  wind_direction_meteorological_281deg_24x24,
  wind_direction_meteorological_282deg_24x24,
  wind_direction_meteorological_283deg_24x24,
  wind_direction_meteorological_284deg_24x24,
  wind_direction_meteorological_285deg_24x24,
  wind_direction_meteorological_286deg_24x24,
  wind_direction_meteorological_287deg_24x24,
  wind_direction_meteorological_288deg_24x24,
  wind_direction_meteorological_289deg_24x24,
  wind_direction_meteorological_290deg_24x24,
  wind_direction_meteorological_291deg_24x24,
  wind_direction_meteorological_292deg_24x24,
  wind_direction_meteorological_293deg_24x24,
  wind_direction_meteorological_294deg_24x24,
  wind_direction_meteorological_295deg_24x24,
  wind_direction_meteorological_296deg_24x24,
  wind_direction_meteorological_297deg_24x24,
  wind_direction_meteorological_298deg_24x24,
  wind_direction_meteorological_299deg_24x24,
  wind_direction_meteorological_300deg_24x24,
  wind_direction_meteorological_301deg_24x24,
  wind_direction_meteorological_302deg_24x24,
  wind_direction_meteorological_303deg_24x24,
  wind_direction_meteorological_304deg_24x24,
  wind_direction_meteorological_305deg_24x24,
  wind_direction_meteorological_306deg_24x24,
  wind_direction_meteorological_307deg_24x24,
  wind_direction_meteorological_308deg_24x24,
  wind_direction_meteorological_309deg_24x24,
  wind_direction_meteorological_310deg_24x24,
  wind_direction_meteorological_311deg_24x24,
  wind_direction_meteorological_312deg_24x24,
  wind_direction_meteorological_313deg_24x24,
  wind_direction_meteorological_314deg_24x24,
  wind_direction_meteorological_315deg_24x24,
  wind_direction_meteorological_316deg_24x24,
  wind_direction_meteorological_317deg_24x24,
  wind_direction_meteorological_318deg_24x24,
  wind_direction_meteorological_319deg_24x24,
  wind_direction_meteorological_320deg_24x24,
  wind_direction_meteorological_321deg_24x24,
  wind_direction_meteorological_322deg_24x24,
  wind_direction_meteorological_323deg_24x24,
  wind_direction_meteorological_324deg_24x24,
  wind_direction_meteorological_325deg_24x24,
  wind_direction_meteorological_326deg_24x24,
  wind_direction_meteorological_327deg_24x24,
  wind_direction_meteorological_328deg_24x24,
  wind_direction_meteorological_329deg_24x24,
  wind_direction_meteorological_330deg_24x24,
  wind_direction_meteorological_331deg_24x24,
  wind_direction_meteorological_332deg_24x24,
  wind_direction_meteorological_333deg_24x24,
  wind_direction_meteorological_334deg_24x24,
  wind_direction_meteorological_335deg_24x24,
  wind_direction_meteorological_336deg_24x24,
  wind_direction_meteorological_337deg_24x24,
  wind_direction_meteorological_338deg_24x24,
  wind_direction_meteorological_339deg_24x24,
  wind_direction_meteorological_340deg_24x24,
  wind_direction_meteorological_341deg_24x24,
  wind_direction_meteorological_342deg_24x24,
  wind_direction_meteorological_343deg_24x24,
  wind_direction_meteorological_344deg_24x24,
  wind_direction_meteorological_345deg_24x24,
  wind_direction_meteorological_346deg_24x24,
  wind_direction_meteorological_347deg_24x24,
  wind_direction_meteorological_348deg_24x24,
  wind_direction_meteorological_349deg_24x24,
  wind_direction_meteorological_350deg_24x24,
  wind_direction_meteorological_351deg_24x24,
  wind_direction_meteorological_352deg_24x24,
  wind_direction_meteorological_353deg_24x24,
  wind_direction_meteorological_354deg_24x24,
  wind_direction_meteorological_355deg_24x24,
  wind_direction_meteorological_356deg_24x24,
  wind_direction_meteorological_357deg_24x24,
  wind_direction_meteorological_358deg_24x24,
  wind_direction_meteorological_359deg_24x24};
#endif // end WIND_ICONS_360

/* Returns a 24x24 wind direction icon bitmap for angles 0 to 359 degrees
 * Parameter is meteorological wind direction, arrow points in the direction the
 * wind is going.
 */
const uint8_t *getWindBitmap24(int windDeg)
{
  windDeg %= 360; // enforce domain
  // number of directions
  int n = sizeof(wind_direction_icon_arr)
          / sizeof(wind_direction_icon_arr[0]);
  int arr_offset = (int) ( (windDeg + (360 / n / 2)) % 360 )
                         / ( 360 / (float) n );

  return wind_direction_icon_arr[arr_offset];
} // end getWindBitmap24

/* Returns a pointer to a string that expresses the Compass Point Notation (CPN)
 * of the given windDeg.
 *
 *   PRECISION                  #     ERROR   EXAMPLE
 *   Cardinal                   4  ±45.000°   E
 *   Intercardinal (Ordinal)    8  ±22.500°   NE
 *   Secondary Intercardinal   16  ±11.250°   NNE
 *   Tertiary Intercardinal    32   ±5.625°   NbE
 */
const char *getCompassPointNotation(int windDeg)
{
#if defined(WIND_INDICATOR_CPN_CARDINAL)
  const int precision = 4;
#elif defined(WIND_INDICATOR_CPN_INTERCARDINAL)
  const int precision = 8;
#elif defined(WIND_INDICATOR_CPN_SECONDARY_INTERCARDINAL)
  const int precision = 16;
#elif defined(WIND_INDICATOR_CPN_TERTIARY_INTERCARDINAL)
  const int precision = 32;
#else
  const int precision = 4;
#endif

  windDeg %= 360; // enforce domain
  int arr_offset = (int) (windDeg / ( 360 / (float) precision )) 
                         * ( 32 / precision) ;

  return COMPASS_POINT_NOTATION[arr_offset];
} // end getCompassPointNotation

/* This function returns a pointer to a string representing the meaning for a
 * HTTP response status code or an arduino client error code.
 * ArduinoJson DeserializationError codes are also included here and are given a
 * negative 100 offset to distinguish them from other client error codes.
 *
 * HTTP response status codes [100, 599]
 * https://developer.mozilla.org/en-US/docs/Web/HTTP/Status
 *
 * HTTP client errors [0, -255]
 * https://github.com/espressif/arduino-esp32/blob/master/libraries/HTTPClient/src/HTTPClient.h
 *
 * ArduinoJson DeserializationError codes [-256, -511]
 * https://arduinojson.org/v6/api/misc/deserializationerror/
 * 
 * WiFi Status codes [-512, -767]
 * https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/src/WiFiType.h
 */
const char *getHttpResponsePhrase(int code)
{
  switch (code)
  {
  // 1xx - Informational Responses
  case 100: return TXT_HTTP_RESPONSE_100;
  case 101: return TXT_HTTP_RESPONSE_101;
  case 102: return TXT_HTTP_RESPONSE_102;
  case 103: return TXT_HTTP_RESPONSE_103;

  // 2xx - Successful Responses
  case 200: return TXT_HTTP_RESPONSE_200;
  case 201: return TXT_HTTP_RESPONSE_201;
  case 202: return TXT_HTTP_RESPONSE_202;
  case 203: return TXT_HTTP_RESPONSE_203;
  case 204: return TXT_HTTP_RESPONSE_204;
  case 205: return TXT_HTTP_RESPONSE_205;
  case 206: return TXT_HTTP_RESPONSE_206;
  case 207: return TXT_HTTP_RESPONSE_207;
  case 208: return TXT_HTTP_RESPONSE_208;
  case 226: return TXT_HTTP_RESPONSE_226;

  // 3xx - Redirection Responses
  case 300: return TXT_HTTP_RESPONSE_300;
  case 301: return TXT_HTTP_RESPONSE_301;
  case 302: return TXT_HTTP_RESPONSE_302;
  case 303: return TXT_HTTP_RESPONSE_303;
  case 304: return TXT_HTTP_RESPONSE_304;
  case 305: return TXT_HTTP_RESPONSE_305;
  case 307: return TXT_HTTP_RESPONSE_307;
  case 308: return TXT_HTTP_RESPONSE_308;

  // 4xx - Client Error Responses
  case 400: return TXT_HTTP_RESPONSE_400;
  case 401: return TXT_HTTP_RESPONSE_401;
  case 402: return TXT_HTTP_RESPONSE_402;
  case 403: return TXT_HTTP_RESPONSE_403;
  case 404: return TXT_HTTP_RESPONSE_404;
  case 405: return TXT_HTTP_RESPONSE_405;
  case 406: return TXT_HTTP_RESPONSE_406;
  case 407: return TXT_HTTP_RESPONSE_407;
  case 408: return TXT_HTTP_RESPONSE_408;
  case 409: return TXT_HTTP_RESPONSE_409;
  case 410: return TXT_HTTP_RESPONSE_410;
  case 411: return TXT_HTTP_RESPONSE_411;
  case 412: return TXT_HTTP_RESPONSE_412;
  case 413: return TXT_HTTP_RESPONSE_413;
  case 414: return TXT_HTTP_RESPONSE_414;
  case 415: return TXT_HTTP_RESPONSE_415;
  case 416: return TXT_HTTP_RESPONSE_416;
  case 417: return TXT_HTTP_RESPONSE_417;
  case 418: return TXT_HTTP_RESPONSE_418;
  case 421: return TXT_HTTP_RESPONSE_421;
  case 422: return TXT_HTTP_RESPONSE_422;
  case 423: return TXT_HTTP_RESPONSE_423;
  case 424: return TXT_HTTP_RESPONSE_424;
  case 425: return TXT_HTTP_RESPONSE_425;
  case 426: return TXT_HTTP_RESPONSE_426;
  case 428: return TXT_HTTP_RESPONSE_428;
  case 429: return TXT_HTTP_RESPONSE_429;
  case 431: return TXT_HTTP_RESPONSE_431;
  case 451: return TXT_HTTP_RESPONSE_451;

  // 5xx - Server Error Responses
  case 500: return TXT_HTTP_RESPONSE_500;
  case 501: return TXT_HTTP_RESPONSE_501;
  case 502: return TXT_HTTP_RESPONSE_502;
  case 503: return TXT_HTTP_RESPONSE_503;
  case 504: return TXT_HTTP_RESPONSE_504;
  case 505: return TXT_HTTP_RESPONSE_505;
  case 506: return TXT_HTTP_RESPONSE_506;
  case 507: return TXT_HTTP_RESPONSE_507;
  case 508: return TXT_HTTP_RESPONSE_508;
  case 510: return TXT_HTTP_RESPONSE_510;
  case 511: return TXT_HTTP_RESPONSE_511;

  // HTTP client errors [0, -255]
  case HTTPC_ERROR_CONNECTION_REFUSED:  return TXT_HTTPC_ERROR_CONNECTION_REFUSED;
  case HTTPC_ERROR_SEND_HEADER_FAILED:  return TXT_HTTPC_ERROR_SEND_HEADER_FAILED;
  case HTTPC_ERROR_SEND_PAYLOAD_FAILED: return TXT_HTTPC_ERROR_SEND_PAYLOAD_FAILED;
  case HTTPC_ERROR_NOT_CONNECTED:       return TXT_HTTPC_ERROR_NOT_CONNECTED;
  case HTTPC_ERROR_CONNECTION_LOST:     return TXT_HTTPC_ERROR_CONNECTION_LOST;
  case HTTPC_ERROR_NO_STREAM:           return TXT_HTTPC_ERROR_NO_STREAM;
  case HTTPC_ERROR_NO_HTTP_SERVER:      return TXT_HTTPC_ERROR_NO_HTTP_SERVER;
  case HTTPC_ERROR_TOO_LESS_RAM:        return TXT_HTTPC_ERROR_TOO_LESS_RAM;
  case HTTPC_ERROR_ENCODING:            return TXT_HTTPC_ERROR_ENCODING;
  case HTTPC_ERROR_STREAM_WRITE:        return TXT_HTTPC_ERROR_STREAM_WRITE;
  case HTTPC_ERROR_READ_TIMEOUT:        return TXT_HTTPC_ERROR_READ_TIMEOUT;

  // ArduinoJson DeserializationError codes  [-256, -511]
  case -256 - (DeserializationError::Code::Ok):              return TXT_DESERIALIZATION_ERROR_OK;
  case -256 - (DeserializationError::Code::EmptyInput):      return TXT_DESERIALIZATION_ERROR_EMPTY_INPUT;
  case -256 - (DeserializationError::Code::IncompleteInput): return TXT_DESERIALIZATION_ERROR_INCOMPLETE_INPUT;
  case -256 - (DeserializationError::Code::InvalidInput):    return TXT_DESERIALIZATION_ERROR_INVALID_INPUT;
  case -256 - (DeserializationError::Code::NoMemory):        return TXT_DESERIALIZATION_ERROR_NO_MEMORY;
  case -256 - (DeserializationError::Code::TooDeep):         return TXT_DESERIALIZATION_ERROR_TOO_DEEP;

  // WiFi Status codes [-512, -767]
  case -512 - WL_NO_SHIELD:       return TXT_WL_NO_SHIELD;
  // case -512 - WL_STOPPED:       return TXT_WL_STOPPED; // future
  case -512 - WL_IDLE_STATUS:     return TXT_WL_IDLE_STATUS;
  case -512 - WL_NO_SSID_AVAIL:   return TXT_WL_NO_SSID_AVAIL;
  case -512 - WL_SCAN_COMPLETED:  return TXT_WL_SCAN_COMPLETED;
  case -512 - WL_CONNECTED:       return TXT_WL_CONNECTED;
  case -512 - WL_CONNECT_FAILED:  return TXT_WL_CONNECT_FAILED;
  case -512 - WL_CONNECTION_LOST: return TXT_WL_CONNECTION_LOST;
  case -512 - WL_DISCONNECTED:    return TXT_WL_DISCONNECTED;

  default:  return "";
  }
} // end getHttpResponsePhrase


/* This function returns a pointer to a string representing the meaning for a
 * WiFi status (wl_status_t).
 *
 * wl_status_t type definition
 * https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/src/WiFiType.h
 */
const char *getWifiStatusPhrase(wl_status_t status)
{
  switch (status)
  {
  case WL_NO_SHIELD:       return TXT_WL_NO_SHIELD;
  // case WL_STOPPED:       return TXT_WL_STOPPED; // future
  case WL_IDLE_STATUS:     return TXT_WL_IDLE_STATUS;
  case WL_NO_SSID_AVAIL:   return TXT_WL_NO_SSID_AVAIL;
  case WL_SCAN_COMPLETED:  return TXT_WL_SCAN_COMPLETED;
  case WL_CONNECTED:       return TXT_WL_CONNECTED;
  case WL_CONNECT_FAILED:  return TXT_WL_CONNECT_FAILED;
  case WL_CONNECTION_LOST: return TXT_WL_CONNECTION_LOST;
  case WL_DISCONNECTED:    return TXT_WL_DISCONNECTED;

  default:  return "";
  }
} // end getWifiStatusPhrase

/* This function sets the builtin LED to LOW and disables it even during deep
 * sleep.
 */
void disableBuiltinLED()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  gpio_hold_en(static_cast<gpio_num_t>(LED_BUILTIN));
  gpio_deep_sleep_hold_en();
  return;
} // end disableBuiltinLED
