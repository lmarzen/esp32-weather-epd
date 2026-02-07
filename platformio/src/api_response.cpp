/* API response deserialization for esp32-weather-epd.
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

#include <cstring>
#include <vector>
#include <ArduinoJson.h>
#include "api_response.h"
#include "config.h"
#include "user_config.h"

DeserializationError deserializeOneCall(WiFiClient &json,
                                        owm_resp_onecall_t &r)
{
  int i;

  JsonDocument filter;
  filter["current"]  = true;
  filter["minutely"] = false;
  filter["hourly"]   = true;
  filter["daily"]    = true;
#if !DISPLAY_ALERTS
  filter["alerts"]   = false;
#else
  // description can be very long so they are filtered out to save on memory
  // along with sender_name
  for (int i = 0; i < OWM_NUM_ALERTS; ++i)
  {
    filter["alerts"][i]["sender_name"] = false;
    filter["alerts"][i]["event"]       = true;
    filter["alerts"][i]["start"]       = true;
    filter["alerts"][i]["end"]         = true;
    filter["alerts"][i]["description"] = false;
    filter["alerts"][i]["tags"]        = true;
  }
#endif

  JsonDocument doc;

  DeserializationError error = deserializeJson(doc, json,
                                         DeserializationOption::Filter(filter));
#if DEBUG_LEVEL >= 1
  Serial.println("[debug] doc.overflowed() : "
                 + String(doc.overflowed()));
#endif
#if DEBUG_LEVEL >= 2
  serializeJsonPretty(doc, Serial);
#endif
  if (error) {
    return error;
  }

  r.lat             = doc["lat"]            .as<float>();
  r.lon             = doc["lon"]            .as<float>();
  r.timezone        = doc["timezone"]       .as<const char *>();
  r.timezone_offset = doc["timezone_offset"].as<int>();

  JsonObject current = doc["current"];
  r.current.dt         = current["dt"]        .as<int64_t>();
  r.current.sunrise    = current["sunrise"]   .as<int64_t>();
  r.current.sunset     = current["sunset"]    .as<int64_t>();
  r.current.temp       = current["temp"]      .as<float>();
  r.current.feels_like = current["feels_like"].as<float>();
  r.current.pressure   = current["pressure"]  .as<int>();
  r.current.humidity   = current["humidity"]  .as<int>();
  r.current.dew_point  = current["dew_point"] .as<float>();
  r.current.clouds     = current["clouds"]    .as<int>();
  r.current.uvi        = current["uvi"]       .as<float>();
  r.current.visibility = current["visibility"].as<int>();
  r.current.wind_speed = current["wind_speed"].as<float>();
  r.current.wind_gust  = current["wind_gust"] .as<float>();
  r.current.wind_deg   = current["wind_deg"]  .as<int>();
  r.current.rain_1h    = current["rain"]["1h"].as<float>();
  r.current.snow_1h    = current["snow"]["1h"].as<float>();
  JsonObject current_weather = current["weather"][0];
  r.current.weather.id          = current_weather["id"]         .as<int>();
  r.current.weather.main        = current_weather["main"]       .as<const char *>();
  r.current.weather.description = current_weather["description"].as<const char *>();
  r.current.weather.icon        = current_weather["icon"]       .as<const char *>();

  // minutely forecast is currently unused
  // i = 0;
  // for (JsonObject minutely : doc["minutely"].as<JsonArray>())
  // {
  //   r.minutely[i].dt            = minutely["dt"]           .as<int64_t>();
  //   r.minutely[i].precipitation = minutely["precipitation"].as<float>();

  //   if (i == OWM_NUM_MINUTELY - 1)
  //   {
  //     break;
  //   }
  //   ++i;
  // }

  i = 0;
  for (JsonObject hourly : doc["hourly"].as<JsonArray>())
  {
    r.hourly[i].dt         = hourly["dt"]        .as<int64_t>();
    r.hourly[i].temp       = hourly["temp"]      .as<float>();
    r.hourly[i].feels_like = hourly["feels_like"].as<float>();
    r.hourly[i].pressure   = hourly["pressure"]  .as<int>();
    r.hourly[i].humidity   = hourly["humidity"]  .as<int>();
    r.hourly[i].dew_point  = hourly["dew_point"] .as<float>();
    r.hourly[i].clouds     = hourly["clouds"]    .as<int>();
    r.hourly[i].uvi        = hourly["uvi"]       .as<float>();
    r.hourly[i].visibility = hourly["visibility"].as<int>();
    r.hourly[i].wind_speed = hourly["wind_speed"].as<float>();
    r.hourly[i].wind_gust  = hourly["wind_gust"] .as<float>();
    r.hourly[i].wind_deg   = hourly["wind_deg"]  .as<int>();
    r.hourly[i].pop        = hourly["pop"]       .as<float>();
    r.hourly[i].rain_1h    = hourly["rain"]["1h"].as<float>();
    r.hourly[i].snow_1h    = hourly["snow"]["1h"].as<float>();
    JsonObject hourly_weather = hourly["weather"][0];
    r.hourly[i].weather.id          = hourly_weather["id"]         .as<int>();
    r.hourly[i].weather.main        = hourly_weather["main"]       .as<const char *>();
    r.hourly[i].weather.description = hourly_weather["description"].as<const char *>();
    r.hourly[i].weather.icon        = hourly_weather["icon"]       .as<const char *>();

    if (i == OWM_NUM_HOURLY - 1)
    {
      break;
    }
    ++i;
  }

  i = 0;
  for (JsonObject daily : doc["daily"].as<JsonArray>())
  {
    r.daily[i].dt         = daily["dt"]        .as<int64_t>();
    r.daily[i].sunrise    = daily["sunrise"]   .as<int64_t>();
    r.daily[i].sunset     = daily["sunset"]    .as<int64_t>();
    r.daily[i].moonrise   = daily["moonrise"]  .as<int64_t>();
    r.daily[i].moonset    = daily["moonset"]   .as<int64_t>();
    r.daily[i].moon_phase = daily["moon_phase"].as<float>();
    JsonObject daily_temp = daily["temp"];
    r.daily[i].temp.morn  = daily_temp["morn"] .as<float>();
    r.daily[i].temp.day   = daily_temp["day"]  .as<float>();
    r.daily[i].temp.eve   = daily_temp["eve"]  .as<float>();
    r.daily[i].temp.night = daily_temp["night"].as<float>();
    r.daily[i].temp.min   = daily_temp["min"]  .as<float>();
    r.daily[i].temp.max   = daily_temp["max"]  .as<float>();
    JsonObject daily_feels_like = daily["feels_like"];
    r.daily[i].feels_like.morn  = daily_feels_like["morn"] .as<float>();
    r.daily[i].feels_like.day   = daily_feels_like["day"]  .as<float>();
    r.daily[i].feels_like.eve   = daily_feels_like["eve"]  .as<float>();
    r.daily[i].feels_like.night = daily_feels_like["night"].as<float>();
    r.daily[i].pressure   = daily["pressure"]  .as<int>();
    r.daily[i].humidity   = daily["humidity"]  .as<int>();
    r.daily[i].dew_point  = daily["dew_point"] .as<float>();
    r.daily[i].clouds     = daily["clouds"]    .as<int>();
    r.daily[i].uvi        = daily["uvi"]       .as<float>();
    r.daily[i].visibility = daily["visibility"].as<int>();
    r.daily[i].wind_speed = daily["wind_speed"].as<float>();
    r.daily[i].wind_gust  = daily["wind_gust"] .as<float>();
    r.daily[i].wind_deg   = daily["wind_deg"]  .as<int>();
    r.daily[i].pop        = daily["pop"]       .as<float>();
    r.daily[i].rain       = daily["rain"]      .as<float>();
    r.daily[i].snow       = daily["snow"]      .as<float>();
    JsonObject daily_weather = daily["weather"][0];
    r.daily[i].weather.id          = daily_weather["id"]         .as<int>();
    r.daily[i].weather.main        = daily_weather["main"]       .as<const char *>();
    r.daily[i].weather.description = daily_weather["description"].as<const char *>();
    r.daily[i].weather.icon        = daily_weather["icon"]       .as<const char *>();

    if (i == OWM_NUM_DAILY - 1)
    {
      break;
    }
    ++i;
  }

#if DISPLAY_ALERTS
  i = 0;
  for (JsonObject alerts : doc["alerts"].as<JsonArray>())
  {
    owm_alerts_t new_alert = {};
    // new_alert.sender_name = alerts["sender_name"].as<const char *>();
    new_alert.event       = alerts["event"]      .as<const char *>();
    new_alert.start       = alerts["start"]      .as<int64_t>();
    new_alert.end         = alerts["end"]        .as<int64_t>();
    // new_alert.description = alerts["description"].as<const char *>();
    new_alert.tags        = alerts["tags"][0]    .as<const char *>();
    r.alerts.push_back(new_alert);

    if (i == OWM_NUM_ALERTS - 1)
    {
      break;
    }
    ++i;
  }
#endif

  return error;
} // end deserializeOneCall

DeserializationError deserializeAirQuality(WiFiClient &json,
                                           owm_resp_air_pollution_t &r)
{
  int i = 0;

  JsonDocument doc;

  DeserializationError error = deserializeJson(doc, json);
#if DEBUG_LEVEL >= 1
  Serial.println("[debug] doc.overflowed() : "
                 + String(doc.overflowed()));
#endif
#if DEBUG_LEVEL >= 2
  serializeJsonPretty(doc, Serial);
#endif
  if (error) {
    return error;
  }

  r.coord.lat = doc["coord"]["lat"].as<float>();
  r.coord.lon = doc["coord"]["lon"].as<float>();

  for (JsonObject list : doc["list"].as<JsonArray>())
  {

    r.main_aqi[i] = list["main"]["aqi"].as<int>();

    JsonObject list_components = list["components"];
    r.components.co[i]    = list_components["co"].as<float>();
    r.components.no[i]    = list_components["no"].as<float>();
    r.components.no2[i]   = list_components["no2"].as<float>();
    r.components.o3[i]    = list_components["o3"].as<float>();
    r.components.so2[i]   = list_components["so2"].as<float>();
    r.components.pm2_5[i] = list_components["pm2_5"].as<float>();
    r.components.pm10[i]  = list_components["pm10"].as<float>();
    r.components.nh3[i]   = list_components["nh3"].as<float>();

    r.dt[i] = list["dt"].as<int64_t>();

    if (i == OWM_NUM_AIR_POLLUTION - 1)
    {
      break;
    }
    ++i;
  }

  return error;
} // end deserializeAirQuality

/* Deserialize CoinGecko /coins/markets API response.
 * Expects the response for 4 coins with sparkline=true and
 * price_change_percentage=24h,7d,30d,1y.
 *
 * Returns true on success.
 */
