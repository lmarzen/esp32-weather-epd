#include <vector>
#include <Arduino.h>

#include "api_response.h"
#include "config.h"
#include "display_utils.h"
#include LANGUAGE_HEADER

// icon header files
#include "icons/icons_32x32.h"
#include "icons/icons_48x48.h"
#include "icons/icons_64x64.h"
#include "icons/icons_196x196.h"

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
}

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
   && (text.charAt(i) != ',' 
    || text.charAt(i) != '.' 
    || text.charAt(i) != '('))
  {
    if (text.charAt(i) != ' ')
    {
      lastChar = i + 1;
    }
    ++i;
  }

  text = text.substring(0, lastChar);

  return;
}

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
int eventUrgency(String &event)
{
  int urgency_lvl = -1;
  for (int i = 0; i < ALERT_URGENCY.size(); ++i)
  {
    if (event.indexOf(ALERT_URGENCY[i]) > 0)
    {
      urgency_lvl = i;
    }
  }
  return urgency_lvl;
}

/* This algorithm filters alerts from the API responses to be displayed.
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
void filterAlerts(std::vector<owm_alerts_t> &resp)
{
  // Convert all event text and tags to lowercase.
  for (auto &alert : resp)
  {
    alert.event.toLowerCase();
    alert.tags.toLowerCase();
  }

  // Deduplicate alerts with the same first tag. Keeping only the most urgent
  // alerts of each tag and alerts who's urgency cannot be determined.
  for (auto it_a = resp.begin(); it_a != resp.end(); ++it_a)
  {
    if (it_a->tags.isEmpty())
    {
      continue; // urgency can not be determined so it remains in the list
    }

    for (auto it_b = resp.begin(); it_b != resp.end(); ++it_b)
    {
      if (it_a != it_b && it_a->tags == it_b->tags)
      {
        // comparing alerts of the same tag, removing the less urgent alert
        if (eventUrgency(it_a->event) >= eventUrgency(it_b->event))
        {
          resp.erase(it_b);
        }
      }
    }
  }

  // Save only the 2 most recent alerts
  while (resp.size() > 2)
  {
    resp.pop_back();
  }

  // Remove trailing/extraneous information
  for (auto &alert : resp)
  {
    truncateExtraAlertInfo(alert.event);
  }

  return;
}

/* Takes the daily weather forecast (from OpenWeatherMap API 
 * response) and returns a pointer to the icon's 64x64 bitmap.
 *
 * Uses multiple factors to return more detailed icons than the simple icon 
 * catagories that OpenWeatherMap provides.
 * 
 * Last Updated: June 26, 2022
 * 
 * References: 
 *   https://openweathermap.org/weather-conditions
 *   https://www.weather.gov/ajk/ForecastTerms
 */
const uint8_t *getForecastBitmap64(owm_daily_t &daily)
{
  int id = daily.weather.id;
  // always using the day icon for weather forecast
  // bool day = daily.weather.icon.charAt(daily.weather.icon.length() - 1) == 'd';
  bool cloudy = daily.clouds > 60.25; // partly cloudy / partly sunny
  bool windy = (UNITS == 'i' 
                      && (daily.wind_speed >= 20   || daily.wind_gust >= 25))
            || (UNITS == 'm' 
                      && (daily.wind_speed >= 32.2 || daily.wind_gust >= 40.2));

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
    if (!cloudy) {return wi_day_thunderstorm_64x64;}
    return wi_thunderstorm_64x64;
  case 230: // Thunderstorm  thunderstorm with light drizzle  11d
  case 231: // Thunderstorm  thunderstorm with drizzle        11d
  case 232: // Thunderstorm  thunderstorm with heavy drizzle  11d
    if (!cloudy) {return wi_day_storm_showers_64x64;}
    return wi_storm_showers_64x64;
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
    if (!cloudy) {return wi_day_showers_64x64;}
    return wi_showers_64x64;
  // Group 5xx: Rain
  case 500: // Rain          light rain                       10d
  case 501: // Rain          moderate rain                    10d
  case 502: // Rain          heavy intensity rain             10d
  case 503: // Rain          very heavy rain                  10d
  case 504: // Rain          extreme rain                     10d
    if (!cloudy && windy) {return wi_day_rain_wind_64x64;}
    if (!cloudy)          {return wi_day_rain_64x64;}
    if (windy)            {return wi_rain_wind_64x64;}
    return wi_rain_64x64;
  case 511: // Rain          freezing rain                    13d
    if (!cloudy) {return wi_day_rain_mix_64x64;}
    return wi_rain_mix_64x64;
  case 520: // Rain          light intensity shower rain      09d
  case 521: // Rain          shower rain                      09d
  case 522: // Rain          heavy intensity shower rain      09d
  case 531: // Rain          ragged shower rain               09d
    if (!cloudy) {return wi_day_showers_64x64;}
    return wi_showers_64x64;
  // Group 6xx: Snow
  case 600: // Snow          light snow                       13d
  case 601: // Snow          Snow                             13d
  case 602: // Snow          Heavy snow                       13d
    if (!cloudy && windy) {return wi_day_snow_wind_64x64;}
    if (!cloudy)          {return wi_day_snow_64x64;}
    if (windy)            {return wi_snow_wind_64x64;}
    return wi_snow_64x64;
  case 611: // Snow          Sleet                            13d
  case 612: // Snow          Light shower sleet               13d
  case 613: // Snow          Shower sleet                     13d
    if (!cloudy) {return wi_day_sleet_64x64;}
    return wi_sleet_64x64;
  case 615: // Snow          Light rain and snow              13d
  case 616: // Snow          Rain and snow                    13d
  case 620: // Snow          Light shower snow                13d
  case 621: // Snow          Shower snow                      13d
  case 622: // Snow          Heavy shower snow                13d
    if (!cloudy) {return wi_day_rain_mix_64x64;}
    return wi_rain_mix_64x64;
  // Group 7xx: Atmosphere
  case 701: // Mist          mist                             50d
    if (!cloudy) {return wi_day_fog_64x64;}
    return wi_fog_64x64;
  case 711: // Smoke         Smoke                            50d
    return wi_smoke_64x64;
  case 721: // Haze          Haze                             50d
    return wi_day_haze_64x64;
  case 731: // Dust          sand/dust whirls                 50d
    return wi_sandstorm_64x64;
  case 741: // Fog           fog                              50d
    if (!cloudy) {return wi_day_fog_64x64;}
    return wi_fog_64x64;
  case 751: // Sand          sand                             50d
    return wi_sandstorm_64x64;
  case 761: // Dust          dust                             50d
    return wi_dust_64x64;
  case 762: // Ash           volcanic ash                     50d
    return wi_volcano_64x64;
  case 771: // Squall        squalls                          50d
    return wi_cloudy_gusts_64x64;
  case 781: // Tornado       tornado                          50d
    return wi_tornado_64x64;
  // Group 800: Clear
  case 800: // Clear         clear sky                        01d 01n
    if (windy) {return wi_windy_64x64;}
    return wi_day_sunny_64x64;
  // Group 80x: Clouds
  case 801: // Clouds        few clouds: 11-25%               02d 02n
    if (windy) {return wi_day_cloudy_windy_64x64;}
    return wi_day_sunny_overcast_64x64;
  case 802: // Clouds        scattered clouds: 25-50%         03d 03n
  case 803: // Clouds        broken clouds: 51-84%            04d 04n
    if (windy) {return wi_day_cloudy_windy_64x64;}
    return wi_day_cloudy_64x64;
  case 804: // Clouds        overcast clouds: 85-100%         04d 04n
    if (windy) {return wi_cloudy_windy_64x64;}
    return wi_cloudy_64x64;
  default:
    // OpenWeatherMap maybe this is a new icon in one of the existing groups
    if (id >= 200 && id < 300) {return wi_thunderstorm_64x64;}
    if (id >= 300 && id < 400) {return wi_showers_64x64;}
    if (id >= 500 && id < 600) {return wi_rain_64x64;}
    if (id >= 600 && id < 700) {return wi_snow_64x64;}
    if (id >= 700 && id < 800) {return wi_fog_64x64;}
    if (id >= 800 && id < 900) {return wi_cloudy_64x64;}
    return wi_na_64x64;
  }
}

