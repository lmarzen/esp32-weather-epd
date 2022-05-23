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
// icon header files
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
int     wifiSignal;
long    startTime = 0;
String  timeStr, dateStr;

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 480

enum alignment {LEFT, RIGHT, CENTER};

// Connections for Waveshare e-paper Driver Board
static const uint8_t EPD_BUSY = 26;
static const uint8_t EPD_CS   = 5;
static const uint8_t EPD_RST  = 27; 
static const uint8_t EPD_DC   = 13; 
static const uint8_t EPD_SCK  = 25;
static const uint8_t EPD_MISO = 12; // Master-In Slave-Out not used, as no data from display
static const uint8_t EPD_MOSI = 2;

GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT> display(GxEPD2_750_T7(/*CS=*/ EPD_CS, /*DC=*/ EPD_DC, /*RST=*/ EPD_RST, /*BUSY=*/ EPD_BUSY)); // B/W display
// GxEPD2_3C<GxEPD2_750c, GxEPD2_750c::HEIGHT> display(GxEPD2_750(/*CS=*/ EPD_CS, /*DC=*/ EPD_DC, /*RST=*/ EPD_RST, /*BUSY=*/ EPD_BUSY));     // 3-colour displays

U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;  // Select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
// Using fonts:
// u8g2_font_helvB08_tf
// u8g2_font_helvB10_tf
// u8g2_font_helvB12_tf
// u8g2_font_helvB14_tf
// u8g2_font_helvB18_tf
// u8g2_font_helvB24_tf

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
    wifiSignal = WiFi.RSSI(); // get Wifi signal strength now, because the WiFi will be turned off to save power!
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
  configTime(0, 0, ntp_server1, ntp_server2); // We will pass 0 for gmtOffset_sec and daylightOffset_sec and use setenv() for timezone offsets
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
  dateStr = day_output;
  timeStr = time_output;
  return true;
}

void BeginSleep() {
  display.powerOff();
  if (!getLocalTime(&timeinfo, 10000)) {
    Serial.println("Failed to obtain time before deep-sleep, using old time.");
  }
  long sleep_timer = (sleep_dur * 60 - ((timeinfo.tm_min % sleep_dur) * 60 + timeinfo.tm_sec));
  esp_sleep_enable_timer_wakeup((sleep_timer + 1) * 1000000LL); // Add 1s extra sleep to allow for fast ESP32 RTCs
  Serial.println("Awake for: " + String((millis() - startTime) / 1000.0, 3) + "s");
  Serial.println("Entering deep-sleep for " + String(sleep_timer) + "(+1)s");
  esp_deep_sleep_start();
}

//#########################################################################################
void drawString(int x, int y, String text, alignment align) {
  int16_t  x1, y1; //the bounds of x,y and w and h of the variable 'text' in pixels.
  uint16_t w, h;
  display.setTextWrap(false);
  display.getTextBounds(text, x, y, &x1, &y1, &w, &h);
  if (align == RIGHT)  x = x - w;
  if (align == CENTER) x = x - w / 2;
  u8g2Fonts.setCursor(x, y + h);
  u8g2Fonts.print(text);
}
//#########################################################################################
void drawStringMaxWidth(int x, int y, unsigned int text_width, String text, alignment align) {
  int16_t  x1, y1; //the bounds of x,y and w and h of the variable 'text' in pixels.
  uint16_t w, h;
  display.getTextBounds(text, x, y, &x1, &y1, &w, &h);
  if (align == RIGHT)  x = x - w;
  if (align == CENTER) x = x - w / 2;
  u8g2Fonts.setCursor(x, y);
  if (text.length() > text_width * 2) {
    u8g2Fonts.setFont(u8g2_font_helvB10_tf);
    text_width = 42;
    y = y - 3;
  }
  u8g2Fonts.println(text.substring(0, text_width));
  if (text.length() > text_width) {
    u8g2Fonts.setCursor(x, y + h + 15);
    String secondLine = text.substring(text_width);
    secondLine.trim(); // Remove any leading spaces
    u8g2Fonts.println(secondLine);
  }
}