bool deserializeCoinGecko(WiFiClient &json, page_data_t &page)
{
  JsonDocument filter;
  // Filter to reduce memory: only extract what we need
  for (int i = 0; i < ASSETS_PER_PAGE; ++i)
  {
    filter[i]["id"]                                = true;
    filter[i]["symbol"]                            = true;
    filter[i]["name"]                              = true;
    filter[i]["current_price"]                     = true;
    filter[i]["price_change_percentage_24h"]        = true;
    filter[i]["price_change_percentage_7d_in_currency"]  = true;
    filter[i]["price_change_percentage_30d_in_currency"] = true;
    filter[i]["price_change_percentage_1y_in_currency"]  = true;
    filter[i]["sparkline_in_7d"]["price"]           = true;
  }

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, json,
                                         DeserializationOption::Filter(filter));
#if DEBUG_LEVEL >= 1
  Serial.println("[debug] CoinGecko doc.overflowed() : "
                 + String(doc.overflowed()));
#endif
  if (error)
  {
    Serial.println("CoinGecko deserialize error: " + String(error.c_str()));
    return false;
  }

  JsonArray arr = doc.as<JsonArray>();
  int idx = 0;
  for (JsonObject coin : arr)
  {
    if (idx >= ASSETS_PER_PAGE) break;

    asset_data_t &a = page.assets[idx];

    const char *sym = coin["symbol"] | "";
    const char *name = coin["name"] | "";
    strncpy(a.symbol, sym, sizeof(a.symbol) - 1);
    a.symbol[sizeof(a.symbol) - 1] = '\0';
    strncpy(a.name, name, sizeof(a.name) - 1);
    a.name[sizeof(a.name) - 1] = '\0';
    // displaySymbol is set by the caller from user_config.h

    a.price         = coin["current_price"]                     | 0.0f;
    a.change_day    = coin["price_change_percentage_24h"]        | 0.0f;
    a.change_week   = coin["price_change_percentage_7d_in_currency"]  | 0.0f;
    a.change_month  = coin["price_change_percentage_30d_in_currency"] | 0.0f;
    a.change_ytd    = coin["price_change_percentage_1y_in_currency"]  | 0.0f;
    a.previousClose = a.price / (1.0f + a.change_day / 100.0f);

    // Extract sparkline data (7-day, ~168 points) — downsample to 30 points
    JsonArray sparkArr = coin["sparkline_in_7d"]["price"];
    int totalPoints = sparkArr.size();
    a.sparklineCount = 0;
    if (totalPoints > 0)
    {
      int step = totalPoints / SPARKLINE_MAX_POINTS;
      if (step < 1) step = 1;
      for (int s = 0; s < totalPoints && a.sparklineCount < SPARKLINE_MAX_POINTS; s += step)
      {
        a.sparkline[a.sparklineCount++] = sparkArr[s].as<float>();
      }
    }
    a.valid = true;
    ++idx;
  }

  page.lastUpdated = time(nullptr);
  page.valid = (idx > 0);
  return page.valid;
} // end deserializeCoinGecko

