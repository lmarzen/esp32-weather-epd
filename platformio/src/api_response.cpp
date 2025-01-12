/* API response deserialization for esp32-weather-epd.
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

#include <vector>
#include <ArduinoJson.h>
#include "api_response.h"
#include "config.h"



DeserializationError deserializeOneCall(const char* jsonString,
                                        owm_resp_onecall_t &r)
{
  StaticJsonDocument<832> filter;
  filter["current"]  = true;
  filter["minutely"] = false;
  filter["hourly"]   = true;
  filter["daily"]    = true;
#if !DISPLAY_ALERTS
  filter["alerts"]   = false;
#else
  JsonArray filter_alerts = filter.createNestedArray("alerts");

  // description can be very long so they are filtered out to save on memory
  // along with sender_name
  JsonObject filter_alerts_0 = filter_alerts.createNestedObject();
  filter_alerts_0["sender_name"] = false;
  filter_alerts_0["event"]       = true;
  filter_alerts_0["start"]       = true;
  filter_alerts_0["end"]         = true;
  filter_alerts_0["description"] = false;
  filter_alerts_0["tags"]        = true;
  JsonObject filter_alerts_1 = filter_alerts.createNestedObject();
  filter_alerts_1["sender_name"] = false;
  filter_alerts_1["event"]       = true;
  filter_alerts_1["start"]       = true;
  filter_alerts_1["end"]         = true;
  filter_alerts_1["description"] = false;
  filter_alerts_1["tags"]        = true;
  JsonObject filter_alerts_2 = filter_alerts.createNestedObject();
  filter_alerts_2["sender_name"] = false;
  filter_alerts_2["event"]       = true;
  filter_alerts_2["start"]       = true;
  filter_alerts_2["end"]         = true;
  filter_alerts_2["description"] = false;
  filter_alerts_2["tags"]        = true;
  JsonObject filter_alerts_3 = filter_alerts.createNestedObject();
  filter_alerts_3["sender_name"] = false;
  filter_alerts_3["event"]       = true;
  filter_alerts_3["start"]       = true;
  filter_alerts_3["end"]         = true;
  filter_alerts_3["description"] = false;
  filter_alerts_3["tags"]        = true;
  JsonObject filter_alerts_4 = filter_alerts.createNestedObject();
  filter_alerts_4["sender_name"] = false;
  filter_alerts_4["event"]       = true;
  filter_alerts_4["start"]       = true;
  filter_alerts_4["end"]         = true;
  filter_alerts_4["description"] = false;
  filter_alerts_4["tags"]        = true;
  JsonObject filter_alerts_5 = filter_alerts.createNestedObject();
  filter_alerts_5["sender_name"] = false;
  filter_alerts_5["event"]       = true;
  filter_alerts_5["start"]       = true;
  filter_alerts_5["end"]         = true;
  filter_alerts_5["description"] = false;
  filter_alerts_5["tags"]        = true;
  JsonObject filter_alerts_6 = filter_alerts.createNestedObject();
  filter_alerts_6["sender_name"] = false;
  filter_alerts_6["event"]       = true;
  filter_alerts_6["start"]       = true;
  filter_alerts_6["end"]         = true;
  filter_alerts_6["description"] = false;
  filter_alerts_6["tags"]        = true;
  JsonObject filter_alerts_7 = filter_alerts.createNestedObject();
  filter_alerts_7["sender_name"] = false;
  filter_alerts_7["event"]       = true;
  filter_alerts_7["start"]       = true;
  filter_alerts_7["end"]         = true;
  filter_alerts_7["description"] = false;
  filter_alerts_7["tags"]        = true;
#endif

  DynamicJsonDocument doc(32 * 1024);
  // DynamicJsonDocument doc(55000);

  // print capacity
  Serial.println("[debug] doc.capacity() : "
                 + String(doc.capacity()) + " B"); // 0 on allocation failure

  DeserializationError error = deserializeJson(doc, jsonString);
                                        //  DeserializationOption::Filter(filter));

  // print size of jsonString
  Serial.println("[debug] jsonString size: "
                 + String(strlen(jsonString)) + " B");



  #if DEBUG_LEVEL >= 1
  Serial.println("[debug] doc.memoryUsage() : "
                 + String(doc.memoryUsage()) + " B");
  Serial.println("[debug] doc.capacity() : "
                 + String(doc.capacity()) + " B"); // 0 on allocation failure
  #endif
  if (error) {
    return error;
  }

  r.lat             = doc["latitude"]            .as<float>();
  r.lon             = doc["longitude"]            .as<float>();
  r.timezone        = doc["timezone"]       .as<const char *>();
  r.timezone_offset = doc["utc_offset_seconds"].as<int>();

  JsonObject current = doc["current"];
  JsonObject daily = doc["daily"];
  JsonObject hourly = doc["hourly"];

  r.current.dt         = current["time"]        .as<int64_t>();
  r.current.sunrise    = daily["sunrise"][0]   .as<int64_t>(); //
  r.current.sunset     = daily["sunset"][0]    .as<int64_t>(); //
  r.current.temp       = current["temperature_2m"]      .as<float>();
  r.current.feels_like = current["apparent_temperature"].as<float>();
  r.current.pressure   = current["surface_pressure"]  .as<int>(); //
  r.current.humidity   = current["relative_humidity_2m"]  .as<int>();
  // r.current.dew_point  = current["dew_point"] .as<float>(); //
  r.current.clouds     = current["cloud_cover"]    .as<int>();
  r.current.uvi        = daily["uv_index_max"][0]       .as<float>(); //
  r.current.visibility = hourly["visibility"][0].as<int>(); //
  r.current.wind_speed = current["wind_speed_10m"].as<float>();
  r.current.wind_gust  = current["wind_gusts_10m"] .as<float>();
  r.current.wind_deg   = current["wind_direction_10m"]  .as<int>(); // w
  r.current.rain_1h    = current["rain"].as<float>();
  r.current.snow_1h    = current["snow"].as<float>();
  // JsonObject current_weather = current["weather"][0];
  r.current.weather.id          = current["weather_code"]         .as<int>();
  // r.current.weather.main        = current_weather["main"]       .as<const char *>();
  // r.current.weather.description = current_weather["description"].as<const char *>();
  // r.current.weather.icon        = current_weather["icon"]       .as<const char *>();

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

  int hours = doc["hourly"]["time"].size();
  for (size_t i = 0; i < hours; i++)
  {
    r.hourly[i].dt         = hourly["time"][i]        .as<int64_t>(); // dt means 
    r.hourly[i].temp       = hourly["temperature_2m"][i]      .as<float>();
    r.hourly[i].feels_like = hourly["apparent_temperature"][i].as<float>();
    r.hourly[i].pressure   = hourly["surface_pressure"][i]  .as<int>();
    r.hourly[i].humidity   = hourly["relative_humidity_2m"][i]  .as<int>();
    r.hourly[i].dew_point  = hourly["dew_point_2m"][i] .as<float>();
    r.hourly[i].clouds     = hourly["cloud_cover"][i]    .as<int>();
    // r.hourly[i].uvi        = hourly["uvi"][i]       .as<float>();
    r.hourly[i].visibility = hourly["visibility"][i].as<int>();
    r.hourly[i].wind_speed = hourly["wind_speed_10m"][i].as<float>();
    r.hourly[i].wind_gust  = hourly["wind_gust_10m"][i] .as<float>();
    r.hourly[i].wind_deg   = hourly["wind_deg_10m"][i]  .as<int>();
    r.hourly[i].pop        = hourly["precipitation_probability"][i]       .as<float>();
    r.hourly[i].rain_1h    = hourly["rain"][i].as<float>();
    r.hourly[i].snow_1h    = hourly["snowfall"][i].as<float>();
    // JsonObject hourly_weather = hourly["weather"][0];
    // r.hourly[i].weather.id          = hourly_weather["id"]         .as<int>();
    // r.hourly[i].weather.main        = hourly_weather["main"]       .as<const char *>();
    // r.hourly[i].weather.description = hourly_weather["description"].as<const char *>();
    // r.hourly[i].weather.icon        = hourly_weather["icon"]       .as<const char *>();

    if (i == OWM_NUM_HOURLY - 1)
    {
      break;
    }
  }

  int days = doc["daily"]["time"].size();
  for (size_t i = 0; i < days; i++)
  {
    r.daily[i].dt         = daily["time"][i]        .as<int64_t>();
    r.daily[i].sunrise    = daily["sunrise"][i]   .as<int64_t>();
    r.daily[i].sunset     = daily["sunset"][i]    .as<int64_t>();
    // r.daily[i].moonrise   = daily["moonrise"]  .as<int64_t>();
    // r.daily[i].moonset    = daily["moonset"]   .as<int64_t>();
    // r.daily[i].moon_phase = daily["moon_phase"].as<float>();
    // JsonObject daily_temp = daily["temp"];
    // r.daily[i].temp.morn  = daily_temp["morn"] .as<float>();
    // r.daily[i].temp.day   = daily_temp["day"]  .as<float>();
    // r.daily[i].temp.eve   = daily_temp["eve"]  .as<float>();
    // r.daily[i].temp.night = daily_temp["night"].as<float>();
    r.daily[i].temp.min   = daily["temperature_2m_min"][i]  .as<float>();
    r.daily[i].temp.max   = daily["temperature_2m_max"][i]  .as<float>();
    Serial.println("daily temp min: " + String(r.daily[i].temp.min));
    Serial.println("daily temp max: " + String(r.daily[i].temp.max));
    // JsonObject daily_feels_like = daily["feels_like"];
    // r.daily[i].feels_like.morn  = daily_feels_like["morn"] .as<float>();
    // r.daily[i].feels_like.day   = daily_feels_like["day"]  .as<float>();
    // r.daily[i].feels_like.eve   = daily_feels_like["eve"]  .as<float>();
    // r.daily[i].feels_like.night = daily_feels_like["night"].as<float>();
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
    // JsonObject daily_weather = daily["weather"][0];
    r.daily[i].weather.id          = daily["weather_code"][i]         .as<int>();
    Serial.println("daily weather id: " + String(r.daily[i].weather.id));
    // r.daily[i].weather.main        = daily_weather["main"]       .as<const char *>();
    // r.daily[i].weather.description = daily_weather["description"].as<const char *>();
    // r.daily[i].weather.icon        = daily_weather["icon"]       .as<const char *>();

    if (i == OWM_NUM_DAILY - 1)
    {
      break;
    }
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

DeserializationError deserializeAirQuality(WiFiClient &stream,
                                           owm_resp_air_pollution_t &r)
{
  int i = 0;

  DynamicJsonDocument doc(6 * 1024);

  DeserializationError error = deserializeJson(doc, stream);
#if DEBUG_LEVEL >= 1
  Serial.println("[debug] doc.memoryUsage() : "
                 + String(doc.memoryUsage()) + " B");
  Serial.println("[debug] doc.capacity() : "
                 + String(doc.capacity()) + " B"); // 0 on allocation failure
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

