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

void beginSleep(unsigned long &startTime, tm *timeInfo)
{
  display.powerOff();
  if (!getLocalTime(timeInfo, 10000))
  {
    Serial.println("Failed to obtain time before deep-sleep, referencing older time.");
  }
  long sleepTimer = (SLEEP_DUR * 60
                    - ((timeInfo->tm_min % SLEEP_DUR) * 60 
                    + timeInfo->tm_sec)) + 1; // Add 1s extra sleep to allow for fast ESP32 RTCs
  esp_sleep_enable_timer_wakeup((sleepTimer) * 1000000LL);
  Serial.println("Awake for: " + String((millis() - startTime) / 1000.0, 3) + "s");
  Serial.println("Entering deep-sleep for " + String(sleepTimer) + "s");
  esp_deep_sleep_start();
}

void setup()
{
  unsigned long startTime = millis();
  Serial.begin(115200);

  owm_resp_onecall_t       owm_onecall = {};
  owm_resp_air_pollution_t owm_air_pollution = {};

  int wifiRSSI = 0; // “Received Signal Strength Indicator"
  wl_status_t wifiStatus = startWiFi(wifiRSSI);

  bool timeConfigured = false;
  tm timeInfo = {};
  if (wifiStatus == WL_CONNECTED)
  {
    timeConfigured = setupTime(&timeInfo);
  }

  bool rxOWM = false;
  if ((wifiStatus == WL_CONNECTED) && timeConfigured)
  {
    WiFiClient client;
    rxOWM |= getOWMonecall(client, owm_onecall);
    rxOWM |= getOWMairpollution(client, owm_air_pollution);
  }
  killWiFi();
  initDisplay();

  if (rxOWM)
  {
    refreshDisplayBuffer(owm_onecall, owm_air_pollution);
    display.display(false); // Full display refresh
  }
  else
  {
    // refreshStatusBuffer(wifiStatus, timeConfigured, rxOWM); // TODO
    display.display(true); // partial display refresh
  }

  beginSleep(startTime, &timeInfo);
}

void loop()
{
  // this will never run
}