/* Takes the current weather (from OpenWeatherMap API response) and returns a 
 * pointer to the icon's 196x196 bitmap.
 *
 * Uses multiple factors to return more detailed icons than the simple icon 
 * catagories that OpenWeatherMap provides.
 * 
 * Last Updated: June 26, 2022
 * 
 * References: 
 *   https://openweathermap.org/weather-conditions
 *   https://www.weather.gov/ajk/ForecastTerms
 */
const uint8_t *getCurrentConditionsBitmap196(owm_current_t &current)
{
  int id = current.weather.id;
  bool day = current.weather.icon.charAt(
                                      current.weather.icon.length() - 1) == 'd';
  bool cloudy = current.clouds > 60.25; // partly cloudy / partly sunny
  bool windy = (UNITS == 'i' 
                  && (current.wind_speed >= 20   || current.wind_gust >= 25))
            || (UNITS == 'm' 
                  && (current.wind_speed >= 32.2 || current.wind_gust >= 40.2));

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
    if (!cloudy && day)  {return wi_day_thunderstorm_196x196;}
    if (!cloudy && !day) {return wi_night_alt_thunderstorm_196x196;}
    return wi_thunderstorm_196x196;
  case 230: // Thunderstorm  thunderstorm with light drizzle  11d
  case 231: // Thunderstorm  thunderstorm with drizzle        11d
  case 232: // Thunderstorm  thunderstorm with heavy drizzle  11d
    if (!cloudy && day)  {return wi_day_storm_showers_196x196;}
    if (!cloudy && !day) {return wi_night_alt_storm_showers_196x196;}
    return wi_storm_showers_196x196;
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
    if (!cloudy && day)  {return wi_day_showers_196x196;}
    if (!cloudy && !day) {return wi_night_alt_showers_196x196;}
    return wi_showers_196x196;
  // Group 5xx: Rain
  case 500: // Rain          light rain                       10d
  case 501: // Rain          moderate rain                    10d
  case 502: // Rain          heavy intensity rain             10d
  case 503: // Rain          very heavy rain                  10d
  case 504: // Rain          extreme rain                     10d
    if (!cloudy && day && windy)  {return wi_day_rain_wind_196x196;}
    if (!cloudy && day)           {return wi_day_rain_196x196;}
    if (!cloudy && !day && windy) {return wi_night_alt_rain_wind_196x196;}
    if (!cloudy && !day)          {return wi_night_alt_rain_196x196;}
    if (windy)                    {return wi_rain_wind_196x196;}
    return wi_rain_196x196;
  case 511: // Rain          freezing rain                    13d
    if (!cloudy && day)  {return wi_day_rain_mix_196x196;}
    if (!cloudy && !day) {return wi_night_alt_rain_mix_196x196;}
    return wi_rain_mix_196x196;
  case 520: // Rain          light intensity shower rain      09d
  case 521: // Rain          shower rain                      09d
  case 522: // Rain          heavy intensity shower rain      09d
  case 531: // Rain          ragged shower rain               09d
    if (!cloudy && day)  {return wi_day_showers_196x196;}
    if (!cloudy && !day) {return wi_night_alt_showers_196x196;}
    return wi_showers_196x196;
  // Group 6xx: Snow
  case 600: // Snow          light snow                       13d
  case 601: // Snow          Snow                             13d
  case 602: // Snow          Heavy snow                       13d
    if (!cloudy && day && windy)  {return wi_day_snow_wind_196x196;}
    if (!cloudy && day)           {return wi_day_snow_196x196;}
    if (!cloudy && !day && windy) {return wi_night_alt_snow_wind_196x196;}
    if (!cloudy && !day)          {return wi_night_alt_snow_196x196;}
    if (windy)                    {return wi_snow_wind_196x196;}
    return wi_snow_196x196;
  case 611: // Snow          Sleet                            13d
  case 612: // Snow          Light shower sleet               13d
  case 613: // Snow          Shower sleet                     13d
    if (!cloudy && day)  {return wi_day_sleet_196x196;}
    if (!cloudy && !day) {return wi_night_alt_sleet_196x196;}
    return wi_sleet_196x196;
  case 615: // Snow          Light rain and snow              13d
  case 616: // Snow          Rain and snow                    13d
  case 620: // Snow          Light shower snow                13d
  case 621: // Snow          Shower snow                      13d
  case 622: // Snow          Heavy shower snow                13d
    if (!cloudy && day)  {return wi_day_rain_mix_196x196;}
    if (!cloudy && !day) {return wi_night_alt_rain_mix_196x196;}
    return wi_rain_mix_196x196;
  // Group 7xx: Atmosphere
  case 701: // Mist          mist                             50d
    if (!cloudy && day)  {return wi_day_fog_196x196;}
    if (!cloudy && !day) {return wi_night_fog_196x196;}
    return wi_fog_196x196;
  case 711: // Smoke         Smoke                            50d
    return wi_smoke_196x196;
  case 721: // Haze          Haze                             50d
    return wi_day_haze_196x196;
  case 731: // Dust          sand/dust whirls                 50d
    return wi_sandstorm_196x196;
  case 741: // Fog           fog                              50d
    if (!cloudy && day)  {return wi_day_fog_196x196;}
    if (!cloudy && !day) {return wi_night_fog_196x196;}
    return wi_fog_196x196;
  case 751: // Sand          sand                             50d
    return wi_sandstorm_196x196;
  case 761: // Dust          dust                             50d
    return wi_dust_196x196;
  case 762: // Ash           volcanic ash                     50d
    return wi_volcano_196x196;
  case 771: // Squall        squalls                          50d
    return wi_cloudy_gusts_196x196;
  case 781: // Tornado       tornado                          50d
    return wi_tornado_196x196;
  // Group 800: Clear
  case 800: // Clear         clear sky                        01d 01n
    if (windy) {return wi_windy_196x196;}
    if (!day)  {return wi_night_clear_196x196;}
    return wi_day_sunny_196x196;
  // Group 80x: Clouds
  case 801: // Clouds        few clouds: 11-25%               02d 02n
    if (windy && day)  {return wi_day_cloudy_windy_196x196;}
    if (windy && !day) {return wi_night_alt_cloudy_windy_196x196;}
    if (!day)          {return wi_night_alt_partly_cloudy_196x196;}
    return wi_day_sunny_overcast_196x196;
  case 802: // Clouds        scattered clouds: 25-50%         03d 03n
  case 803: // Clouds        broken clouds: 51-84%            04d 04n
    if (windy && day)  {return wi_day_cloudy_windy_196x196;}
    if (windy && !day) {return wi_night_alt_cloudy_windy_196x196;}
    if (!day)          {return wi_night_alt_cloudy_196x196;}
    return wi_day_cloudy_196x196;
  case 804: // Clouds        overcast clouds: 85-100%         04d 04n
    if (windy) {return wi_cloudy_windy_196x196;}
    return wi_cloudy_196x196;
  default:
    // OpenWeatherMap maybe this is a new icon in one of the existing groups
    if (id >= 200 && id < 300) {return wi_thunderstorm_196x196;}
    if (id >= 300 && id < 400) {return wi_showers_196x196;}
    if (id >= 500 && id < 600) {return wi_rain_196x196;}
    if (id >= 600 && id < 700) {return wi_snow_196x196;}
    if (id >= 700 && id < 800) {return wi_fog_196x196;}
    if (id >= 800 && id < 900) {return wi_cloudy_196x196;}
    return wi_na_196x196;
  }
}

