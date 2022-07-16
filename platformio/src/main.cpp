// arduino/esp32 libraries
#include <Arduino.h>
#include <time.h>
#include <WiFi.h>

// external library
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// header files
#include "api_response.h"
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

  // GET BATTERY VOLTAGE
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

  // START WIFI AND CONFIGURE TIME
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

  // MAKE API REQUESTS, if wifi is connected and time is configured
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

  if (rxOWM)
  {
    // GET INDOOR TEMPERATURE AND HUMIDITY, start BME280...
    float inTemp     = NAN;
    float inHumidity = NAN;

    TwoWire I2C_bme = TwoWire(0);
    Adafruit_BME280 bme;

    I2C_bme.begin(PIN_BME_SDA, PIN_BME_SCL, 100000); // 100kHz
    if(bme.begin(BME_ADDRESS, &I2C_bme))
    { 
      inTemp     = bme.readTemperature(); // Celsius
      inHumidity = bme.readHumidity();    // %

      Serial.println(inTemp);
      Serial.println(inHumidity);

      // check if BME readings are valid
      // note: readings are checked again before drawing to screen. If a reading is 
      //       not a number (NAN) then an error occured, a dash '-' will be 
      //       displayed.
      if (isnan(inTemp) || isnan(inHumidity)) {
        strncpy(status, "BME read failed", sizeof(status) - 1);
      }
    }
    else
    {
      strncpy(status, "BME not found", sizeof(status) - 1); // check wiring
    }

    // RENDER FULL REFRESH
    String dateStr;
    getDateStr(dateStr, &timeInfo);

    initDisplay();
    debugDisplayBuffer(owm_onecall, owm_air_pollution); // debug, remove later
    
    drawCurrentConditions(owm_onecall.current, owm_onecall.daily[0], 
                          owm_air_pollution, inTemp, inHumidity);
    drawForecast(owm_onecall.daily, timeInfo);
    drawAlerts(owm_onecall.alerts, CITY_STRING, dateStr);
    drawLocationDate(CITY_STRING, dateStr);
    drawOutlookGraph(owm_onecall.hourly);
    drawStatusBar(status, wifiRSSI, batteryVoltage);
    display.display(false); // full display refresh
    display.powerOff();
  }
  else
  {
    // RENDER STATUS BAR PARTIAL REFRESH
    initDisplay();
    drawStatusBar(status, wifiRSSI, batteryVoltage);
    display.display(true); // partial display refresh
    display.powerOff();
  }

  // DEEP-SLEEP
  Serial.println("Min Free Mem: " + String(ESP.getMinFreeHeap()));
  Serial.println("Status: " + String(status));
  beginDeepSleep(startTime, &timeInfo);
  
} // end setup

void loop()
{
  // this will never run
} // end loop
