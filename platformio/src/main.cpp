#include <Arduino.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <Preferences.h>
#include <time.h>
#include <WiFi.h>
#include <Wire.h>

#include "api_response.h"
#include "client_utils.h"
#include "config.h"
#include "display_utils.h"
#include "renderer.h"

#include "icons/icons_196x196.h"

// too large to allocate locally on stack
static owm_resp_onecall_t       owm_onecall;
static owm_resp_air_pollution_t owm_air_pollution;

Preferences prefs;

/* Put esp32 into ultra low-power deep-sleep (<11μA).
 * Alligns wake time to the minute. Sleep times defined in config.cpp.
 */
void beginDeepSleep(unsigned long &startTime, tm *timeInfo)
{
  if (!getLocalTime(timeInfo))
  {
    Serial.print("Failed to obtain time before deep-sleep");
    Serial.println(", referencing older time.");
  }

  int extraHoursUntilWake;
  if (BED_TIME < WAKE_TIME 
   && timeInfo->tm_hour >= BED_TIME && timeInfo->tm_hour < WAKE_TIME)
  { // 0              B   v  W  24
    // |--------------zzzzZzz---|
    extraHoursUntilWake = WAKE_TIME - timeInfo->tm_hour;
  }
  else if (BED_TIME > WAKE_TIME 
   && timeInfo->tm_hour < WAKE_TIME)
  { // 0 v W               B    24
    // |zZz----------------zzzzz|
    extraHoursUntilWake = WAKE_TIME - timeInfo->tm_hour;
  }
  else if (BED_TIME > WAKE_TIME 
   && timeInfo->tm_hour >= BED_TIME)
  { // 0   W               B  v 24
    // |zzz----------------zzzZz|
    extraHoursUntilWake = WAKE_TIME - (timeInfo->tm_hour - 24);
  }
  else // This feature is disabled (BED_TIME == WAKE_TIME)
  {    // OR it is not past BED_TIME
    extraHoursUntilWake = 0;
  }

  unsigned long sleepDuration;
  if (extraHoursUntilWake == 0)
  { // align wake time to nearest multiple of SLEEP_DURATION
    sleepDuration = SLEEP_DURATION * 60UL 
                    - ((timeInfo->tm_min % SLEEP_DURATION) * 60UL 
                        + timeInfo->tm_sec);
  }
  else
  { // align wake time to the hour
    sleepDuration = extraHoursUntilWake * 3600UL
                    - (timeInfo->tm_min * 60UL + timeInfo->tm_sec);
  }

  // if we are within 2 minutes of the next alignment.
  if (sleepDuration <= 120UL)
  {
    sleepDuration += SLEEP_DURATION * 60UL;
  }
  
  // add extra delay to compensate for esp32's with fast RTCs.
  sleepDuration = (unsigned long) (sleepDuration * 1.0067) + 5UL;

  esp_sleep_enable_timer_wakeup(sleepDuration * 1000000);
  Serial.println("Awake for " 
                 + String((millis() - startTime) / 1000.0, 3) + "s");
  Serial.println("Deep-sleep for " + String(sleepDuration) + "s");
  esp_deep_sleep_start();
} // end beginDeepSleep

/* Program entry point.
 */