/* Returns a 32x32 bitmap for a given alert.
 *
 * The purpose of this function is to return a relevant bitmap for an alert.
 * This is done by searching the event text for key terminology defined in the 
 * included language header.
 * If a relevant category can not be determined, the default alert bitmap will 
 * be returned. (warning triangle icon)
 */
const uint8_t *getAlertBitmap32(owm_alerts_t &alert)
{
  enum alert_category c = getAlertCategory(alert);
  switch (c)
  {
  case NOT_FOUND:
    // this is the default if an alert wasn't associated with a catagory
    return warning_icon_32x32;
  case SMOG:
    return wi_smog_32x32;
  case SMOKE:
    return wi_smoke_32x32;
  case FOG:
    return wi_fog_32x32;
  case METEOR:
    return wi_meteor_32x32;
  case NUCLEAR:
    return ionizing_radiation_symbol_32x32;
  case BIOHAZARD:
    return biological_hazard_symbol_32x32;
  case EARTHQUAKE:
    return wi_earthquake_32x32;
  case TSUNAMI:
    return wi_tsunami_32x32;
  case FIRE:
    return wi_fire_32x32;
  case HEAT:
    return wi_fire_32x32;
  case WINTER:
    return wi_snowflake_cold_32x32;
  case LIGHTNING:
    return wi_lightning_32x32;
  case SANDSTORM:
    return wi_sandstorm_32x32;
  case FLOOD:
    return wi_flood_32x32;
  case VOLCANO:
    return wi_volcano_32x32;
  case AIR_QUALITY:
    return wi_dust_32x32;
  case TORNADO:
    return wi_tornado_32x32;
  case SMALL_CRAFT_ADVISORY:
    return wi_small_craft_advisory_32x32;
  case GALE_WARNING:
    return wi_gale_warning_32x32;
  case STORM_WARNING:
    return wi_storm_warning_32x32;
  case HURRICANE_WARNING:
    return wi_hurricane_warning_32x32;
  case HURRICANE:
    return wi_hurricane_32x32;
  case DUST:
    return wi_dust_32x32;
  case STRONG_WIND:
    return wi_strong_wind_32x32;
  }
  return NULL; // this return should never be reached
}

/* Returns a 48x48 bitmap for a given alert.
 *
 * The purpose of this function is to return a relevant bitmap for an alert.
 * This is done by searching the event text for key terminology defined in the 
 * included language header.
 * If a relevant category can not be determined, the default alert bitmap will 
 * be returned. (warning triangle icon)
 */
const uint8_t *getAlertBitmap48(owm_alerts_t &alert)
{
  enum alert_category c = getAlertCategory(alert);
  switch (c)
  {
  case NOT_FOUND:
    // this is the default if an alert wasn't associated with a catagory
    return warning_icon_48x48;
  case SMOG:
    return wi_smog_48x48;
  case SMOKE:
    return wi_smoke_48x48;
  case FOG:
    return wi_fog_48x48;
  case METEOR:
    return wi_meteor_48x48;
  case NUCLEAR:
    return ionizing_radiation_symbol_48x48;
  case BIOHAZARD:
    return biological_hazard_symbol_48x48;
  case EARTHQUAKE:
    return wi_earthquake_48x48;
  case TSUNAMI:
    return wi_tsunami_48x48;
  case FIRE:
    return wi_fire_48x48;
  case HEAT:
    return wi_fire_48x48;
  case WINTER:
    return wi_snowflake_cold_48x48;
  case LIGHTNING:
    return wi_lightning_48x48;
  case SANDSTORM:
    return wi_sandstorm_48x48;
  case FLOOD:
    return wi_flood_48x48;
  case VOLCANO:
    return wi_volcano_48x48;
  case AIR_QUALITY:
    return wi_dust_48x48;
  case TORNADO:
    return wi_tornado_48x48;
  case SMALL_CRAFT_ADVISORY:
    return wi_small_craft_advisory_48x48;
  case GALE_WARNING:
    return wi_gale_warning_48x48;
  case STORM_WARNING:
    return wi_storm_warning_48x48;
  case HURRICANE_WARNING:
    return wi_hurricane_warning_48x48;
  case HURRICANE:
    return wi_hurricane_48x48;
  case DUST:
    return wi_dust_48x48;
  case STRONG_WIND:
    return wi_strong_wind_48x48;
  }
  return NULL; // this code return never be reached 
}