/* Deserialize Yahoo Finance /v8/finance/chart/ API response for a single symbol.
 * Populates one asset_data_t with current price, previous close, and sparkline.
 *
 * Returns true on success.
 */
bool deserializeYahooFinance(WiFiClient &json, asset_data_t &asset)
{
  JsonDocument filter;
  filter["chart"]["result"][0]["meta"]["regularMarketPrice"] = true;
  filter["chart"]["result"][0]["meta"]["previousClose"]      = true;
  filter["chart"]["result"][0]["meta"]["currency"]           = true;
  filter["chart"]["result"][0]["indicators"]["quote"][0]["close"] = true;

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, json,
                                         DeserializationOption::Filter(filter));
#if DEBUG_LEVEL >= 1
  Serial.println("[debug] YahooFinance doc.overflowed() : "
                 + String(doc.overflowed()));
#endif
  if (error)
  {
    Serial.println("Yahoo Finance deserialize error: " + String(error.c_str()));
    return false;
  }

  JsonObject result = doc["chart"]["result"][0];
  if (result.isNull())
  {
    Serial.println("Yahoo Finance: no result in response");
    return false;
  }

  JsonObject meta = result["meta"];
  asset.price         = meta["regularMarketPrice"] | 0.0f;
  asset.previousClose = meta["previousClose"]      | 0.0f;

  // Calculate day change percentage
  if (asset.previousClose > 0.0f)
  {
    asset.change_day = ((asset.price - asset.previousClose) / asset.previousClose) * 100.0f;
  }
  else
  {
    asset.change_day = 0.0f;
  }

  // Extract close prices for sparkline
  JsonArray closes = result["indicators"]["quote"][0]["close"];
  asset.sparklineCount = 0;
  int totalPoints = closes.size();
  if (totalPoints > 0)
  {
    // For 1mo range with 1d interval, we get ~22 trading days
    // Take up to SPARKLINE_MAX_POINTS
    int step = 1;
    if (totalPoints > SPARKLINE_MAX_POINTS)
    {
      step = totalPoints / SPARKLINE_MAX_POINTS;
    }
    float lastValid = 0.0f;
    for (int i = 0; i < totalPoints && asset.sparklineCount < SPARKLINE_MAX_POINTS; i += step)
    {
      float val = closes[i].as<float>();
      if (val > 0.0f)
      {
        lastValid = val;
      }
      asset.sparkline[asset.sparklineCount++] = (val > 0.0f) ? val : lastValid;
    }

    // Calculate week and month changes from sparkline data
    if (asset.sparklineCount >= 2)
    {
      float oldest = asset.sparkline[0];
      float newest = asset.sparkline[asset.sparklineCount - 1];
      if (oldest > 0.0f)
      {
        asset.change_month = ((newest - oldest) / oldest) * 100.0f;
      }
      // Week change: ~5 trading days from end
      int weekIdx = asset.sparklineCount - 5;
      if (weekIdx < 0) weekIdx = 0;
      float weekPrice = asset.sparkline[weekIdx];
      if (weekPrice > 0.0f)
      {
        asset.change_week = ((newest - weekPrice) / weekPrice) * 100.0f;
      }
    }
  }

  asset.valid = (asset.price > 0.0f);
  return asset.valid;
} // end deserializeYahooFinance

