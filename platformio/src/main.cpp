/* Main program for ESP32 Financial Ticker + Weather Display.
 * Based on esp32-weather-epd by Luke Marzen.
 *
 * Extended with multi-page financial ticker rotation:
 *   Page 0: Cryptocurrency (CoinGecko API)
 *   Page 1: Stock Indices (Yahoo Finance)
 *   Page 2: Commodities (Yahoo Finance)
 *   Page 3: Foreign Exchange (Yahoo Finance)
 *   Page 4: Weather (OpenWeatherMap - original display)
 *
 * Copyright (C) 2022-2025  Luke Marzen
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

#include "config.h"
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Preferences.h>
#include <time.h>
#include <WiFi.h>
#include <Wire.h>

#include "_locale.h"
#include "api_response.h"
#include "client_utils.h"
#include "config.h"
#include "display_utils.h"
#include "icons/icons_196x196.h"
#include "renderer.h"
#include "user_config.h"

#if defined(SENSOR_BME280)
  #include <Adafruit_BME280.h>
#endif
#if defined(SENSOR_BME680)
  #include <Adafruit_BME680.h>
#endif
#if defined(USE_HTTPS_WITH_CERT_VERIF) || defined(USE_HTTPS_NO_CERT_VERIF)
  #include <WiFiClientSecure.h>
#endif
#ifdef USE_HTTPS_WITH_CERT_VERIF
  #include "cert.h"
#endif

// ── RTC Memory (survives deep sleep) ────────────────────────────────────────
RTC_DATA_ATTR int currentPage = 0;
RTC_DATA_ATTR time_t lastDataFetch = 0;

// Financial data pages cached in RTC memory
RTC_DATA_ATTR page_data_t cryptoPage   = {};
RTC_DATA_ATTR page_data_t indicesPage  = {};
RTC_DATA_ATTR page_data_t commoditiesPage = {};
RTC_DATA_ATTR page_data_t forexPage    = {};

// too large to allocate locally on stack
static owm_resp_onecall_t       owm_onecall;
static owm_resp_air_pollution_t owm_air_pollution;

Preferences prefs;

/* Deep sleep for the configured page duration (default 60 seconds).
 */
void beginPageSleep(unsigned long startTime)
{
  uint64_t sleepDuration = PAGE_DURATION_SEC;

  // Add small compensation for ESP32 RTC drift
  sleepDuration += 3ULL;

#if DEBUG_LEVEL >= 1
  printHeapUsage();
#endif

  esp_sleep_enable_timer_wakeup(sleepDuration * 1000000ULL);
  Serial.print("Awake for ");
  Serial.println(String((millis() - startTime) / 1000.0, 3) + "s");
  Serial.print("Sleeping for ");
  Serial.println(String((uint32_t)sleepDuration) + "s (next page: "
                 + String(currentPage) + ")");
  esp_deep_sleep_start();
}

/* Render the weather page using the original weather display pipeline.
 * This is called when currentPage == 4 (TOTAL_PAGES - 1).
 */
