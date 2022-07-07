// built-in C++ libraries
// ...

// additional libraries
//#include <Adafruit_BusIO_Register.h>
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
#include "config.h"
#include "display_utils.h"
#include "renderer.h"

// language
#include LANGUAGE_HEADER

// B/W display
GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT> display(
  GxEPD2_750_T7(PIN_EPD_CS,
                PIN_EPD_DC,
                PIN_EPD_RST,
                PIN_EPD_BUSY));
// 3-colour displays
// GxEPD2_3C<GxEPD2_750c, GxEPD2_750c::HEIGHT> display(
//   GxEPD2_750(PIN_EPD_CS,
//              PIN_EPD_DC,
//              PIN_EPD_RST,
//              PIN_EPD_BUSY));

extern owm_resp_onecall_t owm_onecall;
extern owm_resp_air_pollution_t owm_air_pollution;

void initDisplay()
{
  display.init(115200, true, 2, false);
  // display.init(); for older Waveshare HAT's
  SPI.end();
  SPI.begin(PIN_EPD_SCK,
            PIN_EPD_MISO,
            PIN_EPD_MOSI,
            PIN_EPD_CS);

  display.setRotation(0);
  display.setTextSize(1);
  display.setTextColor(GxEPD_BLACK);
  display.fillScreen(GxEPD_WHITE);
  display.setFullWindow();
}

void drawString(int x, int y, String text, alignment_t alignment)
{
  int16_t x1, y1;
  uint16_t w, h;
  display.setTextWrap(false);
  display.getTextBounds(text, x, y, &x1, &y1, &w, &h);
  if (alignment == RIGHT)
    x = x - w;
  if (alignment == CENTER)
    x = x - w / 2;
  display.setCursor(x, y);
  display.print(text);
}

void debugDisplayBuffer(owm_resp_onecall_t       &owm_onecall,
                        owm_resp_air_pollution_t &owm_air_pollution)
{


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

  // if vector size...
  //display.drawInvertedBitmap(400, 0, getAlertBitmap48(owm_onecall.alerts[0]), 48, 48, GxEPD_BLACK);
  //display.drawInvertedBitmap(400, 0, getAlertBitmap32(owm_onecall.alerts[1]), 32, 32, GxEPD_BLACK);

  display.drawInvertedBitmap(400, 400, getForecastBitmap64(owm_onecall.daily[1]), 64, 64, GxEPD_BLACK);

  // end debug
}