void helloWorld()
{
  drawString(100, 100, "Hello World", CENTER);
  display.display(false);
}

void initDisplay() {
  display.init(115200, true, 2, false); // init(uint32_t serial_diag_bitrate, bool initial, uint16_t reset_duration, bool pulldown_rst_mode)
  //display.init(); for older Waveshare HAT's
  SPI.end();
  SPI.begin(EPD_SCK, EPD_MISO, EPD_MOSI, EPD_CS);

  u8g2Fonts.begin(display);                  // connect u8g2 procedures to Adafruit GFX
  u8g2Fonts.setFontMode(1);                  // use u8g2 transparent mode (this is default)
  u8g2Fonts.setFontDirection(0);             // left to right (this is default)
  u8g2Fonts.setForegroundColor(GxEPD_BLACK); // apply Adafruit GFX color
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE); // apply Adafruit GFX color
  u8g2Fonts.setFont(u8g2_font_helvB10_tf);   // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  display.fillScreen(GxEPD_WHITE);
  display.setFullWindow();
}

void updateDisplayBuffer() {
  // NOTE: Using 'drawInvertedBitmap' and not 'drawBitmap' so that images are WYSIWYG, otherwise all images need to be inverted
  display.drawInvertedBitmap(0, 0, wi_day_rain_wind_196x196, 196, 196, GxEPD_BLACK);
  display.drawInvertedBitmap(196, 0, wi_day_rain_wind_160x160, 160, 160, GxEPD_BLACK);
  display.drawInvertedBitmap(356, 0, wi_day_rain_wind_96x96, 96, 96, GxEPD_BLACK);
  display.drawInvertedBitmap(452, 0, wi_day_rain_wind_64x64, 64, 64, GxEPD_BLACK);
  display.drawInvertedBitmap(516, 0, wi_day_rain_wind_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(564, 0, wi_day_rain_wind_32x32, 32, 32, GxEPD_BLACK);
  display.drawInvertedBitmap(596, 0, wi_day_rain_wind_16x16, 16, 16, GxEPD_BLACK);

  display.drawInvertedBitmap(0, 196, house_thermometer_196x196, 196, 196, GxEPD_BLACK);
  display.drawInvertedBitmap(196, 196, house_thermometer_160x160, 160, 160, GxEPD_BLACK);
  display.drawInvertedBitmap(356, 196, house_thermometer_96x96, 96, 96, GxEPD_BLACK);
  display.drawInvertedBitmap(452, 196, house_thermometer_64x64, 64, 64, GxEPD_BLACK);
  display.drawInvertedBitmap(516, 196, house_thermometer_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(564, 196, house_thermometer_32x32, 32, 32, GxEPD_BLACK);
  display.drawInvertedBitmap(596, 196, house_thermometer_16x16, 16, 16, GxEPD_BLACK);
  
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
  startTime = millis();
  Serial.begin(115200);
  // Serial.println("Primes: " + String(countPrimes(1000000)));
  // esp_sleep_enable_timer_wakeup((5) * 1000000LL);
  // Serial.println("Awake for: " + String((millis() - startTime) / 1000.0, 3) + "s");
  // esp_deep_sleep_start();

  // TODO will firebeetle led stay off when on battery? otheriwse desolder...
//#ifdef LED_BUILTIN
//  pinMode(LED_BUILTIN, INPUT); // If it's On, turn it off and some boards use GPIO-5 for SPI-SS, which remains low after screen use
//  digitalWrite(LED_BUILTIN, HIGH);
//#endif

  if ( (StartWifi() == WL_CONNECTED) && SetupTime() ) {
    initDisplay();

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
    updateDisplayBuffer();
    display.display(false); // Full display refresh
  }
  BeginSleep();
}

void loop() { // this will never run
}





