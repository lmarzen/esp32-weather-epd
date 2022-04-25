// built-in Arduino/C libraries
#include <Arduino.h>
#include <SPI.h>
#include <WiFi.h>
#include <time.h>

// additional libraries
#include <Adafruit_BusIO_Register.h>
#include <ArduinoJson.h>
#include <GxEPD2_BW.h>
#include <U8g2_for_Adafruit_GFX.h>

// header files
#include "config.h"
#include "lang.h"
#include "icons/icons_16x16.h"
#include "icons/icons_32x32.h"
#include "icons/icons_48x48.h"
#include "icons/icons_64x64.h"
#include "icons/icons_96x96.h"
#include "icons/icons_128x128.h"
#include "icons/icons_160x160.h"
#include "icons/icons_196x196.h"

// GLOBAL VARIABLES
tm timeinfo;
int     wifi_signal;
long    start_time = 0;
String  time_str, date_str;


void printLocalTime()
{
  if(!getLocalTime(&timeinfo, 10000)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

uint8_t StartWifi() {
  WiFi.mode(WIFI_STA);
  Serial.printf("Connecting to '%s'", ssid);
  WiFi.begin(ssid, password);
  
  unsigned long timeout = millis() + 10000; // timeout if wifi does not connect in 10s from now
  uint8_t connection_status = WiFi.status();
  
  while ( (connection_status != WL_CONNECTED) && (millis() < timeout) ) {
    Serial.print(".");
    delay(50);
    connection_status = WiFi.status();
  }
  Serial.println();
  
  if (connection_status == WL_CONNECTED) {
    wifi_signal = WiFi.RSSI(); // get Wifi signal strength now, because the WiFi will be turned off to save power!
    Serial.println("IP: " + WiFi.localIP().toString());
  } else {
    Serial.printf("Could not connect to '%s'\n", ssid);
  }
  return connection_status;
}

void KillWiFi() {
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

bool SetupTime() {
  configTime(0, 0, ntpServer1, ntpServer2); // We will pass 0 for gmtOffset_sec and daylightOffset_sec and use setenv() for timezone offsets
  setenv("TZ", timezone, 1);
  tzset();
  if (!getLocalTime(&timeinfo, 10000)) {
    Serial.println("Failed to obtain time");
    return false;
  }
  Serial.println(&timeinfo, "Time setup complete at: %A, %B %d %Y %H:%M:%S");
  return true;
}

// TODO
bool UpdateTimeDateStrings() {
  char time_output[30], day_output[30], update_time[30];
  // see http://www.cplusplus.com/reference/ctime/strftime/
  if (units[0] == 'm') {
    if ((!strcmp(lang,"cz")) || !(strcmp(lang,"de")) || !(strcmp(lang,"pl")) || !(strcmp(lang,"nl"))){
      sprintf(day_output, "%s, %02u. %s %04u", weekday_D[timeinfo.tm_wday], timeinfo.tm_mday, month_M[timeinfo.tm_mon], (timeinfo.tm_year) + 1900); // day_output >> So., 23. Juni 2019 <<
    }
    else
    {
      sprintf(day_output, "%s %02u-%s-%04u", weekday_D[timeinfo.tm_wday], timeinfo.tm_mday, month_M[timeinfo.tm_mon], (timeinfo.tm_year) + 1900);
    }
    strftime(update_time, sizeof(update_time), "%H:%M:%S", &timeinfo);  // Creates: '14:05:49'
    sprintf(time_output, "%s %s", TXT_UPDATED, update_time);
  }
  else
  {
    strftime(day_output, sizeof(day_output), "%a %b-%d-%Y", &timeinfo); // Creates  'Sat May-31-2019'
    strftime(update_time, sizeof(update_time), "%r", &timeinfo);        // Creates: '02:05:49pm'
    sprintf(time_output, "%s %s", TXT_UPDATED, update_time);
  }
  date_str = day_output;
  time_str = time_output;
  return true;
}

void BeginSleep() {
  // TODO
  //display.powerOff();
  if (!getLocalTime(&timeinfo, 10000)) {
    Serial.println("Failed to obtain time before deep-sleep, using old time.");
  }
  long sleep_timer = (sleep_dur * 60 - ((timeinfo.tm_min % sleep_dur) * 60 + timeinfo.tm_sec));
  esp_sleep_enable_timer_wakeup((sleep_timer + 1) * 1000000LL); // Add 1s extra sleep to allow for fast ESP32 RTC timers
  Serial.println("Awake for: " + String((millis() - start_time) / 1000.0, 3) + "s");
  Serial.println("Entering deep-sleep for " + String(sleep_timer) + "(+1)s");
  esp_deep_sleep_start();
}

/*
// debug performance testing
bool isPrime(int N)
{
    for(int i = 2 ; i * i <= N ; i++)
        if(N % i == 0)
            return false;
    return true;
}
int countPrimes(int N)
{
    if(N < 3)
        return 0;
    int cnt = 1;
    for(int i = 3 ; i < N ; i += 2)
        if(isPrime(i))
            cnt++;
    return cnt;
}
*/

void setup() {
  start_time = millis();
  Serial.begin(115200);
  // Serial.println("Primes: " + String(countPrimes(1000000)));
  // esp_sleep_enable_timer_wakeup((5) * 1000000LL);
  // Serial.println("Awake for: " + String((millis() - start_time) / 1000.0, 3) + "s");
  // esp_deep_sleep_start();

  // TODO will firebeetle led stay off when on battery? otheriwse desolder...
//#ifdef LED_BUILTIN
//  pinMode(LED_BUILTIN, INPUT); // If it's On, turn it off and some boards use GPIO-5 for SPI-SS, which remains low after screen use
//  digitalWrite(LED_BUILTIN, HIGH);
//#endif

  if ( (StartWifi() == WL_CONNECTED) && SetupTime() ) {
    UpdateTimeDateStrings();
    KillWiFi();
    /*
    if ((CurrentHour >= WakeupTime && CurrentHour <= SleepTime) || DebugDisplayUpdate) {
      //InitialiseDisplay(); // Give screen time to initialise by getting weather data!
      uint8_t Attempts = 1;
      bool RxWeather = false, RxForecast = false;
      WiFiClient client;   // wifi client object
      while ((RxWeather == false || RxForecast == false) && Attempts <= 2) { // Try up-to 2 time for Weather and Forecast data
        if (RxWeather  == false) RxWeather  = obtain_wx_data(client, "weather");
        if (RxForecast == false) RxForecast = obtain_wx_data(client, "forecast");
        Attempts++;
      }
      if (RxWeather && RxForecast) { // Only if received both Weather or Forecast proceed
        StopWiFi(); // Reduces power consumption
        DisplayWeather();
        display.display(false); // Full screen update mode
      }
    }
    */
  }
  BeginSleep();
}

void loop() { // this will never run
}





