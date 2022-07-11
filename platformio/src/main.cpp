// arduino/esp32 libraries
#include <Arduino.h>
#include <DHT.h>
#include <SPI.h>
#include <time.h>
#include <WiFi.h>
#include <Wire.h>

// header files
#include "api_response.h"
#include "aqi.h"
#include "client_utils.h"
#include "config.h"
#include "display_utils.h"
#include "renderer.h"

// too large to allocate locally on stack
static owm_resp_onecall_t       owm_onecall;
static owm_resp_air_pollution_t owm_air_pollution;

void beginDeepSleep(unsigned long &startTime, tm *timeInfo)
{
  if (!getLocalTime(timeInfo))
  {
    Serial.print("Failed to obtain time before deep-sleep");
    Serial.println(", referencing older time.");
  }
  unsigned long sleepDuration = (SLEEP_DURATION * 60
                              - ((timeInfo->tm_min % SLEEP_DURATION) * 60 
                              + timeInfo->tm_sec)) + 1;
  esp_sleep_enable_timer_wakeup(sleepDuration * 1000000);
  Serial.println("Awake for " 
                 + String((millis() - startTime) / 1000.0, 3) + "s");
  Serial.println("Deep-sleep for " + String(sleepDuration) + "s");
  esp_deep_sleep_start();
} // end beginDeepSleep

void setup()
{
  unsigned long startTime = millis();
  Serial.begin(115200);

  // DFRobot FireBeetle Esp32-E V1.0 has voltage divider (1M+1M), so readings 
  // must be multiplied by 2. Readings are divided by 1000 to convert mV to V.
  double batteryVoltage = 
            static_cast<double>(analogRead(PIN_BAT_ADC)) / 1000.0 * (3.3 / 2.0);
  Serial.println("Battery voltage: " + String(batteryVoltage,2));
  if (batteryVoltage <= CRIT_LOW_BATTERY_VOLTAGE)
  { // critically low battery, deep-sleep now
    esp_sleep_enable_timer_wakeup(CRIT_LOW_BATTERY_SLEEP_INTERVAL 
                                  * 60 * 1000000);
    Serial.println("Critically low battery voltage!");
    Serial.println("Deep-sleep for " 
                   + String(CRIT_LOW_BATTERY_SLEEP_INTERVAL) + "min");
    esp_deep_sleep_start();
  }
  else if (batteryVoltage <= LOW_BATTERY_VOLTAGE)
  { // low battery, deep-sleep now
    esp_sleep_enable_timer_wakeup(LOW_BATTERY_SLEEP_INTERVAL * 60 * 1000000);
    Serial.println("Low battery voltage!");
    Serial.println("Deep-sleep for " 
                   + String(LOW_BATTERY_SLEEP_INTERVAL) + "min");
    esp_deep_sleep_start();
  }

  char status[30] = {};

  int wifiRSSI = 0; // “Received Signal Strength Indicator"
  wl_status_t wifiStatus = startWiFi(wifiRSSI);

  bool timeConfigured = false;
  tm timeInfo = {};
  if (wifiStatus == WL_CONNECTED)
  {
    timeConfigured = setupTime(&timeInfo);
  }
  else
  { // ensures last byte is '\0'
    strncpy(status, "WiFi connection failed", sizeof(status) - 1);
  }

  bool rxOWM = false;
  if ((wifiStatus == WL_CONNECTED) && timeConfigured)
  {
    WiFiClient client;
    rxOWM |= getOWMonecall(client, owm_onecall);
    rxOWM |= getOWMairpollution(client, owm_air_pollution);
    if (rxOWM == false)
    {                                       // ensures last byte is '\0'
      strncpy(status, "API call failed", sizeof(status) - 1);
    }
  }
  else
  {
    if (timeConfigured == false)
    {                                         // ensures last byte is '\0'
      strncpy(status, "Time setup failed", sizeof(status) - 1);
    }
  }
  killWiFi();

  // SWITCHING TO BME280
  // read DHT sensor (needs 2s to ensure new readings)
  float inTemp     = 0; // Celsius
  float inHumidity = 0;    // %
  // check if DHT readings are valid
  // note: readings are checked again before drawing to screen. A dash '-' will 
  //       be displayed if an invalid DHT reading is detected.
  if (isnan(inTemp) || isnan(inHumidity)) {
    strncpy(status, "DHT read failed", sizeof(status) - 1);
  }
  delay(20); // Serial print sometimes fails without this delay

  if (rxOWM)
  {
    filterAlerts(owm_onecall.alerts);
    initDisplay();
    debugDisplayBuffer(owm_onecall, owm_air_pollution); // debug, remove later
    
    drawCurrentConditions(owm_onecall.current, owm_onecall.daily[0], 
                          owm_air_pollution, inTemp, inHumidity);
    drawForecast(owm_onecall.daily);
    drawAlerts(owm_onecall.alerts);
    drawLocationDate(CITY_STRING, &timeInfo);
    drawOutlookGraph(owm_onecall.hourly);
    drawStatusBar(status, wifiRSSI, batteryVoltage);
    display.display(false); // full display refresh
    display.powerOff();
  }
  else
  {
    initDisplay();
    drawStatusBar(status, wifiRSSI, batteryVoltage);
    display.display(true); // partial display refresh
    display.powerOff();
  }
  Serial.println("Min Free Mem: " + String(ESP.getMinFreeHeap()));
  Serial.println("Status: " + String(status));
  beginDeepSleep(startTime, &timeInfo);
} // end setup

void loop()
{
  // this will never run
} // end loop
