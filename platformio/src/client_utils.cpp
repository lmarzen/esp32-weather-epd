// built-in C++ libraries
#include <cstring>
#include <vector>

// arduino/esp32 libraries
#include <Arduino.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <time.h>
#include <WiFi.h>

// additional libraries
#include <Adafruit_BusIO_Register.h>
#include <ArduinoJson.h>
#include <GxEPD2_BW.h>

// header files
#include "api_response.h"
#include "aqi.h"
#include "client_utils.h"
#include "config.h"
#include "display_utils.h"
#include "renderer.h"

/* Power-on and connect wifi.
 * Takes int parameter to store wifi RSSI, or “Received Signal Strength 
 * Indicator"
 *
 * Returns wifi status.
 */
wl_status_t startWiFi(int &wifiRSSI)
{
  WiFi.mode(WIFI_STA);
  Serial.printf("Connecting to '%s'", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long timeout = millis() + 10000; // timeout if wifi does not connect in 10s from now
  wl_status_t connection_status = WiFi.status();

  while ((connection_status != WL_CONNECTED) && (millis() < timeout))
  {
    Serial.print(".");
    delay(50);
    connection_status = WiFi.status();
  }
  Serial.println();

  if (connection_status == WL_CONNECTED)
  {
    wifiRSSI = WiFi.RSSI(); // get Wifi signal strength now, because the WiFi will be turned off to save power!
    Serial.println("IP: " + WiFi.localIP().toString());
  }
  else
  {
    Serial.printf("Could not connect to '%s'\n", WIFI_SSID);
  }
  return connection_status;
}

/* Disconnect and power-off wifi.
 */
void killWiFi()
{
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

/* Prints the local time to serial monitor.
 *
 * Returns true if getting local time was a success, otherwise false.
 */
bool printLocalTime(tm *timeInfo)
{
  if (!getLocalTime(timeInfo))
  {
    Serial.println("Failed to obtain time");
    return false;
  }
  Serial.println(timeInfo, "%A, %B %d %Y %H:%M:%S");
  return true;
}

/* Connects to NTP server and stores time in a tm struct, adjusted for the time
 * zone specified in config.cpp.
 * 
 * Returns true if success, otherwise false.
 * 
 * Note: Must be connected to wifi to get time from NTP server.
 */
bool setupTime(tm *timeInfo)
{
  configTime(0, 0, NTP_SERVER_1, NTP_SERVER_2); // We will pass 0 for gmtOffset_sec and daylightOffset_sec and use setenv() for timezone offsets
  setenv("TZ", TIMEZONE, 1);
  tzset();
  return printLocalTime(timeInfo);
}

/* Perform an HTTP GET request to OpenWeatherMap's "One Call" API
 * If data is recieved, it will be parsed and stored in the global variable
 * owm_onecall.
 *
 * Returns true if OK response is recieved and response is successfully parsed,
 * otherwise false.
 */
bool getOWMonecall(WiFiClient &client, owm_resp_onecall_t &r)
{
  int attempts = 0;
  bool rxSuccess = false;
  String unitsStr = (UNITS == 'i') ? "imperial" : "metric";
  String uri = "/data/2.5/onecall?lat=" + LAT + "&lon=" + LON 
               + "&units=" + unitsStr + "&lang=" + LANG 
               + "&exclude=minutely&appid=" + OWM_APIKEY;

  while (!rxSuccess && attempts < 2)
  {
    HTTPClient http;
    http.begin(client, OWM_ENDPOINT, 80, uri);
    int httpResponse = http.GET();
    if (httpResponse == HTTP_CODE_OK)
    {
      rxSuccess = deserializeOneCall(http.getStream(), r);
    }
    else
    {
      Serial.println("OpenWeatherMap One Call API connection error: " 
        + String(httpResponse, DEC) + " " + http.errorToString(httpResponse));
    }
    client.stop();
    http.end();
    ++attempts;
  }

  return rxSuccess;
}

/* Perform an HTTP GET request to OpenWeatherMap's "Air Pollution" API
 * If data is recieved, it will be parsed and stored in the global variable
 * owm_air_pollution.
 *
 * Returns true if OK response is recieved and response is successfully parsed,
 * otherwise false.
 */
bool getOWMairpollution(WiFiClient &client, owm_resp_air_pollution_t &r)
{
  int attempts = 0;
  bool rxSuccess = false;
  String unitsStr = (UNITS == 'i') ? "imperial" : "metric";

  // set start and end to approriate values so that the last 24 hours of air
  // pollution history is returned. Unix, UTC. Us
  time_t now;
  int64_t end = time(&now);
  int64_t start = end - (3600 * 23);
  char endStr[22];
  char startStr[22];
  sprintf(endStr, "%lld", end);
  sprintf(startStr, "%lld", start);

  String uri = "/data/2.5/air_pollution/history?lat=" + LAT + "&lon=" + LON 
               + "&start=" + startStr + "&end=" + endStr 
               + "&appid=" + OWM_APIKEY;

  while (!rxSuccess && attempts < 2)
  {
    HTTPClient http;
    http.begin(client, OWM_ENDPOINT, 80, uri);
    int httpResponse = http.GET();
    if (httpResponse == HTTP_CODE_OK)
    {
      rxSuccess = deserializeAirQuality(http.getStream(), r);
    }
    else
    {
      Serial.println("OpenWeatherMap Air Pollution API connection error: " 
        + String(httpResponse, DEC) + " " + http.errorToString(httpResponse));
    }
    client.stop();
    http.end();
    ++attempts;
  }

  return rxSuccess;
}

/*
// TODO
bool updateTimeDateStrings()
{
  char time_output[30], day_output[30], update_time[30];
  // see http://www.cplusplus.com/reference/ctime/strftime/
  if (UNITS == 'm')
  {
    if ((LANG == "cz") 
     || (LANG == "de") 
     || (LANG == "pl") 
     || (LANG == "nl"))
    {
      sprintf(day_output, "%s, %02u. %s %04u", TXT_dddd[timeInfo.tm_wday], timeInfo.tm_mday, TXT_MMMM[timeInfo.tm_mon], (timeInfo.tm_year) + 1900); // day_output >> So., 23. Juni 2019 <<
    }
    else
    {
      sprintf(day_output, "%s %02u-%s-%04u", TXT_dddd[timeInfo.tm_wday], timeInfo.tm_mday, TXT_MMMM[timeInfo.tm_mon], (timeInfo.tm_year) + 1900);
    }
    strftime(update_time, sizeof(update_time), "%H:%M:%S", &timeInfo); // Creates: '14:05:49'
    sprintf(time_output, "%s %s", TXT_UPDATED, update_time);
  }
  else
  {
    strftime(day_output, sizeof(day_output), "%a %b-%d-%Y", &timeInfo); // Creates  'Sat May-31-2019'
    strftime(update_time, sizeof(update_time), "%r", &timeInfo);        // Creates: '02:05:49pm'
    sprintf(time_output, "%s %s", TXT_UPDATED, update_time);
  }
  dateStr = day_output;
  timeStr = time_output;
  return true;
}
*/