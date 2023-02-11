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

// language
#include LANGUAGE_HEADER

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
} // startWiFi

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
  int attempts = 0;
  while (!getLocalTime(timeInfo) && attempts++ < 3)
  {
    Serial.println("Failed to obtain time");
    return false;
  }
  Serial.println(timeInfo, "%A, %B %d, %Y %H:%M:%S");
  return true;
} // killWiFi

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
} // setupTime

/* Perform an HTTP GET request to OpenWeatherMap's "One Call" API
 * If data is recieved, it will be parsed and stored in the global variable
 * owm_onecall.
 *
 * Returns the HTTP Status Code.
 */
int getOWMonecall(WiFiClient &client, owm_resp_onecall_t &r)
{
  int attempts = 0;
  bool rxSuccess = false;
  DeserializationError jsonErr = {};
#ifdef UNITS_METRIC
  const char unitsStr[] = "metric";
#endif // end UNITS_METRIC
#ifdef UNITS_IMPERIAL
  const char unitsStr[] = "imperial";
#endif // end UNITS_IMPERIAL

  String uri = "/data/" + OWM_ONECALL_VERSION
               + "/onecall?lat=" + LAT + "&lon=" + LON 
               + "&units=" + unitsStr + "&lang=" + LANG 
               + "&exclude=minutely&appid=" + OWM_APIKEY;

  int httpResponse = 0;
  while (!rxSuccess && attempts < 3)
  {
    HTTPClient http;
    http.begin(client, OWM_ENDPOINT, 80, uri);
    httpResponse = http.GET();
    if (httpResponse == HTTP_CODE_OK)
    {
      jsonErr = deserializeOneCall(http.getStream(), r);
      rxSuccess = !jsonErr;
    }
    else
    {
      Serial.println("OpenWeatherMap One Call " + OWM_ONECALL_VERSION 
        + " API error: " + String(httpResponse, DEC) + " " 
        + getHttpResponsePhrase(httpResponse));
    }
    client.stop();
    http.end();
    ++attempts;
  }

  if (httpResponse == HTTP_CODE_OK && jsonErr)
  { // indicates client json DeserializationError
    // given a -100 offset to distiguish these errors from http client errors
    return -100 - static_cast<int>(jsonErr.code());
  }
  return httpResponse;
} // getOWMonecall

/* Perform an HTTP GET request to OpenWeatherMap's "Air Pollution" API
 * If data is recieved, it will be parsed and stored in the global variable
 * owm_air_pollution.
 *
 * Returns the HTTP Status Code.
 */
int getOWMairpollution(WiFiClient &client, owm_resp_air_pollution_t &r)
{
  int attempts = 0;
  bool rxSuccess = false;
  DeserializationError jsonErr = {};

  // set start and end to approriate values so that the last 24 hours of air
  // pollution history is returned. Unix, UTC.
  time_t now;
  int64_t end = time(&now);
  // minus 1 is important here, otherwise we could get an extra hour of history
  int64_t start = end - ((3600 * OWM_NUM_AIR_POLLUTION) - 1);
  char endStr[22];
  char startStr[22];
  sprintf(endStr, "%lld", end);
  sprintf(startStr, "%lld", start);

  String uri = "/data/2.5/air_pollution/history?lat=" + LAT + "&lon=" + LON 
               + "&start=" + startStr + "&end=" + endStr 
               + "&appid=" + OWM_APIKEY;

  int httpResponse = 0;
  while (!rxSuccess && attempts < 3)
  {
    HTTPClient http;
    http.begin(client, OWM_ENDPOINT, 80, uri);
    httpResponse = http.GET();
    if (httpResponse == HTTP_CODE_OK)
    {
      jsonErr = deserializeAirQuality(http.getStream(), r);
      rxSuccess = !jsonErr;
    }
    else
    {
      Serial.println("OpenWeatherMap Air Pollution API error: " 
        + String(httpResponse, DEC) + " " 
        + getHttpResponsePhrase(httpResponse));
    }
    client.stop();
    http.end();
    ++attempts;
  }

  if (httpResponse == HTTP_CODE_OK && jsonErr)
  { // indicates client json DeserializationError
    // given a -100 offset to distiguish these errors from http client errors
    return -100 - static_cast<int>(jsonErr.code());
  }
  return httpResponse;
} // getOWMairpollution