void renderWeatherPage(unsigned long startTime, uint32_t batteryVoltage,
                       int wifiRSSI, tm &timeInfo, bool timeConfigured)
{
  // MAKE WEATHER API REQUESTS
#ifdef USE_HTTP
  WiFiClient client;
#elif defined(USE_HTTPS_NO_CERT_VERIF)
  WiFiClientSecure client;
  client.setInsecure();
#elif defined(USE_HTTPS_WITH_CERT_VERIF)
  WiFiClientSecure client;
  client.setCACert(cert_Sectigo_RSA_Organization_Validation_Secure_Server_CA);
#endif

  String statusStr = {};
  String tmpStr = {};

  int rxStatus = getOWMonecall(client, owm_onecall);
  if (rxStatus != HTTP_CODE_OK)
  {
    killWiFi();
    statusStr = "One Call " + OWM_ONECALL_VERSION + " API";
    tmpStr = String(rxStatus, DEC) + ": " + getHttpResponsePhrase(rxStatus);
    initDisplay();
    do
    {
      drawError(wi_cloud_down_196x196, statusStr, tmpStr);
    } while (display.nextPage());
    powerOffDisplay();
    // Advance page so we don't get stuck on weather errors
    currentPage = (currentPage + 1) % TOTAL_PAGES;
    beginPageSleep(startTime);
    return;
  }
  rxStatus = getOWMairpollution(client, owm_air_pollution);
  if (rxStatus != HTTP_CODE_OK)
  {
    killWiFi();
    statusStr = "Air Pollution API";
    tmpStr = String(rxStatus, DEC) + ": " + getHttpResponsePhrase(rxStatus);
    initDisplay();
    do
    {
      drawError(wi_cloud_down_196x196, statusStr, tmpStr);
    } while (display.nextPage());
    powerOffDisplay();
    currentPage = (currentPage + 1) % TOTAL_PAGES;
    beginPageSleep(startTime);
    return;
  }
  killWiFi(); // WiFi no longer needed

  // GET INDOOR TEMPERATURE AND HUMIDITY, start BMEx80...
  pinMode(PIN_BME_PWR, OUTPUT);
  digitalWrite(PIN_BME_PWR, HIGH);
#if defined(SENSOR_INIT_DELAY_MS) && SENSOR_INIT_DELAY_MS > 0
  delay(SENSOR_INIT_DELAY_MS);
#endif
  TwoWire I2C_bme = TwoWire(0);
  I2C_bme.begin(PIN_BME_SDA, PIN_BME_SCL, 100000); // 100kHz
  float inTemp     = NAN;
  float inHumidity = NAN;
#if defined(SENSOR_BME280)
  Serial.print(String(TXT_READING_FROM) + " BME280... ");
  Adafruit_BME280 bme;
  if(bme.begin(BME_ADDRESS, &I2C_bme))
  {
#endif
#if defined(SENSOR_BME680)
  Serial.print(String(TXT_READING_FROM) + " BME680... ");
  Adafruit_BME680 bme(&I2C_bme);
  if(bme.begin(BME_ADDRESS))
  {
#endif
    inTemp     = bme.readTemperature(); // Celsius
    inHumidity = bme.readHumidity();    // %
    if (std::isnan(inTemp) || std::isnan(inHumidity))
    {
      statusStr = "BME " + String(TXT_READ_FAILED);
      Serial.println(statusStr);
    }
    else
    {
      Serial.println(TXT_SUCCESS);
    }
  }
  else
  {
    statusStr = "BME " + String(TXT_NOT_FOUND); // check wiring
    Serial.println(statusStr);
  }
  digitalWrite(PIN_BME_PWR, LOW);

  String refreshTimeStr;
  getRefreshTimeStr(refreshTimeStr, timeConfigured, &timeInfo);
  String dateStr;
  getDateStr(dateStr, &timeInfo);

  // RENDER WEATHER PAGE (identical to original)
  initDisplay();
  do
  {
    drawCurrentConditions(owm_onecall.current, owm_onecall.daily[0],
                          owm_air_pollution, inTemp, inHumidity);
    drawOutlookGraph(owm_onecall.hourly, owm_onecall.daily, timeInfo);
    drawForecast(owm_onecall.daily, timeInfo);
    drawLocationDate(CITY_STRING, dateStr);
#if DISPLAY_ALERTS
    drawAlerts(owm_onecall.alerts, CITY_STRING, dateStr);
#endif
    drawStatusBar(statusStr, refreshTimeStr, wifiRSSI, batteryVoltage);
  } while (display.nextPage());
  powerOffDisplay();
}

/* Program entry point.
 * Implements the multi-page rotation:
 *   - Wake from deep sleep
 *   - Connect WiFi + sync time
 *   - Check if data refresh is needed (every full cycle)
 *   - Render the current page
 *   - Advance to next page
 *   - Deep sleep for PAGE_DURATION_SEC
 */
void setup()
{
  unsigned long startTime = millis();
  Serial.begin(115200);

  Serial.println("\n=== ESP32 Financial Ticker + Weather ===");
  Serial.println("Page: " + String(currentPage) + "/" + String(TOTAL_PAGES - 1));

#if DEBUG_LEVEL >= 1
  printHeapUsage();
#endif

  disableBuiltinLED();

  // Open namespace for read/write to non-volatile storage
  prefs.begin(NVS_NAMESPACE, false);

#if BATTERY_MONITORING
  uint32_t batteryVoltage = readBatteryVoltage();
  Serial.print(TXT_BATTERY_VOLTAGE);
  Serial.println(": " + String(batteryVoltage) + "mv");

  // When the battery is low, the display should be updated to reflect that, but
  // only the first time we detect low voltage.
  bool lowBat = prefs.getBool("lowBat", false);

  // low battery, deep sleep now
  if (batteryVoltage <= LOW_BATTERY_VOLTAGE)
  {
    if (lowBat == false)
    { // battery is now low for the first time
      prefs.putBool("lowBat", true);
      prefs.end();
      initDisplay();
      do
      {
        drawError(battery_alert_0deg_196x196, TXT_LOW_BATTERY);
      } while (display.nextPage());
      powerOffDisplay();
    }

    if (batteryVoltage <= CRIT_LOW_BATTERY_VOLTAGE)
    { // critically low battery
      Serial.println(TXT_CRIT_LOW_BATTERY_VOLTAGE);
      Serial.println(TXT_HIBERNATING_INDEFINITELY_NOTICE);
    }
    else if (batteryVoltage <= VERY_LOW_BATTERY_VOLTAGE)
    { // very low battery
      esp_sleep_enable_timer_wakeup(VERY_LOW_BATTERY_SLEEP_INTERVAL
                                    * 60ULL * 1000000ULL);
      Serial.println(TXT_VERY_LOW_BATTERY_VOLTAGE);
      Serial.print(TXT_ENTERING_DEEP_SLEEP_FOR);
      Serial.println(" " + String(VERY_LOW_BATTERY_SLEEP_INTERVAL) + "min");
    }
    else
    { // low battery
      esp_sleep_enable_timer_wakeup(LOW_BATTERY_SLEEP_INTERVAL
                                    * 60ULL * 1000000ULL);
      Serial.println(TXT_LOW_BATTERY_VOLTAGE);
      Serial.print(TXT_ENTERING_DEEP_SLEEP_FOR);
      Serial.println(" " + String(LOW_BATTERY_SLEEP_INTERVAL) + "min");
    }
    esp_deep_sleep_start();
  }
  // battery is no longer low, reset variable in non-volatile storage
  if (lowBat == true)
  {
    prefs.putBool("lowBat", false);
  }
#else
  uint32_t batteryVoltage = UINT32_MAX;
#endif

  // All data should have been loaded from NVS. Close filesystem.
  prefs.end();

  tm timeInfo = {};

  // ── START WIFI ──
  int wifiRSSI = 0;
  wl_status_t wifiStatus = startWiFi(wifiRSSI);
  if (wifiStatus != WL_CONNECTED)
  { // WiFi Connection Failed
    killWiFi();
    initDisplay();
    if (wifiStatus == WL_NO_SSID_AVAIL)
    {
      Serial.println(TXT_NETWORK_NOT_AVAILABLE);
      do
      {
        drawError(wifi_x_196x196, TXT_NETWORK_NOT_AVAILABLE);
      } while (display.nextPage());
    }
    else
    {
      Serial.println(TXT_WIFI_CONNECTION_FAILED);
      do
      {
        drawError(wifi_x_196x196, TXT_WIFI_CONNECTION_FAILED);
      } while (display.nextPage());
    }
    powerOffDisplay();
    // Advance page so we don't get stuck on WiFi errors
    currentPage = (currentPage + 1) % TOTAL_PAGES;
    beginPageSleep(startTime);
  }

  // ── TIME SYNCHRONIZATION ──
  configTzTime(TIMEZONE, NTP_SERVER_1, NTP_SERVER_2);
  bool timeConfigured = waitForSNTPSync(&timeInfo);
  if (!timeConfigured)
  {
    Serial.println(TXT_TIME_SYNCHRONIZATION_FAILED);
    killWiFi();
    initDisplay();
    do
    {
      drawError(wi_time_4_196x196, TXT_TIME_SYNCHRONIZATION_FAILED);
    } while (display.nextPage());
    powerOffDisplay();
    currentPage = (currentPage + 1) % TOTAL_PAGES;
    beginPageSleep(startTime);
  }

  // ── CHECK IF DATA REFRESH IS NEEDED ──
  // Refresh all financial data every full cycle (TOTAL_PAGES * PAGE_DURATION_SEC)
  // or on first boot (lastDataFetch == 0)
  time_t now = time(nullptr);
  int refreshInterval = PAGE_DURATION_SEC * TOTAL_PAGES;
  bool needsRefresh = (lastDataFetch == 0)
                    || (now - lastDataFetch >= refreshInterval);

  if (needsRefresh && currentPage < (TOTAL_PAGES - 1))
  {
    // Fetch all financial data (weather fetches its own data on its page)
    Serial.println("Data refresh triggered (age: "
                   + String(lastDataFetch > 0 ? (int)(now - lastDataFetch) : -1)
                   + "s)");
    fetchAllFinancialData(cryptoPage, indicesPage, commoditiesPage, forexPage);
    lastDataFetch = now;
  }

  // ── RENDER CURRENT PAGE ──
  const char *pageNames[] = {"Cryptocurrency", "Stock Indices", "Commodities",
                              "FX Rates", "Weather"};
  const char *pageTitles[] = {"CRYPTOCURRENCY", "STOCK INDICES", "COMMODITIES",
                               "FX RATES", "Weather"};

  Serial.println("Rendering: " + String(pageNames[currentPage]));

  if (currentPage < (TOTAL_PAGES - 1))
  {
    // Financial pages (0-3)
    killWiFi(); // WiFi no longer needed for rendering

    page_data_t *pages[] = {&cryptoPage, &indicesPage,
                            &commoditiesPage, &forexPage};

    initDisplay();
    do
    {
      renderFinancialPage(*pages[currentPage], pageTitles[currentPage],
                          currentPage + 1, TOTAL_PAGES,
                          wifiRSSI, batteryVoltage);
    } while (display.nextPage());
    powerOffDisplay();
  }
  else
  {
    // Weather page (page 4) — uses original rendering pipeline
    renderWeatherPage(startTime, batteryVoltage, wifiRSSI,
                      timeInfo, timeConfigured);
  }

  // ── ADVANCE TO NEXT PAGE ──
  currentPage = (currentPage + 1) % TOTAL_PAGES;

  // ── DEEP SLEEP ──
  beginPageSleep(startTime);
} // end setup

/* This will never run.
 */
void loop()
{
} // end loop