void setup()
{
  unsigned long startTime = millis();
  Serial.begin(115200);

  // GET BATTERY VOLTAGE
  // DFRobot FireBeetle Esp32-E V1.0 has voltage divider (1M+1M), so readings 
  // are multiplied by 2. Readings are divided by 1000 to convert mV to V.
  double batteryVoltage = 
            static_cast<double>(analogRead(PIN_BAT_ADC)) / 1000.0 * (3.5 / 2.0);
            // use / 1000.0 * (3.3 / 2.0) multiplier above for firebeetle esp32
            // use / 1000.0 * (3.5 / 2.0) for firebeetle esp32-E
  Serial.println("Battery voltage: " + String(batteryVoltage,2));

  // When the battery is low, the display should be updated to reflect that, but
  // only the first time we detect low voltage. The next time the display will
  // refresh is when voltage is no longer low. To keep track of that we will 
  // make use of non-volatile storage.
  // Open namespace for read/write to non-volatile storage
  prefs.begin("lowBat", false);
  bool lowBat = prefs.getBool("lowBat", false);

  // low battery, deep-sleep now
  if (batteryVoltage <= LOW_BATTERY_VOLTAGE)
  {
    if (lowBat == false)
    { // battery is now low for the first time
      prefs.putBool("lowBat", true);
      initDisplay();
      drawError(battery_alert_0deg_196x196, "Low Battery", "");
      display.display(false); // full display refresh
      display.powerOff();
    }

    if (batteryVoltage <= CRIT_LOW_BATTERY_VOLTAGE)
    { // critically low battery
      // don't set esp_sleep_enable_timer_wakeup();
      // We won't wake up again until someone manually presses the RST button.
      Serial.println("Critically low battery voltage!");
      Serial.println("Hibernating without wake time!");
    }
    else if (batteryVoltage <= VERY_LOW_BATTERY_VOLTAGE)
    { // very low battery
      esp_sleep_enable_timer_wakeup(VERY_LOW_BATTERY_SLEEP_INTERVAL 
                                    * 60ULL * 1000000ULL);
      Serial.println("Very low battery voltage!");
      Serial.println("Deep-sleep for " 
                     + String(VERY_LOW_BATTERY_SLEEP_INTERVAL) + "min");
    }
    else
    { // low battery
      esp_sleep_enable_timer_wakeup(LOW_BATTERY_SLEEP_INTERVAL
                                    * 60ULL * 1000000ULL);
      Serial.println("Low battery voltage!");
      Serial.println("Deep-sleep for " 
                    + String(LOW_BATTERY_SLEEP_INTERVAL) + "min");
    }
    esp_deep_sleep_start();
  }
  // battery is no longer low, reset variable in non-volatile storage
  if (lowBat == true)
  {
    prefs.putBool("lowBat", false);
  }

  String statusStr = {};
  String tmpStr = {};
  tm timeInfo = {};

  // START WIFI
  int wifiRSSI = 0; // “Received Signal Strength Indicator"
  wl_status_t wifiStatus = startWiFi(wifiRSSI);
  if (wifiStatus != WL_CONNECTED)
  { // WiFi Connection Failed
    killWiFi();
    initDisplay();
    if (wifiStatus == WL_NO_SSID_AVAIL)
    {
      Serial.println("SSID Not Available");
      drawError(wifi_off_196x196, "SSID Not Available", "");
    }
    else
    {
      Serial.println("WiFi Connection Failed");
      drawError(wifi_off_196x196, "WiFi Connection", "Failed");
    }
    display.display(false); // full display refresh
    display.powerOff();
    beginDeepSleep(startTime, &timeInfo);
  }
  
  // FETCH TIME
  bool timeConfigured = false;
  timeConfigured = setupTime(&timeInfo);
  if (!timeConfigured)
  { // Failed To Fetch The Time
    Serial.println("Failed To Fetch The Time");
    killWiFi();
    initDisplay();
    drawError(wi_time_4_196x196, "Failed To Fetch", "The Time");
    display.display(false); // full display refresh
    display.powerOff();
    beginDeepSleep(startTime, &timeInfo);
  }
  String refreshTimeStr;
  getRefreshTimeStr(refreshTimeStr, timeConfigured, &timeInfo);

  // MAKE API REQUESTS
  int rxOWM[2] = {};
  WiFiClient client;
  rxOWM[0] = getOWMonecall(client, owm_onecall);
  if (rxOWM[0] != HTTP_CODE_OK)
  {
    statusStr = "One Call " + OWM_ONECALL_VERSION + " API";
    tmpStr = String(rxOWM[0], DEC) + ": " + getHttpResponsePhrase(rxOWM[0]);
    killWiFi();
    initDisplay();
    drawError(wi_cloud_down_196x196, statusStr, tmpStr);
    display.display(false); // full display refresh
    display.powerOff();
    beginDeepSleep(startTime, &timeInfo);
  }
  rxOWM[1] = getOWMairpollution(client, owm_air_pollution);
  killWiFi(); // wifi no longer needed
  if (rxOWM[1] != HTTP_CODE_OK)
  {
    statusStr = "Air Pollution API";
    tmpStr = String(rxOWM[1], DEC) + ": " + getHttpResponsePhrase(rxOWM[1]);
    initDisplay();
    drawError(wi_cloud_down_196x196, statusStr, tmpStr);
    display.display(false); // full display refresh
    display.powerOff();
    beginDeepSleep(startTime, &timeInfo);
  }

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

    // check if BME readings are valid
    // note: readings are checked again before drawing to screen. If a reading
    //       is not a number (NAN) then an error occured, a dash '-' will be
    //       displayed.
    if (isnan(inTemp) || isnan(inHumidity)) {
      statusStr = "BME read failed";
    }
  }
  else
  {
    statusStr = "BME not found"; // check wiring
  }

  // RENDER FULL REFRESH
  String dateStr;
  getDateStr(dateStr, &timeInfo);

  initDisplay();
  drawCurrentConditions(owm_onecall.current, owm_onecall.daily[0], 
                        owm_air_pollution, inTemp, inHumidity);
  drawForecast(owm_onecall.daily, timeInfo);
  drawAlerts(owm_onecall.alerts, CITY_STRING, dateStr);
  drawLocationDate(CITY_STRING, dateStr);
  drawOutlookGraph(owm_onecall.hourly, timeInfo);
  drawStatusBar(statusStr, refreshTimeStr, wifiRSSI, batteryVoltage);
  display.display(false); // full display refresh
  display.powerOff();

  // DEEP-SLEEP
  Serial.println("Status: " + statusStr);
  beginDeepSleep(startTime, &timeInfo);
} // end setup

/* This will never run
 */
void loop()
{
} // end loop
