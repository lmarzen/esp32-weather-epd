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

// too large to allocate locally on stack
static owm_resp_onecall_t       owm_onecall;
static owm_resp_air_pollution_t owm_air_pollution;

void beginDeepSleep(unsigned long &startTime, tm *timeInfo)
{
  if (!getLocalTime(timeInfo))
  {
    Serial.println("Failed to obtain time before deep-sleep, referencing older time.");
  }
  unsigned long sleepDuration = (SLEEP_DURATION * 60
                              - ((timeInfo->tm_min % SLEEP_DURATION) * 60 
                              + timeInfo->tm_sec)) + 1;
  esp_sleep_enable_timer_wakeup(sleepDuration * 1000000);
  Serial.println("Awake for " + String((millis() - startTime) / 1000.0, 3) + "s");
  Serial.println("Deep-sleep for " + String(sleepDuration) + "s");
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
  { // critically low battery, deep-sleep now
    esp_sleep_enable_timer_wakeup(CRIT_LOW_BATTERY_SLEEP_INTERVAL * 60 * 1000000);
    Serial.println("Critically low battery voltage!");
    Serial.println("Deep-sleep for " + String(CRIT_LOW_BATTERY_SLEEP_INTERVAL) + "min");
    esp_deep_sleep_start();
  }
  else if (batteryVoltage <= LOW_BATTERY_VOLTAGE)
  { // low battery, deep-sleep now
    esp_sleep_enable_timer_wakeup(LOW_BATTERY_SLEEP_INTERVAL * 60 * 1000000);
    Serial.println("Low battery voltage!");
    Serial.println("Deep-sleep for " + String(LOW_BATTERY_SLEEP_INTERVAL) + "min");
    esp_deep_sleep_start();
  }

  char status[30] = {};
  
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

  // read DHT sensor (needs 2s to ensure new readings)
  float inTemp     = dht.readTemperature(); // Celsius
  float inHumidity = dht.readHumidity();    // %
  // check if DHT readings are valid
  // note: readings are checked again before drawing to screen. A dash '-' will 
  //       be displayed if an invalid DHT reading is detected.
  if (isnan(inTemp) || isnan(inHumidity)) {
    strncpy(status, "DHT read failed", sizeof(status) - 1);
    Serial.println(status);
  } else {
    Serial.print("Temperature: "); 
    Serial.print(inTemp);
    Serial.println("`C");
    Serial.print("Humidity: "); 
    Serial.print(inHumidity);
    Serial.println("%%");
  }

  if (rxOWM)
  {
    filterAlerts(owm_onecall.alerts);
    initDisplay();
    debugDisplayBuffer(owm_onecall, owm_air_pollution); // debug, remove later
    
    drawCurrentConditions(owm_onecall.current, owm_air_pollution, inTemp, inHumidity);
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
  Serial.println(ESP.getMinFreeHeap());
  beginDeepSleep(startTime, &timeInfo);
}

void loop()
{
  // this will never run
}
