// arduino/esp32 libraries
#include <Arduino.h>
#include <DHT.h>
#include <SPI.h>
#include <time.h>
#include <WiFi.h>

// header files
#include "api_response.h"
#include "aqi.h"
#include "client_utils.h"
#include "config.h"
#include "display_utils.h"
#include "renderer.h"

void beginHibernate(unsigned long &startTime, tm *timeInfo)
{
  display.powerOff();
  if (!getLocalTime(timeInfo))
  {
    Serial.println("Failed to obtain time before deep-sleep, referencing older time.");
  }
  unsigned long sleepDuration = (SLEEP_DURATION * 60
                              - ((timeInfo->tm_min % SLEEP_DURATION) * 60 
                              + timeInfo->tm_sec)) + 1; // Add 1s extra sleep to allow for fast ESP32 RTCs
  esp_sleep_enable_timer_wakeup(sleepDuration * 1000000);
  Serial.println("Awake for: " + String((millis() - startTime) / 1000.0, 3) + "s");
  Serial.println("Hibernating for " + String(sleepDuration) + "s");
  esp_deep_sleep_start();
}

void setup()
{
  unsigned long startTime = millis();
  Serial.begin(115200);

  // DFRobot FireBeetle Esp32-E V1.0 has voltage divider (1M+1M), so readings 
  // must be multiplied by 2. Readings are divided by 1000 to convert mV to V.
  double batteryVoltage = static_cast<double>(analogRead(PIN_BAT_ADC)) / 1000.0 * (3.3 / 2.0);
  Serial.println("Battery voltage: " + String(batteryVoltage,2));
  if (batteryVoltage <= CRIT_LOW_BATTERY_VOLTAGE)
  { // critically low battery, hibernate now
    esp_sleep_enable_timer_wakeup(CRIT_LOW_BATTERY_SLEEP_INTERVAL * 60 * 1000000);
    Serial.println("Critically low battery voltage!");
    Serial.println("Hibernating for " + String(CRIT_LOW_BATTERY_SLEEP_INTERVAL) + "min");
    esp_deep_sleep_start();
  }
  else if (batteryVoltage <= LOW_BATTERY_VOLTAGE)
  { // low battery, hibernate now
    esp_sleep_enable_timer_wakeup(LOW_BATTERY_SLEEP_INTERVAL * 60 * 1000000);
    Serial.println("Low battery voltage!");
    Serial.println("Hibernating for " + String(LOW_BATTERY_SLEEP_INTERVAL) + "min");
    esp_deep_sleep_start();
  }

  char statusStr[30] = {};
  
  // begin dht now. DHT may have old readings and may take up to 2s to refresh
  DHT dht(PIN_DHT, DHT_TYPE);
  dht.begin();

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
    strncpy(statusStr, "WiFi connection failed", sizeof(statusStr) - 1);
  }

  bool rxOWM = false;
  owm_resp_onecall_t       owm_onecall = {};
  owm_resp_air_pollution_t owm_air_pollution = {};
  if ((wifiStatus == WL_CONNECTED) && timeConfigured)
  {
    WiFiClient client;
    rxOWM |= getOWMonecall(client, owm_onecall);
    rxOWM |= getOWMairpollution(client, owm_air_pollution);
    if (rxOWM == false)
    {                                       // ensures last byte is '\0'
      strncpy(statusStr, "API call failed", sizeof(statusStr) - 1);
    }
  }
  else
  {
    if (timeConfigured == false)
    {                                         // ensures last byte is '\0'
      strncpy(statusStr, "Time setup failed", sizeof(statusStr) - 1);
    }
  }
  killWiFi();

  // read DHT sensor (needs 2s to ensure new readings)
  float inTemp     = dht.readTemperature(); // Celsius
  float inHumidity = dht.readHumidity();    // %
  // check if DHT readings are valid
  // note: readings are checked again before drawing to screen. A dash '-' will 
  //       be displayed if an invalid DHT reading is detected.
  if (isnan(inTemp) || isnan(inHumidity)) {
    strncpy(statusStr, "DHT read failed", sizeof(statusStr) - 1);
    Serial.println(statusStr);
  } else {
    Serial.print("Temperature: "); 
    Serial.print(inTemp);
    Serial.println("`C");
    Serial.print("Humidity: "); 
    Serial.print(inHumidity);
    Serial.println("%%");
  }

  initDisplay();

  if (rxOWM)
  {
    //snprintf(statusStr, sizeof(statusStr), "%A, %B %d %Y %H:%M:%S", timeInfo);
    
    drawCurrentConditions(owm_onecall.current, owm_air_pollution, inTemp, inHumidity);
    drawForecast(owm_onecall.daily);
    drawAlerts(owm_onecall.alerts);
    drawLocationDate(CITY_STRING, timeInfo);
    drawOutlookGraph(owm_onecall.hourly);
    drawStatusBar(statusStr, wifiRSSI, batteryVoltage);
    display.display(false); // Full display refresh
  }
  else
  {
    drawStatusBar(statusStr, wifiRSSI, batteryVoltage);
    display.display(true); // partial display refresh
  }

  beginHibernate(startTime, &timeInfo);
}

void loop()
{
  // this will never run
}
