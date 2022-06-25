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

// fonts (modified font files that have the degree symbol mapped to '`')
#include "fonts/FreeSans6pt7b.h"
#include "fonts/FreeSans7pt7b.h"
#include "fonts/FreeSans8pt7b.h"
#include "fonts/FreeSans9pt7b.h"
#include "fonts/FreeSans10pt7b.h"
#include "fonts/FreeSans11pt7b.h"
#include "fonts/FreeSans12pt7b.h"
#include "fonts/FreeSans14pt7b.h"
#include "fonts/FreeSans16pt7b.h"
#include "fonts/FreeSans18pt7b.h"
#include "fonts/FreeSans20pt7b.h"
#include "fonts/FreeSans22pt7b.h"
#include "fonts/FreeSans24pt7b.h"
#include "fonts/FreeSans26pt7b.h"
// only has character set used for displaying temperature (0123456789.-`)
#include "fonts/FreeSans48pt_temperature.h"

// icon header files
#include "icons/icons_16x16.h"
#include "icons/icons_32x32.h"
#include "icons/icons_48x48.h"
#include "icons/icons_64x64.h"
#include "icons/icons_96x96.h"
#include "icons/icons_128x128.h"
#include "icons/icons_160x160.h"
#include "icons/icons_196x196.h"

// header files
#include "api_response.h"
#include "aqi.h"
#include "config.h"
#include "lang.h"
#include "weather_utils.h"

// PREPROCESSOR MACROS
#define DISP_WIDTH  800
#define DISP_HEIGHT 480

// GLOBAL VARIABLES
tm timeinfo;
int wifiSignal;
long startTime = 0;
owm_resp_onecall_t       owm_onecall = {};
owm_resp_air_pollution_t owm_air_pollution = {};
String timeStr, dateStr;

enum alignment
{
  LEFT,
  RIGHT,
  CENTER
};

// B/W display
GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT> display(
  GxEPD2_750_T7(config::PIN_EPD_CS,
                config::PIN_EPD_DC,
                config::PIN_EPD_RST,
                config::PIN_EPD_BUSY));
// 3-colour displays
// GxEPD2_3C<GxEPD2_750c, GxEPD2_750c::HEIGHT> display(
//   GxEPD2_750(config::PIN_EPD_CS,
//              config::PIN_EPD_DC,
//              config::PIN_EPD_RST,
//              config::PIN_EPD_BUSY));

void printLocalTime()
{
  if (!getLocalTime(&timeinfo, 10000))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

wl_status_t startWiFi()
{
  WiFi.mode(WIFI_STA);
  Serial.printf("Connecting to '%s'", config::WIFI_SSID);
  WiFi.begin(config::WIFI_SSID, config::WIFI_PASSWORD);

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
    wifiSignal = WiFi.RSSI(); // get Wifi signal strength now, because the WiFi will be turned off to save power!
    Serial.println("IP: " + WiFi.localIP().toString());
  }
  else
  {
    Serial.printf("Could not connect to '%s'\n", config::WIFI_SSID);
  }
  return connection_status;
}