/* Returns true of a String, s, contains any of the strings in the terminology 
 * vector.
 *
 * Note: This function is case sensitive.
 */
bool containsTerminology(const String s, const std::vector<String> &terminology)
{
  for (const String &term : terminology)
  {
    if (s.indexOf(term) > 0)
    {
      return true;
    }
  }
  return false;
}

/* Returns the category of an alert based on the terminology found in the event 
 * name.
 *
 * Weather alert terminology is defined in the included language header.
 */
enum alert_category getAlertCategory(owm_alerts_t &alert)
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
  if (containsTerminology(alert.event, TERM_TSUNAMI))
  {
    return alert_category::TSUNAMI;
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
}

#include "icons/icons_24x24.h"
/* Returns a 24x24 wind direction icon bitmap for angles 0 to 359 degrees
 * Parameter is meteorological wind direction, arrow points in the direction the
 * wind is going.
 */
const uint8_t *getWindBitmap24(int wind_deg)
{
  wind_deg %= 360;
  if (wind_deg == 0) {return wind_direction_meteorological_0deg_24x24;}
  if (wind_deg == 1) {return wind_direction_meteorological_1deg_24x24;}
  if (wind_deg == 2) {return wind_direction_meteorological_2deg_24x24;}
  if (wind_deg == 3) {return wind_direction_meteorological_3deg_24x24;}
  if (wind_deg == 4) {return wind_direction_meteorological_4deg_24x24;}
  if (wind_deg == 5) {return wind_direction_meteorological_5deg_24x24;}
  if (wind_deg == 6) {return wind_direction_meteorological_6deg_24x24;}
  if (wind_deg == 7) {return wind_direction_meteorological_7deg_24x24;}
  if (wind_deg == 8) {return wind_direction_meteorological_8deg_24x24;}
  if (wind_deg == 9) {return wind_direction_meteorological_9deg_24x24;}
  if (wind_deg == 10) {return wind_direction_meteorological_10deg_24x24;}
  if (wind_deg == 11) {return wind_direction_meteorological_11deg_24x24;}
  if (wind_deg == 12) {return wind_direction_meteorological_12deg_24x24;}
  if (wind_deg == 13) {return wind_direction_meteorological_13deg_24x24;}
  if (wind_deg == 14) {return wind_direction_meteorological_14deg_24x24;}
  if (wind_deg == 15) {return wind_direction_meteorological_15deg_24x24;}
  if (wind_deg == 16) {return wind_direction_meteorological_16deg_24x24;}
  if (wind_deg == 17) {return wind_direction_meteorological_17deg_24x24;}
  if (wind_deg == 18) {return wind_direction_meteorological_18deg_24x24;}
  if (wind_deg == 19) {return wind_direction_meteorological_19deg_24x24;}
  if (wind_deg == 20) {return wind_direction_meteorological_20deg_24x24;}
  if (wind_deg == 21) {return wind_direction_meteorological_21deg_24x24;}
  if (wind_deg == 22) {return wind_direction_meteorological_22deg_24x24;}
  if (wind_deg == 23) {return wind_direction_meteorological_23deg_24x24;}
  if (wind_deg == 24) {return wind_direction_meteorological_24deg_24x24;}
  if (wind_deg == 25) {return wind_direction_meteorological_25deg_24x24;}
  if (wind_deg == 26) {return wind_direction_meteorological_26deg_24x24;}
  if (wind_deg == 27) {return wind_direction_meteorological_27deg_24x24;}
  if (wind_deg == 28) {return wind_direction_meteorological_28deg_24x24;}
  if (wind_deg == 29) {return wind_direction_meteorological_29deg_24x24;}
  if (wind_deg == 30) {return wind_direction_meteorological_30deg_24x24;}
  if (wind_deg == 31) {return wind_direction_meteorological_31deg_24x24;}
  if (wind_deg == 32) {return wind_direction_meteorological_32deg_24x24;}
  if (wind_deg == 33) {return wind_direction_meteorological_33deg_24x24;}
  if (wind_deg == 34) {return wind_direction_meteorological_34deg_24x24;}
  if (wind_deg == 35) {return wind_direction_meteorological_35deg_24x24;}
  if (wind_deg == 36) {return wind_direction_meteorological_36deg_24x24;}
  if (wind_deg == 37) {return wind_direction_meteorological_37deg_24x24;}
  if (wind_deg == 38) {return wind_direction_meteorological_38deg_24x24;}
  if (wind_deg == 39) {return wind_direction_meteorological_39deg_24x24;}
  if (wind_deg == 40) {return wind_direction_meteorological_40deg_24x24;}
  if (wind_deg == 41) {return wind_direction_meteorological_41deg_24x24;}
  if (wind_deg == 42) {return wind_direction_meteorological_42deg_24x24;}
  if (wind_deg == 43) {return wind_direction_meteorological_43deg_24x24;}
  if (wind_deg == 44) {return wind_direction_meteorological_44deg_24x24;}
  if (wind_deg == 45) {return wind_direction_meteorological_45deg_24x24;}
  if (wind_deg == 46) {return wind_direction_meteorological_46deg_24x24;}
  if (wind_deg == 47) {return wind_direction_meteorological_47deg_24x24;}
  if (wind_deg == 48) {return wind_direction_meteorological_48deg_24x24;}
  if (wind_deg == 49) {return wind_direction_meteorological_49deg_24x24;}
  if (wind_deg == 50) {return wind_direction_meteorological_50deg_24x24;}
  if (wind_deg == 51) {return wind_direction_meteorological_51deg_24x24;}
  if (wind_deg == 52) {return wind_direction_meteorological_52deg_24x24;}
  if (wind_deg == 53) {return wind_direction_meteorological_53deg_24x24;}
  if (wind_deg == 54) {return wind_direction_meteorological_54deg_24x24;}
  if (wind_deg == 55) {return wind_direction_meteorological_55deg_24x24;}
  if (wind_deg == 56) {return wind_direction_meteorological_56deg_24x24;}
  if (wind_deg == 57) {return wind_direction_meteorological_57deg_24x24;}
  if (wind_deg == 58) {return wind_direction_meteorological_58deg_24x24;}
  if (wind_deg == 59) {return wind_direction_meteorological_59deg_24x24;}
  if (wind_deg == 60) {return wind_direction_meteorological_60deg_24x24;}
  if (wind_deg == 61) {return wind_direction_meteorological_61deg_24x24;}
  if (wind_deg == 62) {return wind_direction_meteorological_62deg_24x24;}
  if (wind_deg == 63) {return wind_direction_meteorological_63deg_24x24;}
  if (wind_deg == 64) {return wind_direction_meteorological_64deg_24x24;}
  if (wind_deg == 65) {return wind_direction_meteorological_65deg_24x24;}
  if (wind_deg == 66) {return wind_direction_meteorological_66deg_24x24;}
  if (wind_deg == 67) {return wind_direction_meteorological_67deg_24x24;}
  if (wind_deg == 68) {return wind_direction_meteorological_68deg_24x24;}
  if (wind_deg == 69) {return wind_direction_meteorological_69deg_24x24;}
  if (wind_deg == 70) {return wind_direction_meteorological_70deg_24x24;}
  if (wind_deg == 71) {return wind_direction_meteorological_71deg_24x24;}
  if (wind_deg == 72) {return wind_direction_meteorological_72deg_24x24;}
  if (wind_deg == 73) {return wind_direction_meteorological_73deg_24x24;}
  if (wind_deg == 74) {return wind_direction_meteorological_74deg_24x24;}
  if (wind_deg == 75) {return wind_direction_meteorological_75deg_24x24;}
  if (wind_deg == 76) {return wind_direction_meteorological_76deg_24x24;}
  if (wind_deg == 77) {return wind_direction_meteorological_77deg_24x24;}
  if (wind_deg == 78) {return wind_direction_meteorological_78deg_24x24;}
  if (wind_deg == 79) {return wind_direction_meteorological_79deg_24x24;}
  if (wind_deg == 80) {return wind_direction_meteorological_80deg_24x24;}
  if (wind_deg == 81) {return wind_direction_meteorological_81deg_24x24;}
  if (wind_deg == 82) {return wind_direction_meteorological_82deg_24x24;}
  if (wind_deg == 83) {return wind_direction_meteorological_83deg_24x24;}
  if (wind_deg == 84) {return wind_direction_meteorological_84deg_24x24;}
  if (wind_deg == 85) {return wind_direction_meteorological_85deg_24x24;}
  if (wind_deg == 86) {return wind_direction_meteorological_86deg_24x24;}
  if (wind_deg == 87) {return wind_direction_meteorological_87deg_24x24;}
  if (wind_deg == 88) {return wind_direction_meteorological_88deg_24x24;}
  if (wind_deg == 89) {return wind_direction_meteorological_89deg_24x24;}
  if (wind_deg == 90) {return wind_direction_meteorological_90deg_24x24;}
  if (wind_deg == 91) {return wind_direction_meteorological_91deg_24x24;}
  if (wind_deg == 92) {return wind_direction_meteorological_92deg_24x24;}
  if (wind_deg == 93) {return wind_direction_meteorological_93deg_24x24;}
  if (wind_deg == 94) {return wind_direction_meteorological_94deg_24x24;}
  if (wind_deg == 95) {return wind_direction_meteorological_95deg_24x24;}
  if (wind_deg == 96) {return wind_direction_meteorological_96deg_24x24;}
  if (wind_deg == 97) {return wind_direction_meteorological_97deg_24x24;}
  if (wind_deg == 98) {return wind_direction_meteorological_98deg_24x24;}
  if (wind_deg == 99) {return wind_direction_meteorological_99deg_24x24;}
  if (wind_deg == 100) {return wind_direction_meteorological_100deg_24x24;}
  if (wind_deg == 101) {return wind_direction_meteorological_101deg_24x24;}
  if (wind_deg == 102) {return wind_direction_meteorological_102deg_24x24;}
  if (wind_deg == 103) {return wind_direction_meteorological_103deg_24x24;}
  if (wind_deg == 104) {return wind_direction_meteorological_104deg_24x24;}
  if (wind_deg == 105) {return wind_direction_meteorological_105deg_24x24;}
  if (wind_deg == 106) {return wind_direction_meteorological_106deg_24x24;}
  if (wind_deg == 107) {return wind_direction_meteorological_107deg_24x24;}
  if (wind_deg == 108) {return wind_direction_meteorological_108deg_24x24;}
  if (wind_deg == 109) {return wind_direction_meteorological_109deg_24x24;}
  if (wind_deg == 110) {return wind_direction_meteorological_110deg_24x24;}
  if (wind_deg == 111) {return wind_direction_meteorological_111deg_24x24;}
  if (wind_deg == 112) {return wind_direction_meteorological_112deg_24x24;}
  if (wind_deg == 113) {return wind_direction_meteorological_113deg_24x24;}
  if (wind_deg == 114) {return wind_direction_meteorological_114deg_24x24;}
  if (wind_deg == 115) {return wind_direction_meteorological_115deg_24x24;}
  if (wind_deg == 116) {return wind_direction_meteorological_116deg_24x24;}
  if (wind_deg == 117) {return wind_direction_meteorological_117deg_24x24;}
  if (wind_deg == 118) {return wind_direction_meteorological_118deg_24x24;}
  if (wind_deg == 119) {return wind_direction_meteorological_119deg_24x24;}
  if (wind_deg == 120) {return wind_direction_meteorological_120deg_24x24;}
  if (wind_deg == 121) {return wind_direction_meteorological_121deg_24x24;}
  if (wind_deg == 122) {return wind_direction_meteorological_122deg_24x24;}
  if (wind_deg == 123) {return wind_direction_meteorological_123deg_24x24;}
  if (wind_deg == 124) {return wind_direction_meteorological_124deg_24x24;}
  if (wind_deg == 125) {return wind_direction_meteorological_125deg_24x24;}
  if (wind_deg == 126) {return wind_direction_meteorological_126deg_24x24;}
  if (wind_deg == 127) {return wind_direction_meteorological_127deg_24x24;}
  if (wind_deg == 128) {return wind_direction_meteorological_128deg_24x24;}
  if (wind_deg == 129) {return wind_direction_meteorological_129deg_24x24;}
  if (wind_deg == 130) {return wind_direction_meteorological_130deg_24x24;}
  if (wind_deg == 131) {return wind_direction_meteorological_131deg_24x24;}
  if (wind_deg == 132) {return wind_direction_meteorological_132deg_24x24;}
  if (wind_deg == 133) {return wind_direction_meteorological_133deg_24x24;}
  if (wind_deg == 134) {return wind_direction_meteorological_134deg_24x24;}
  if (wind_deg == 135) {return wind_direction_meteorological_135deg_24x24;}
  if (wind_deg == 136) {return wind_direction_meteorological_136deg_24x24;}
  if (wind_deg == 137) {return wind_direction_meteorological_137deg_24x24;}
  if (wind_deg == 138) {return wind_direction_meteorological_138deg_24x24;}
  if (wind_deg == 139) {return wind_direction_meteorological_139deg_24x24;}
  if (wind_deg == 140) {return wind_direction_meteorological_140deg_24x24;}
  if (wind_deg == 141) {return wind_direction_meteorological_141deg_24x24;}
  if (wind_deg == 142) {return wind_direction_meteorological_142deg_24x24;}
  if (wind_deg == 143) {return wind_direction_meteorological_143deg_24x24;}
  if (wind_deg == 144) {return wind_direction_meteorological_144deg_24x24;}
  if (wind_deg == 145) {return wind_direction_meteorological_145deg_24x24;}
  if (wind_deg == 146) {return wind_direction_meteorological_146deg_24x24;}
  if (wind_deg == 147) {return wind_direction_meteorological_147deg_24x24;}
  if (wind_deg == 148) {return wind_direction_meteorological_148deg_24x24;}
  if (wind_deg == 149) {return wind_direction_meteorological_149deg_24x24;}
  if (wind_deg == 150) {return wind_direction_meteorological_150deg_24x24;}
  if (wind_deg == 151) {return wind_direction_meteorological_151deg_24x24;}
  if (wind_deg == 152) {return wind_direction_meteorological_152deg_24x24;}
  if (wind_deg == 153) {return wind_direction_meteorological_153deg_24x24;}
  if (wind_deg == 154) {return wind_direction_meteorological_154deg_24x24;}
  if (wind_deg == 155) {return wind_direction_meteorological_155deg_24x24;}
  if (wind_deg == 156) {return wind_direction_meteorological_156deg_24x24;}
  if (wind_deg == 157) {return wind_direction_meteorological_157deg_24x24;}
  if (wind_deg == 158) {return wind_direction_meteorological_158deg_24x24;}
  if (wind_deg == 159) {return wind_direction_meteorological_159deg_24x24;}
  if (wind_deg == 160) {return wind_direction_meteorological_160deg_24x24;}
  if (wind_deg == 161) {return wind_direction_meteorological_161deg_24x24;}
  if (wind_deg == 162) {return wind_direction_meteorological_162deg_24x24;}
  if (wind_deg == 163) {return wind_direction_meteorological_163deg_24x24;}
  if (wind_deg == 164) {return wind_direction_meteorological_164deg_24x24;}
  if (wind_deg == 165) {return wind_direction_meteorological_165deg_24x24;}
  if (wind_deg == 166) {return wind_direction_meteorological_166deg_24x24;}
  if (wind_deg == 167) {return wind_direction_meteorological_167deg_24x24;}
  if (wind_deg == 168) {return wind_direction_meteorological_168deg_24x24;}
  if (wind_deg == 169) {return wind_direction_meteorological_169deg_24x24;}
  if (wind_deg == 170) {return wind_direction_meteorological_170deg_24x24;}
  if (wind_deg == 171) {return wind_direction_meteorological_171deg_24x24;}
  if (wind_deg == 172) {return wind_direction_meteorological_172deg_24x24;}
  if (wind_deg == 173) {return wind_direction_meteorological_173deg_24x24;}
  if (wind_deg == 174) {return wind_direction_meteorological_174deg_24x24;}
  if (wind_deg == 175) {return wind_direction_meteorological_175deg_24x24;}
  if (wind_deg == 176) {return wind_direction_meteorological_176deg_24x24;}
  if (wind_deg == 177) {return wind_direction_meteorological_177deg_24x24;}
  if (wind_deg == 178) {return wind_direction_meteorological_178deg_24x24;}
  if (wind_deg == 179) {return wind_direction_meteorological_179deg_24x24;}
  if (wind_deg == 180) {return wind_direction_meteorological_180deg_24x24;}
  if (wind_deg == 181) {return wind_direction_meteorological_181deg_24x24;}
  if (wind_deg == 182) {return wind_direction_meteorological_182deg_24x24;}
  if (wind_deg == 183) {return wind_direction_meteorological_183deg_24x24;}
  if (wind_deg == 184) {return wind_direction_meteorological_184deg_24x24;}
  if (wind_deg == 185) {return wind_direction_meteorological_185deg_24x24;}
  if (wind_deg == 186) {return wind_direction_meteorological_186deg_24x24;}
  if (wind_deg == 187) {return wind_direction_meteorological_187deg_24x24;}
  if (wind_deg == 188) {return wind_direction_meteorological_188deg_24x24;}
  if (wind_deg == 189) {return wind_direction_meteorological_189deg_24x24;}
  if (wind_deg == 190) {return wind_direction_meteorological_190deg_24x24;}
  if (wind_deg == 191) {return wind_direction_meteorological_191deg_24x24;}
  if (wind_deg == 192) {return wind_direction_meteorological_192deg_24x24;}
  if (wind_deg == 193) {return wind_direction_meteorological_193deg_24x24;}
  if (wind_deg == 194) {return wind_direction_meteorological_194deg_24x24;}
  if (wind_deg == 195) {return wind_direction_meteorological_195deg_24x24;}
  if (wind_deg == 196) {return wind_direction_meteorological_196deg_24x24;}
  if (wind_deg == 197) {return wind_direction_meteorological_197deg_24x24;}
  if (wind_deg == 198) {return wind_direction_meteorological_198deg_24x24;}
  if (wind_deg == 199) {return wind_direction_meteorological_199deg_24x24;}
  if (wind_deg == 200) {return wind_direction_meteorological_200deg_24x24;}
  if (wind_deg == 201) {return wind_direction_meteorological_201deg_24x24;}
  if (wind_deg == 202) {return wind_direction_meteorological_202deg_24x24;}
  if (wind_deg == 203) {return wind_direction_meteorological_203deg_24x24;}
  if (wind_deg == 204) {return wind_direction_meteorological_204deg_24x24;}
  if (wind_deg == 205) {return wind_direction_meteorological_205deg_24x24;}
  if (wind_deg == 206) {return wind_direction_meteorological_206deg_24x24;}
  if (wind_deg == 207) {return wind_direction_meteorological_207deg_24x24;}
  if (wind_deg == 208) {return wind_direction_meteorological_208deg_24x24;}
  if (wind_deg == 209) {return wind_direction_meteorological_209deg_24x24;}
  if (wind_deg == 210) {return wind_direction_meteorological_210deg_24x24;}
  if (wind_deg == 211) {return wind_direction_meteorological_211deg_24x24;}
  if (wind_deg == 212) {return wind_direction_meteorological_212deg_24x24;}
  if (wind_deg == 213) {return wind_direction_meteorological_213deg_24x24;}
  if (wind_deg == 214) {return wind_direction_meteorological_214deg_24x24;}
  if (wind_deg == 215) {return wind_direction_meteorological_215deg_24x24;}
  if (wind_deg == 216) {return wind_direction_meteorological_216deg_24x24;}
  if (wind_deg == 217) {return wind_direction_meteorological_217deg_24x24;}
  if (wind_deg == 218) {return wind_direction_meteorological_218deg_24x24;}
  if (wind_deg == 219) {return wind_direction_meteorological_219deg_24x24;}
  if (wind_deg == 220) {return wind_direction_meteorological_220deg_24x24;}
  if (wind_deg == 221) {return wind_direction_meteorological_221deg_24x24;}
  if (wind_deg == 222) {return wind_direction_meteorological_222deg_24x24;}
  if (wind_deg == 223) {return wind_direction_meteorological_223deg_24x24;}
  if (wind_deg == 224) {return wind_direction_meteorological_224deg_24x24;}
  if (wind_deg == 225) {return wind_direction_meteorological_225deg_24x24;}
  if (wind_deg == 226) {return wind_direction_meteorological_226deg_24x24;}
  if (wind_deg == 227) {return wind_direction_meteorological_227deg_24x24;}
  if (wind_deg == 228) {return wind_direction_meteorological_228deg_24x24;}
  if (wind_deg == 229) {return wind_direction_meteorological_229deg_24x24;}
  if (wind_deg == 230) {return wind_direction_meteorological_230deg_24x24;}
  if (wind_deg == 231) {return wind_direction_meteorological_231deg_24x24;}
  if (wind_deg == 232) {return wind_direction_meteorological_232deg_24x24;}
  if (wind_deg == 233) {return wind_direction_meteorological_233deg_24x24;}
  if (wind_deg == 234) {return wind_direction_meteorological_234deg_24x24;}
  if (wind_deg == 235) {return wind_direction_meteorological_235deg_24x24;}
  if (wind_deg == 236) {return wind_direction_meteorological_236deg_24x24;}
  if (wind_deg == 237) {return wind_direction_meteorological_237deg_24x24;}
  if (wind_deg == 238) {return wind_direction_meteorological_238deg_24x24;}
  if (wind_deg == 239) {return wind_direction_meteorological_239deg_24x24;}
  if (wind_deg == 240) {return wind_direction_meteorological_240deg_24x24;}
  if (wind_deg == 241) {return wind_direction_meteorological_241deg_24x24;}
  if (wind_deg == 242) {return wind_direction_meteorological_242deg_24x24;}
  if (wind_deg == 243) {return wind_direction_meteorological_243deg_24x24;}
  if (wind_deg == 244) {return wind_direction_meteorological_244deg_24x24;}
  if (wind_deg == 245) {return wind_direction_meteorological_245deg_24x24;}
  if (wind_deg == 246) {return wind_direction_meteorological_246deg_24x24;}
  if (wind_deg == 247) {return wind_direction_meteorological_247deg_24x24;}
  if (wind_deg == 248) {return wind_direction_meteorological_248deg_24x24;}
  if (wind_deg == 249) {return wind_direction_meteorological_249deg_24x24;}
  if (wind_deg == 250) {return wind_direction_meteorological_250deg_24x24;}
  if (wind_deg == 251) {return wind_direction_meteorological_251deg_24x24;}
  if (wind_deg == 252) {return wind_direction_meteorological_252deg_24x24;}
  if (wind_deg == 253) {return wind_direction_meteorological_253deg_24x24;}
  if (wind_deg == 254) {return wind_direction_meteorological_254deg_24x24;}
  if (wind_deg == 255) {return wind_direction_meteorological_255deg_24x24;}
  if (wind_deg == 256) {return wind_direction_meteorological_256deg_24x24;}
  if (wind_deg == 257) {return wind_direction_meteorological_257deg_24x24;}
  if (wind_deg == 258) {return wind_direction_meteorological_258deg_24x24;}
  if (wind_deg == 259) {return wind_direction_meteorological_259deg_24x24;}
  if (wind_deg == 260) {return wind_direction_meteorological_260deg_24x24;}
  if (wind_deg == 261) {return wind_direction_meteorological_261deg_24x24;}
  if (wind_deg == 262) {return wind_direction_meteorological_262deg_24x24;}
  if (wind_deg == 263) {return wind_direction_meteorological_263deg_24x24;}
  if (wind_deg == 264) {return wind_direction_meteorological_264deg_24x24;}
  if (wind_deg == 265) {return wind_direction_meteorological_265deg_24x24;}
  if (wind_deg == 266) {return wind_direction_meteorological_266deg_24x24;}
  if (wind_deg == 267) {return wind_direction_meteorological_267deg_24x24;}
  if (wind_deg == 268) {return wind_direction_meteorological_268deg_24x24;}
  if (wind_deg == 269) {return wind_direction_meteorological_269deg_24x24;}
  if (wind_deg == 270) {return wind_direction_meteorological_270deg_24x24;}
  if (wind_deg == 271) {return wind_direction_meteorological_271deg_24x24;}
  if (wind_deg == 272) {return wind_direction_meteorological_272deg_24x24;}
  if (wind_deg == 273) {return wind_direction_meteorological_273deg_24x24;}
  if (wind_deg == 274) {return wind_direction_meteorological_274deg_24x24;}
  if (wind_deg == 275) {return wind_direction_meteorological_275deg_24x24;}
  if (wind_deg == 276) {return wind_direction_meteorological_276deg_24x24;}
  if (wind_deg == 277) {return wind_direction_meteorological_277deg_24x24;}
  if (wind_deg == 278) {return wind_direction_meteorological_278deg_24x24;}
  if (wind_deg == 279) {return wind_direction_meteorological_279deg_24x24;}
  if (wind_deg == 280) {return wind_direction_meteorological_280deg_24x24;}
  if (wind_deg == 281) {return wind_direction_meteorological_281deg_24x24;}
  if (wind_deg == 282) {return wind_direction_meteorological_282deg_24x24;}
  if (wind_deg == 283) {return wind_direction_meteorological_283deg_24x24;}
  if (wind_deg == 284) {return wind_direction_meteorological_284deg_24x24;}
  if (wind_deg == 285) {return wind_direction_meteorological_285deg_24x24;}
  if (wind_deg == 286) {return wind_direction_meteorological_286deg_24x24;}
  if (wind_deg == 287) {return wind_direction_meteorological_287deg_24x24;}
  if (wind_deg == 288) {return wind_direction_meteorological_288deg_24x24;}
  if (wind_deg == 289) {return wind_direction_meteorological_289deg_24x24;}
  if (wind_deg == 290) {return wind_direction_meteorological_290deg_24x24;}
  if (wind_deg == 291) {return wind_direction_meteorological_291deg_24x24;}
  if (wind_deg == 292) {return wind_direction_meteorological_292deg_24x24;}
  if (wind_deg == 293) {return wind_direction_meteorological_293deg_24x24;}
  if (wind_deg == 294) {return wind_direction_meteorological_294deg_24x24;}
  if (wind_deg == 295) {return wind_direction_meteorological_295deg_24x24;}
  if (wind_deg == 296) {return wind_direction_meteorological_296deg_24x24;}
  if (wind_deg == 297) {return wind_direction_meteorological_297deg_24x24;}
  if (wind_deg == 298) {return wind_direction_meteorological_298deg_24x24;}
  if (wind_deg == 299) {return wind_direction_meteorological_299deg_24x24;}
  if (wind_deg == 300) {return wind_direction_meteorological_300deg_24x24;}
  if (wind_deg == 301) {return wind_direction_meteorological_301deg_24x24;}
  if (wind_deg == 302) {return wind_direction_meteorological_302deg_24x24;}
  if (wind_deg == 303) {return wind_direction_meteorological_303deg_24x24;}
  if (wind_deg == 304) {return wind_direction_meteorological_304deg_24x24;}
  if (wind_deg == 305) {return wind_direction_meteorological_305deg_24x24;}
  if (wind_deg == 306) {return wind_direction_meteorological_306deg_24x24;}
  if (wind_deg == 307) {return wind_direction_meteorological_307deg_24x24;}
  if (wind_deg == 308) {return wind_direction_meteorological_308deg_24x24;}
  if (wind_deg == 309) {return wind_direction_meteorological_309deg_24x24;}
  if (wind_deg == 310) {return wind_direction_meteorological_310deg_24x24;}
  if (wind_deg == 311) {return wind_direction_meteorological_311deg_24x24;}
  if (wind_deg == 312) {return wind_direction_meteorological_312deg_24x24;}
  if (wind_deg == 313) {return wind_direction_meteorological_313deg_24x24;}
  if (wind_deg == 314) {return wind_direction_meteorological_314deg_24x24;}
  if (wind_deg == 315) {return wind_direction_meteorological_315deg_24x24;}
  if (wind_deg == 316) {return wind_direction_meteorological_316deg_24x24;}
  if (wind_deg == 317) {return wind_direction_meteorological_317deg_24x24;}
  if (wind_deg == 318) {return wind_direction_meteorological_318deg_24x24;}
  if (wind_deg == 319) {return wind_direction_meteorological_319deg_24x24;}
  if (wind_deg == 320) {return wind_direction_meteorological_320deg_24x24;}
  if (wind_deg == 321) {return wind_direction_meteorological_321deg_24x24;}
  if (wind_deg == 322) {return wind_direction_meteorological_322deg_24x24;}
  if (wind_deg == 323) {return wind_direction_meteorological_323deg_24x24;}
  if (wind_deg == 324) {return wind_direction_meteorological_324deg_24x24;}
  if (wind_deg == 325) {return wind_direction_meteorological_325deg_24x24;}
  if (wind_deg == 326) {return wind_direction_meteorological_326deg_24x24;}
  if (wind_deg == 327) {return wind_direction_meteorological_327deg_24x24;}
  if (wind_deg == 328) {return wind_direction_meteorological_328deg_24x24;}
  if (wind_deg == 329) {return wind_direction_meteorological_329deg_24x24;}
  if (wind_deg == 330) {return wind_direction_meteorological_330deg_24x24;}
  if (wind_deg == 331) {return wind_direction_meteorological_331deg_24x24;}
  if (wind_deg == 332) {return wind_direction_meteorological_332deg_24x24;}
  if (wind_deg == 333) {return wind_direction_meteorological_333deg_24x24;}
  if (wind_deg == 334) {return wind_direction_meteorological_334deg_24x24;}
  if (wind_deg == 335) {return wind_direction_meteorological_335deg_24x24;}
  if (wind_deg == 336) {return wind_direction_meteorological_336deg_24x24;}
  if (wind_deg == 337) {return wind_direction_meteorological_337deg_24x24;}
  if (wind_deg == 338) {return wind_direction_meteorological_338deg_24x24;}
  if (wind_deg == 339) {return wind_direction_meteorological_339deg_24x24;}
  if (wind_deg == 340) {return wind_direction_meteorological_340deg_24x24;}
  if (wind_deg == 341) {return wind_direction_meteorological_341deg_24x24;}
  if (wind_deg == 342) {return wind_direction_meteorological_342deg_24x24;}
  if (wind_deg == 343) {return wind_direction_meteorological_343deg_24x24;}
  if (wind_deg == 344) {return wind_direction_meteorological_344deg_24x24;}
  if (wind_deg == 345) {return wind_direction_meteorological_345deg_24x24;}
  if (wind_deg == 346) {return wind_direction_meteorological_346deg_24x24;}
  if (wind_deg == 347) {return wind_direction_meteorological_347deg_24x24;}
  if (wind_deg == 348) {return wind_direction_meteorological_348deg_24x24;}
  if (wind_deg == 349) {return wind_direction_meteorological_349deg_24x24;}
  if (wind_deg == 350) {return wind_direction_meteorological_350deg_24x24;}
  if (wind_deg == 351) {return wind_direction_meteorological_351deg_24x24;}
  if (wind_deg == 352) {return wind_direction_meteorological_352deg_24x24;}
  if (wind_deg == 353) {return wind_direction_meteorological_353deg_24x24;}
  if (wind_deg == 354) {return wind_direction_meteorological_354deg_24x24;}
  if (wind_deg == 355) {return wind_direction_meteorological_355deg_24x24;}
  if (wind_deg == 356) {return wind_direction_meteorological_356deg_24x24;}
  if (wind_deg == 357) {return wind_direction_meteorological_357deg_24x24;}
  if (wind_deg == 358) {return wind_direction_meteorological_358deg_24x24;}
  if (wind_deg == 359) {return wind_direction_meteorological_359deg_24x24;}
  return NULL;
}