void drawCurrentConditions(owm_current_t &current, 
                           owm_resp_air_pollution_t &owm_air_pollution, 
                           float inTemp, float inHumidity)
{
  String str;
  // current weather icon
  display.drawInvertedBitmap(0, 0, getCurrentConditionsBitmap196(current), 196, 196, GxEPD_BLACK);

  // current temp
  display.setFont(&FreeSans48pt_temperature);
  str = String(round(current.temp), 0);
  drawString(196 + 164 / 2 - 20, 196 / 2 + 69 / 2, str, CENTER);
  display.setFont(&FreeSans14pt7b);
  char tempUnits[3] = {'`', (UNITS == 'm') ? 'C' : 'F', '\0'};
  drawString(display.getCursorX(), 196 / 2 - 69 / 2 + 20, tempUnits, LEFT);

  // current feels like
  display.setFont(&FreeSans12pt7b);
  str = String(TXT_FEELS_LIKE) + ' ' + String(round(current.feels_like), 0) + '`';
  drawString(196 + 164 / 2, 98 + 69 / 2 + 12 + 17, str, CENTER);

  // line dividing top and bottom display areas
  display.drawLine(0, 196, DISP_WIDTH - 1, 196, GxEPD_BLACK);

  // current weather data
  display.drawInvertedBitmap(0, 204 + (48 + 8) * 0, wi_sunrise_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(0, 204 + (48 + 8) * 1, wi_strong_wind_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(0, 204 + (48 + 8) * 2, wi_day_sunny_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(0, 204 + (48 + 8) * 3, air_filter_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(0, 204 + (48 + 8) * 4, house_thermometer_48x48, 48, 48, GxEPD_BLACK);

  display.drawInvertedBitmap(170, 204 + (48 + 8) * 0, wi_sunset_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(170, 204 + (48 + 8) * 1, wi_humidity_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(170, 204 + (48 + 8) * 2, wi_barometer_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(170, 204 + (48 + 8) * 3, visibility_icon_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(170, 204 + (48 + 8) * 4, house_humidity_48x48, 48, 48, GxEPD_BLACK);

  display.setFont(&FreeSans7pt7b);
  drawString(48, 204 + 10 + (48 + 8) * 0, TXT_SUNRISE, LEFT);
  drawString(48, 204 + 10 + (48 + 8) * 1, TXT_WIND, LEFT);
  drawString(48, 204 + 10 + (48 + 8) * 2, TXT_UV_INDEX, LEFT);
  drawString(48, 204 + 10 + (48 + 8) * 3, TXT_AIR_QUALITY_INDEX, LEFT);
  drawString(48, 204 + 10 + (48 + 8) * 4, TXT_INDOOR_TEMPERATURE, LEFT);

  drawString(170 + 48, 204 + 10 + (48 + 8) * 0, TXT_SUNSET, LEFT);
  drawString(170 + 48, 204 + 10 + (48 + 8) * 1, TXT_HUMIDITY, LEFT);
  drawString(170 + 48, 204 + 10 + (48 + 8) * 2, TXT_PRESSURE, LEFT);
  drawString(170 + 48, 204 + 10 + (48 + 8) * 3, TXT_VISIBILITY, LEFT);
  drawString(170 + 48, 204 + 10 + (48 + 8) * 4, TXT_INDOOR_HUMIDITY, LEFT);

  display.setFont(&FreeSans12pt7b);
  drawString(48, 204 + 17 + (48 + 8) * 0 - 17 / 2 + 48 / 2, "6:00", LEFT);
  drawString(48, 204 + 17 + (48 + 8) * 1 - 17 / 2 + 48 / 2, "18mph", LEFT);
  drawString(48, 204 + 17 + (48 + 8) * 2 - 17 / 2 + 48 / 2, "10 - High", LEFT);
  drawString(48, 204 + 17 + (48 + 8) * 3 - 17 / 2 + 48 / 2, "Good", LEFT);
  drawString(48, 204 + 17 + (48 + 8) * 4 - 17 / 2 + 48 / 2, "78`", LEFT);

  drawString(170 + 48, 204 + 17 + (48 + 8) * 0 - 17 / 2 + 48 / 2, "18:00", LEFT);
  drawString(170 + 48, 204 + 17 + (48 + 8) * 1 - 17 / 2 + 48 / 2, "12%", LEFT);
  drawString(170 + 48, 204 + 17 + (48 + 8) * 2 - 17 / 2 + 48 / 2, "29.65in", LEFT);
  drawString(170 + 48, 204 + 17 + (48 + 8) * 3 - 17 / 2 + 48 / 2, "4000ft", LEFT);
  drawString(170 + 48, 204 + 17 + (48 + 8) * 4 - 17 / 2 + 48 / 2, "20%", LEFT);

  return;
}
void drawForecast(owm_daily_t *const daily)
{
  return;
}
void drawAlerts(std::vector<owm_alerts_t> &alerts)
{
  return;
}

void drawLocationDate(const String &city, tm *timeInfo)
{
  char dateBuffer[48] = {};
  snprintf(dateBuffer, sizeof(dateBuffer), "%s, %s %d",
           TXT_dddd[timeInfo->tm_wday],
           TXT_MMMM[timeInfo->tm_mon],
           timeInfo->tm_mday);
  // alternatively,
  // strftime(dateBuffer, sizeof(dateBuffer), "%A, %B %d", timeInfo);

  // location, date
  display.setFont(&FreeSans16pt7b);
  drawString(DISP_WIDTH - 1, 23, city, RIGHT);
  display.setFont(&FreeSans12pt7b);
  drawString(DISP_WIDTH - 1, 30 + 4 + 17, dateBuffer, RIGHT);
  return;
}

void drawOutlookGraph(owm_hourly_t *const hourly)
{
  return;
}
void drawStatusBar(char *const statusStr, int wifiRSSI, double batteryVoltage)
{
  return;
}