void killWiFi()
{
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

bool setupTime()
{
  configTime(0, 0, config::NTP_SERVER_1, config::NTP_SERVER_2); // We will pass 0 for gmtOffset_sec and daylightOffset_sec and use setenv() for timezone offsets
  setenv("TZ", config::TIMEZONE, 1);
  tzset();
  if (!getLocalTime(&timeinfo, 10000))
  {
    Serial.println("Failed to obtain time");
    return false;
  }
  Serial.println(&timeinfo, "Time setup complete at: %A, %B %d %Y %H:%M:%S");
  return true;
}

// TODO
bool updateTimeDateStrings()
{
  char time_output[30], day_output[30], update_time[30];
  // see http://www.cplusplus.com/reference/ctime/strftime/
  if (config::UNITS == 'm')
  {
    if ((config::LANG == "cz") 
     || (config::LANG == "de") 
     || (config::LANG == "pl") 
     || (config::LANG == "nl"))
    {
      sprintf(day_output, "%s, %02u. %s %04u", weekday_D[timeinfo.tm_wday], timeinfo.tm_mday, month_M[timeinfo.tm_mon], (timeinfo.tm_year) + 1900); // day_output >> So., 23. Juni 2019 <<
    }
    else
    {
      sprintf(day_output, "%s %02u-%s-%04u", weekday_D[timeinfo.tm_wday], timeinfo.tm_mday, month_M[timeinfo.tm_mon], (timeinfo.tm_year) + 1900);
    }
    strftime(update_time, sizeof(update_time), "%H:%M:%S", &timeinfo); // Creates: '14:05:49'
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

void beginSleep()
{
  display.powerOff();
  if (!getLocalTime(&timeinfo, 10000))
  {
    Serial.println("Failed to obtain time before deep-sleep, using old time.");
  }
  long sleep_timer = (config::SLEEP_DUR * 60 
                     - ((timeinfo.tm_min % config::SLEEP_DUR) * 60 
                     + timeinfo.tm_sec));
  esp_sleep_enable_timer_wakeup((sleep_timer + 1) * 1000000LL); // Add 1s extra sleep to allow for fast ESP32 RTCs
  Serial.println("Awake for: " + String((millis() - startTime) / 1000.0, 3) + "s");
  Serial.println("Entering deep-sleep for " + String(sleep_timer) + "(+1)s");
  esp_deep_sleep_start();
}

void drawString(int x, int y, String text, alignment align)
{
  int16_t x1, y1;
  uint16_t w, h;
  display.setTextWrap(false);
  display.getTextBounds(text, x, y, &x1, &y1, &w, &h);
  if (align == RIGHT)
    x = x - w;
  if (align == CENTER)
    x = x - w / 2;
  display.setCursor(x, y);
  display.print(text);
}

void initDisplay()
{
  display.init(115200, true, 2, false);
  // display.init(); for older Waveshare HAT's
  SPI.end();
  SPI.begin(config::PIN_EPD_SCK,
            config::PIN_EPD_MISO,
            config::PIN_EPD_MOSI,
            config::PIN_EPD_CS);

  display.setRotation(0);
  display.setTextSize(1);
  display.setTextColor(GxEPD_BLACK);
  display.fillScreen(GxEPD_WHITE);
  display.setFullWindow();
}

void updateDisplayBuffer()
{
  // location, date
  display.setFont(&FreeSans16pt7b);
  drawString(DISP_WIDTH - 1, 23, "Tucson, Arizona", RIGHT);
  display.setFont(&FreeSans12pt7b);
  drawString(DISP_WIDTH - 1, 30 + 4 + 17, "Saturday, May 29", RIGHT);

  // 1 Alert
  // (fits on 1 line)
  /*
  display.drawInvertedBitmap(196, 8, wi_tornado_48x48, 48, 48, GxEPD_BLACK);
  display.setFont(&FreeSans16pt7b);
  drawString(196 + 48 + 4, 24 + 8 - 12 + 23, "Tornado Watch", LEFT);
  */
  // (fits on 1 line with smaller font)
  /*
  display.drawInvertedBitmap(196, 8, warning_icon_48x48, 48, 48, GxEPD_BLACK);
  display.setFont(&FreeSans12pt7b);
  drawString(196 + 48 + 4, 24 + 8 - 12 + 17, "Severe Thunderstorm Warning", LEFT);
  */
  // (needs 2 lines with smaller font)
  /*
  display.drawInvertedBitmap(196, 8, wi_tornado_48x48, 48, 48, GxEPD_BLACK);
  display.setFont(&FreeSans12pt7b);
  drawString(196 + 48 + 4, 24 + 8 - 22 - 4 - 2 + 17, "Crazy Severe Thunderstorm", LEFT);
  drawString(196 + 48 + 4, 24 + 8 - 2 + 17, " Warning", LEFT);
  */
  // 2 Alerts
  /*
  display.drawInvertedBitmap(196, 0, wi_tornado_32x32, 32, 32, GxEPD_BLACK);
  display.drawInvertedBitmap(196, 32, warning_icon_32x32, 32, 32, GxEPD_BLACK);
  display.setFont(&FreeSans12pt7b);
  drawString(196 + 32 + 3, 5 + 17, "Severe Thunderstorm Warning", LEFT);
  drawString(196 + 32 + 3, 32 + 5 + 17, "Hurricane Force Wind Warning", LEFT);
  */

  // current weather icon
  display.drawInvertedBitmap(0, 0, wi_day_rain_wind_196x196, 196, 196, GxEPD_BLACK);
  // current temp
  display.setFont(&FreeSans48pt_temperature);
  drawString(196 + 164 / 2 - 20, 196 / 2 + 69 / 2, "88", CENTER);
  display.setFont(&FreeSans14pt7b);
  drawString(display.getCursorX(), 196 / 2 - 69 / 2 + 20, "`F", LEFT);
  // current feels like
  display.setFont(&FreeSans12pt7b);
  drawString(196 + 164 / 2, 98 + 69 / 2 + 12 + 17, "Feels Like 86`", CENTER);

  // OLD IDEA TO DISPLAY HIGH AND LOW WITH CURRENT TEMP
  // decided against this to because it is redundent with temperature graph
  // and removing this from above the current temperture helps reduce clutter
  // current weather (with alert)
  // current weather icon
  /*
  display.drawInvertedBitmap(0, 0, wi_day_rain_wind_196x196, 196, 196, GxEPD_BLACK);
  // current temp
  display.setFont(&FreeSans48pt_temperature);
  drawString(196 + 162 / 2 - 20, 98 - 69 / 2 + 30, "199", CENTER);
  display.setFont(&FreeSans14pt7b);
  drawString(display.getCursorX(), 98 - 69 / 2 + 30, "`F", LEFT);
  // today's high | low
  display.setFont(&FreeSans14pt7b);
  drawString(196 + 162 / 2     , 98 - 69 / 2 - 6, "|", CENTER);
  drawString(196 + 162 / 2 - 10, 98 - 69 / 2 + 4, "199`", RIGHT);
  drawString(196 + 162 / 2 + 12, 98 - 69 / 2 + 4, "76`", LEFT);
  // current feels like
  display.setFont(&FreeSans12pt7b);
  drawString(196 + 162 / 2 + 4, 98 + 69 / 2 + 40, "Like 86`", CENTER);
  */

  // 5 day, forecast icons
  display.drawInvertedBitmap(398, 98 + 69 / 2 - 32 - 6, wi_day_fog_64x64, 64, 64, GxEPD_BLACK);
  display.drawInvertedBitmap(480, 98 + 69 / 2 - 32 - 6, wi_day_rain_wind_64x64, 64, 64, GxEPD_BLACK);
  display.drawInvertedBitmap(562, 98 + 69 / 2 - 32 - 6, wi_snow_64x64, 64, 64, GxEPD_BLACK);
  display.drawInvertedBitmap(644, 98 + 69 / 2 - 32 - 6, wi_thunderstorm_64x64, 64, 64, GxEPD_BLACK);
  display.drawInvertedBitmap(726, 98 + 69 / 2 - 32 - 6, wi_windy_64x64, 64, 64, GxEPD_BLACK);
  // 5 day, day of week label
  display.setFont(&FreeSans11pt7b);
  drawString(398 + 32, 98 + 69 / 2 - 32 - 26 - 6 + 16, "Sun", CENTER);
  drawString(480 + 32, 98 + 69 / 2 - 32 - 26 - 6 + 16, "Mon", CENTER);
  drawString(562 + 32, 98 + 69 / 2 - 32 - 26 - 6 + 16, "Tue", CENTER);
  drawString(644 + 32, 98 + 69 / 2 - 32 - 26 - 6 + 16, "Wed", CENTER);
  drawString(726 + 32, 98 + 69 / 2 - 32 - 26 - 6 + 16, "Thur", CENTER);
  // 5 day, high | low
  display.setFont(&FreeSans8pt7b);
  drawString(398 + 32, 98 + 69 / 2 + 38 - 4 + 12 - 2, "|", CENTER);
  drawString(398 + 32 - 4, 98 + 69 / 2 + 38 - 6 + 12, "199`", RIGHT);
  drawString(398 + 32 + 5, 98 + 69 / 2 + 38 - 6 + 12, "198`", LEFT);
  drawString(480 + 32, 98 + 69 / 2 + 38 - 4 + 12 - 2, "|", CENTER);
  drawString(480 + 32 - 4, 98 + 69 / 2 + 38 - 6 + 12, "199`", RIGHT);
  drawString(480 + 32 + 5, 98 + 69 / 2 + 38 - 6 + 12, "-22`", LEFT);
  drawString(562 + 32, 98 + 69 / 2 + 38 - 4 + 12 - 2, "|", CENTER);
  drawString(562 + 32 - 4, 98 + 69 / 2 + 38 - 6 + 12, "99`", RIGHT);
  drawString(562 + 32 + 5, 98 + 69 / 2 + 38 - 6 + 12, "67`", LEFT);
  drawString(644 + 32, 98 + 69 / 2 + 38 - 4 + 12 - 2, "|", CENTER);
  drawString(644 + 32 - 4, 98 + 69 / 2 + 38 - 6 + 12, "0`", RIGHT);
  drawString(644 + 32 + 5, 98 + 69 / 2 + 38 - 6 + 12, "0`", LEFT);
  drawString(726 + 32, 98 + 69 / 2 + 38 - 4 + 12 - 2, "|", CENTER);
  drawString(726 + 32 - 4, 98 + 69 / 2 + 38 - 6 + 12, "79`", RIGHT);
  drawString(726 + 32 + 5, 98 + 69 / 2 + 38 - 6 + 12, "199`", LEFT);

  // line dividing top and bottom display areas
  display.drawLine(0, 196, DISP_WIDTH - 1, 196, GxEPD_BLACK);

  // current weather data
  int16_t sp = 8;
  display.drawInvertedBitmap(0, 204 + (48 + sp) * 0, wi_sunrise_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(0, 204 + (48 + sp) * 1, wi_strong_wind_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(0, 204 + (48 + sp) * 2, wi_day_sunny_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(0, 204 + (48 + sp) * 3, air_filter_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(0, 204 + (48 + sp) * 4, house_thermometer_48x48, 48, 48, GxEPD_BLACK);

  display.drawInvertedBitmap(170, 204 + (48 + sp) * 0, wi_sunset_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(170, 204 + (48 + sp) * 1, wi_humidity_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(170, 204 + (48 + sp) * 2, wi_barometer_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(170, 204 + (48 + sp) * 3, visibility_icon_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(170, 204 + (48 + sp) * 4, house_humidity_48x48, 48, 48, GxEPD_BLACK);

  display.setFont(&FreeSans7pt7b);
  drawString(48, 204 + 10 + (48 + sp) * 0, "Sunrise", LEFT);
  drawString(48, 204 + 10 + (48 + sp) * 1, "Wind", LEFT);
  drawString(48, 204 + 10 + (48 + sp) * 2, "UV Index", LEFT);
  drawString(48, 204 + 10 + (48 + sp) * 3, "Air Quality Index", LEFT);
  drawString(48, 204 + 10 + (48 + sp) * 4, "Temperature", LEFT);

  drawString(170 + 48, 204 + 10 + (48 + sp) * 0, "Sunset", LEFT);
  drawString(170 + 48, 204 + 10 + (48 + sp) * 1, "Humidity", LEFT);
  drawString(170 + 48, 204 + 10 + (48 + sp) * 2, "Pressure", LEFT);
  drawString(170 + 48, 204 + 10 + (48 + sp) * 3, "Visibiliy", LEFT);
  drawString(170 + 48, 204 + 10 + (48 + sp) * 4, "Humidity", LEFT);

  display.setFont(&FreeSans12pt7b);
  drawString(48, 204 + 17 + (48 + sp) * 0 - 17 / 2 + 48 / 2, "6:00", LEFT);
  drawString(48, 204 + 17 + (48 + sp) * 1 - 17 / 2 + 48 / 2, "18mph", LEFT);
  drawString(48, 204 + 17 + (48 + sp) * 2 - 17 / 2 + 48 / 2, "10 - High", LEFT);
  drawString(48, 204 + 17 + (48 + sp) * 3 - 17 / 2 + 48 / 2, "Good", LEFT);
  drawString(48, 204 + 17 + (48 + sp) * 4 - 17 / 2 + 48 / 2, "78`", LEFT);

  drawString(170 + 48, 204 + 17 + (48 + sp) * 0 - 17 / 2 + 48 / 2, "18:00", LEFT);
  drawString(170 + 48, 204 + 17 + (48 + sp) * 1 - 17 / 2 + 48 / 2, "12%", LEFT);
  drawString(170 + 48, 204 + 17 + (48 + sp) * 2 - 17 / 2 + 48 / 2, "29.65in", LEFT);
  drawString(170 + 48, 204 + 17 + (48 + sp) * 3 - 17 / 2 + 48 / 2, "4000ft", LEFT);
  drawString(170 + 48, 204 + 17 + (48 + sp) * 4 - 17 / 2 + 48 / 2, "20%", LEFT);

  // debug
  int16_t x1, y1;
  uint16_t w, h;
  char str[20];

  // Total font height
  display.setFont(&FreeSans6pt7b);
  display.getTextBounds("TpXygQq", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "6 h: %d", h);
  drawString(500, 215, str, LEFT);

  display.setFont(&FreeSans7pt7b);
  display.getTextBounds("TpXygQq", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "7 h: %d", h);
  drawString(500, 230, str, LEFT);

  display.setFont(&FreeSans8pt7b);
  display.getTextBounds("TpXygQq", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "8 h: %d", h);
  drawString(500, 245, str, LEFT);

  display.setFont(&FreeSans9pt7b);
  display.getTextBounds("TpXygQq", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "9 h: %d", h);
  drawString(500, 260, str, LEFT);

  display.setFont(&FreeSans10pt7b);
  display.getTextBounds("TpXygQq", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "10 h: %d", h);
  drawString(500, 275, str, LEFT);

  display.setFont(&FreeSans11pt7b);
  display.getTextBounds("TpXygQq", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "11 h: %d", h);
  drawString(500, 290, str, LEFT);

  display.setFont(&FreeSans12pt7b);
  display.getTextBounds("TpXygQq", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "12 h: %d", h);
  drawString(500, 305, str, LEFT);

  display.setFont(&FreeSans14pt7b);
  display.getTextBounds("TpXygQq", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "14 h: %d", h);
  drawString(500, 320, str, LEFT);

  display.setFont(&FreeSans16pt7b);
  display.getTextBounds("TpXygQq", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "16 h: %d", h);
  drawString(500, 335, str, LEFT);

  display.setFont(&FreeSans18pt7b);
  display.getTextBounds("TpXygQq", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "18 h: %d", h);
  drawString(500, 350, str, LEFT);

  display.setFont(&FreeSans20pt7b);
  display.getTextBounds("TpXygQq", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "20 h: %d", h);
  drawString(500, 365, str, LEFT);

  display.setFont(&FreeSans22pt7b);
  display.getTextBounds("TpXygQq", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "22 h: %d", h);
  drawString(500, 380, str, LEFT);

  display.setFont(&FreeSans24pt7b);
  display.getTextBounds("TpXygQq", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "24 h: %d", h);
  drawString(500, 395, str, LEFT);

  display.setFont(&FreeSans26pt7b);
  display.getTextBounds("TpXygQq", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "26 h: %d", h);
  drawString(500, 410, str, LEFT);

  display.setFont(&FreeSans48pt_temperature);
  display.getTextBounds("1234567890.-`", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "48temp h: %d", h);
  drawString(500, 425, str, LEFT);

  // Upper font height
  display.setFont(&FreeSans6pt7b);
  display.getTextBounds("TMNH", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "6 h: %d", h);
  drawString(580, 215, str, LEFT);

  display.setFont(&FreeSans7pt7b);
  display.getTextBounds("TMNH", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "7 h: %d", h);
  drawString(580, 230, str, LEFT);

  display.setFont(&FreeSans8pt7b);
  display.getTextBounds("TMNH", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "8 h: %d", h);
  drawString(580, 245, str, LEFT);

  display.setFont(&FreeSans9pt7b);
  display.getTextBounds("TMNH", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "9 h: %d", h);
  drawString(580, 260, str, LEFT);

  display.setFont(&FreeSans10pt7b);
  display.getTextBounds("TMNH", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "10 h: %d", h);
  drawString(580, 275, str, LEFT);

  display.setFont(&FreeSans11pt7b);
  display.getTextBounds("TMNH", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "11 h: %d", h);
  drawString(580, 290, str, LEFT);

  display.setFont(&FreeSans12pt7b);
  display.getTextBounds("TMNH", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "12 h: %d", h);
  drawString(580, 305, str, LEFT);

  display.setFont(&FreeSans14pt7b);
  display.getTextBounds("TMNH", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "14 h: %d", h);
  drawString(580, 320, str, LEFT);

  display.setFont(&FreeSans16pt7b);
  display.getTextBounds("TMNH", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "16 h: %d", h);
  drawString(580, 335, str, LEFT);

  display.setFont(&FreeSans18pt7b);
  display.getTextBounds("TMNH", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "18 h: %d", h);
  drawString(580, 350, str, LEFT);

  display.setFont(&FreeSans20pt7b);
  display.getTextBounds("TMNH", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "20 h: %d", h);
  drawString(580, 365, str, LEFT);

  display.setFont(&FreeSans22pt7b);
  display.getTextBounds("TMNH", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "22 h: %d", h);
  drawString(580, 380, str, LEFT);

  display.setFont(&FreeSans24pt7b);
  display.getTextBounds("TMNH", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "24 h: %d", h);
  drawString(580, 395, str, LEFT);

  display.setFont(&FreeSans26pt7b);
  display.getTextBounds("TMNH", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "26 h: %d", h);
  drawString(580, 410, str, LEFT);

  display.setFont(&FreeSans48pt_temperature);
  display.getTextBounds("1234567890.-`", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "48temp h: %d", h);
  drawString(580, 425, str, LEFT);

  display.setFont(&FreeSans14pt7b);
  display.getTextBounds("`F`", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans8pt7b);
  sprintf(str, "w: %d h: %d", w, h);
  drawString(500, 440, str, LEFT);
  // end debug
}

/* Perform an HTTP GET request to OpenWeatherMap's "One Call" API
 * If data is recieved, it will be parsed and stored in the global variable
 * owm_onecall.
 *
 * Returns true if OK response is recieved and response is successfully parsed,
 * otherwise false.
 */
bool getOWMonecall(WiFiClient &client)
{
  int attempts = 0;
  bool rxSuccess = false;
  String unitsStr = (config::UNITS == 'i') ? "imperial" : "metric";
  String uri = "/data/2.5/onecall?lat=" + config::LAT + "&lon=" + config::LON 
               + "&units=" + unitsStr + "&lang=" + config::LANG 
               + "&exclude=minutely&appid=" + config::OWM_APIKEY;

  while (!rxSuccess && attempts < 2)
  {
    HTTPClient http;
    http.begin(client, config::OWM_ENDPOINT, 80, uri);
    int httpResponse = http.GET();
    if (httpResponse == HTTP_CODE_OK)
    {
      rxSuccess = deserializeOneCall(http.getStream(), &owm_onecall);
    }
    else
    {
      Serial.println("OpenWeatherMap One Call API connection error: " 
        + String(httpResponse, DEC) + " " + http.errorToString(httpResponse));
    }
    client.stop();
    http.end();
    ++attempts;
  }

  return rxSuccess;
}

/* Perform an HTTP GET request to OpenWeatherMap's "Air Pollution" API
 * If data is recieved, it will be parsed and stored in the global variable
 * owm_air_pollution.
 *
 * Returns true if OK response is recieved and response is successfully parsed,
 * otherwise false.
 */
bool getOWMairpollution(WiFiClient &client)
{
  int attempts = 0;
  bool rxSuccess = false;
  String unitsStr = (config::UNITS == 'i') ? "imperial" : "metric";

  // set start and end to approriate values so that the last 24 hours of air
  // pollution history is returned. Unix, UTC. Us
  time_t now;
  int64_t end = time(&now);
  int64_t start = end - (3600 * 23);
  char endStr[22];
  char startStr[22];
  sprintf(endStr, "%lld", end);
  sprintf(startStr, "%lld", start);

  String uri = "/data/2.5/air_pollution/history?lat=" 
               + config::LAT + "&lon=" + config::LON 
              + "&start=" + startStr + "&end=" + endStr 
              + "&appid=" + config::OWM_APIKEY;

  while (!rxSuccess && attempts < 2)
  {
    HTTPClient http;
    http.begin(client, config::OWM_ENDPOINT, 80, uri);
    int httpResponse = http.GET();
    if (httpResponse == HTTP_CODE_OK)
    {
      rxSuccess = deserializeAirQuality(http.getStream(), &owm_air_pollution);
    }
    else
    {
      Serial.println("OpenWeatherMap Air Pollution API connection error: " 
        + String(httpResponse, DEC) + " " + http.errorToString(httpResponse));
    }
    client.stop();
    http.end();
    ++attempts;
  }

  return rxSuccess;
}

void setup()
{
  startTime = millis();
  Serial.begin(115200);

  // TODO will firebeetle led stay off when on battery? otheriwse desolder...
  //#ifdef LED_BUILTIN
  //  pinMode(LED_BUILTIN, INPUT); // If it's On, turn it off and some boards use GPIO-5 for SPI-SS, which remains low after screen use
  //  digitalWrite(LED_BUILTIN, HIGH);
  //#endif

  wl_status_t wifiStatus = startWiFi();

  bool timeConfigured = false;
  if (wifiStatus == WL_CONNECTED)
  {
    timeConfigured = setupTime();
  }

  bool rxOWM = false;
  if ((wifiStatus == WL_CONNECTED) && timeConfigured)
  {
    WiFiClient client;
    rxOWM |= getOWMonecall(client);
    rxOWM |= getOWMairpollution(client);
  }
  killWiFi();
  initDisplay();

  if (rxOWM)
  {
    updateDisplayBuffer();
    display.display(false); // Full display refresh
  }
  else
  {
    // partialRefreshStatus(wifiStatus, timeConfigured, rxOWM);
    display.display(true); // partial display refresh
  }

  beginSleep();
}

void loop()
{
  // this will never run
}
