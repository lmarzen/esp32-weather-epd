#include <GxEPD2_BW.h>

// fonts (modified font files that have the degree symbol mapped to '`')
#include "fonts/FreeSans4pt7b.h"
#include "fonts/FreeSans5pt7b.h"
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

/* Returns the string width in pixels
 */
uint16_t getStringWidth(String text)
{
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  return w;
}

/* Returns the string height in pixels
 */
uint16_t getStringHeight(String text)
{
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  return h;
}

/* Draws a string with alignment
 */
void drawString(int16_t x, int16_t y, String text, alignment_t alignment)
{
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, x, y, &x1, &y1, &w, &h);
  if (alignment == RIGHT)
    x = x - w;
  if (alignment == CENTER)
    x = x - w / 2;
  display.setCursor(x, y);
  display.print(text);
} // end drawString

/* Draws a string that will flow into the next line when max_width is reached.
 * If a string exceeds max_lines an ellipsis (...) will terminate the last word.
 * Lines will break at spaces(' ') and dashes('-').
 * 
 * Note: max_width should be big enough to accommodate the largest word that
 *       will be displayed. If an unbroken string of characters longer than
 *       max_width exist in text, then the string will be printed beyond 
 *       max_width.
 */
void drawMultiLnString(int16_t x, int16_t y, String text, alignment_t alignment, 
                       uint16_t max_width, uint16_t max_lines, 
                       int16_t line_spacing)
{
  
  uint16_t current_line = 0;
  String textRemaining = text;
  // print until we reach max_lines or no more text remains
  while (current_line < max_lines && !textRemaining.isEmpty())
  {
    int16_t  x1, y1;
    uint16_t w, h;

    display.getTextBounds(textRemaining, 0, 0, &x1, &y1, &w, &h);
    
    int endIndex = textRemaining.length();
    // check if remaining text is to wide, if it is then print what we can
    String subStr = textRemaining;
    int splitAt = 0;
    int keepLastChar = 0;
    while (w > max_width && splitAt != -1)
    {
      if (keepLastChar)
      {
        // if we kept the last character during the last iteration of this while
        // loop, remove it now so we don't get stuck in an infinite loop.
        subStr.remove(subStr.length() - 1);
      }

      // find the last place in the string that we can break it.
      if (current_line < max_lines - 1)
      {
        splitAt = max(subStr.lastIndexOf(" "), 
                    subStr.lastIndexOf("-"));
      }
      else
      {
        // this is the last line, only break at spaces so we can add ellipsis
        splitAt = subStr.lastIndexOf(" ");
      }
      
      // if splitAt == -1 then there is an unbroken set of characters that is 
      // longer than max_width. Otherwise if splitAt != -1 then we can continue
      // the loop until the string is <= max_width
      if (splitAt != -1)
      {
        endIndex = splitAt;
        subStr = subStr.substring(0, endIndex + 1);

        char lastChar = subStr.charAt(endIndex);
        if (lastChar == ' ')
        {
          // remove this char now so it is not counted towards line width
          keepLastChar = 0;
          subStr.remove(endIndex);
          --endIndex;
        }
        else if (lastChar == '-')
        {
          // this char will be printed on this line and removed next iteration
          keepLastChar = 1;
        }

        if (current_line < max_lines - 1)
        {
          // this is not the last line
          display.getTextBounds(subStr, 0, 0, &x1, &y1, &w, &h);
        }
        else
        {
          // this is the last line, we need to make sure there is space for 
          // ellipsis
          display.getTextBounds(subStr + "...", 0, 0, &x1, &y1, &w, &h);
          if (w <= max_width)
          {
            // ellipsis fit, add them to subStr
            subStr = subStr + "...";
          }
        }

      } // end if (splitAt != -1)
    } // end inner while
    
    drawString(x, y + (current_line * line_spacing), subStr, alignment);

    // update textRemaining to no longer include what was printed
    // +1 for exclusive bounds, +1 to get passed space/dash 
    textRemaining = textRemaining.substring(endIndex + 2 - keepLastChar);

    ++current_line;
  } // end outer while

  return;
} // end drawMultiLnString

/* Initialize e-paper display
 */
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
  display.setTextWrap(false);
} // end initDisplay

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

  display.setFont(&FreeSans11pt7b);
  display.getTextBounds("u", 0, 0, &x1, &y1, &w, &h);
  display.setFont(&FreeSans12pt7b);
  sprintf(str, "w: %d h: %d", w, h);
  drawString(500, 440, str, LEFT);

  // if vector size...
  //display.drawInvertedBitmap(400, 0, getAlertBitmap48(owm_onecall.alerts[0]), 48, 48, GxEPD_BLACK);
  //display.drawInvertedBitmap(400, 0, getAlertBitmap32(owm_onecall.alerts[1]), 32, 32, GxEPD_BLACK);

  display.drawInvertedBitmap(400, 400,
                             getForecastBitmap64(owm_onecall.daily[1]),
                             64, 64, GxEPD_BLACK);

  // end debug
}

/* This function is responsible for drawing the current conditions and 
 * associated icons.
 */
void drawCurrentConditions(owm_current_t &current, owm_daily_t &today,
                           owm_resp_air_pollution_t &owm_air_pollution, 
                           float inTemp, float inHumidity)
{
  String dataStr, unitStr;
  // current weather icon
  display.drawInvertedBitmap(0, 0,
                             getCurrentConditionsBitmap196(current, today), 
                             196, 196, GxEPD_BLACK);

  // current temp
  display.setFont(&FreeSans48pt_temperature);
  dataStr = String(round(current.temp), 0);
  drawString(196 + 164 / 2 - 20, 196 / 2 + 69 / 2, dataStr, CENTER);
  display.setFont(&FreeSans14pt7b);
#ifdef UNITS_METRIC
  const char tempUnits[] = "`C";
#endif // end UNITS_METRIC
#ifdef UNITS_IMPERIAL
  const char tempUnits[] = "`F";
#endif // end UNITS_IMPERIAL
  drawString(display.getCursorX(), 196 / 2 - 69 / 2 + 20, tempUnits, LEFT);

  // current feels like
  display.setFont(&FreeSans12pt7b);
  dataStr = String(TXT_FEELS_LIKE) + ' ' 
            + String(round(current.feels_like), 0) + '`';
  drawString(196 + 164 / 2, 98 + 69 / 2 + 12 + 17, dataStr, CENTER);

  // line dividing top and bottom display areas
  display.drawLine(0, 196, DISP_WIDTH - 1, 196, GxEPD_BLACK);

  // current weather data icons
  display.drawInvertedBitmap(0, 204 + (48 + 8) * 0,
                             wi_sunrise_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(0, 204 + (48 + 8) * 1,
                             wi_strong_wind_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(0, 204 + (48 + 8) * 2,
                             wi_day_sunny_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(0, 204 + (48 + 8) * 3,
                             air_filter_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(0, 204 + (48 + 8) * 4,
                             house_thermometer_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(170, 204 + (48 + 8) * 0,
                             wi_sunset_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(170, 204 + (48 + 8) * 1,
                             wi_humidity_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(170, 204 + (48 + 8) * 2,
                             wi_barometer_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(170, 204 + (48 + 8) * 3,
                             visibility_icon_48x48, 48, 48, GxEPD_BLACK);
  display.drawInvertedBitmap(170, 204 + (48 + 8) * 4,
                             house_humidity_48x48, 48, 48, GxEPD_BLACK);

  // current weather data labels
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

  // sunrise
  display.setFont(&FreeSans12pt7b);
  char timeBuffer[12] = {}; // big enough to accommodate "hh:mm:ss am"
  time_t ts = current.sunrise;
  tm *timeInfo = localtime(&ts);
  strftime(timeBuffer, sizeof(timeBuffer), TIME_FORMAT, timeInfo);
  drawString(48, 204 + 17 / 2 + (48 + 8) * 0 + 48 / 2, timeBuffer, LEFT);

  // wind
  display.drawInvertedBitmap(48, 204 + 24 / 2 + (48 + 8) * 1, 
                             getWindBitmap24(current.wind_deg), 
                             24, 24, GxEPD_BLACK);
  dataStr =  String(round(current.wind_speed), 0);
  drawString(48 + 24, 204 + 17 / 2 + (48 + 8) * 1 + 48 / 2, dataStr, LEFT);
  #ifdef UNITS_METRIC
  unitStr = "m/s";
#endif // end UNITS_METRIC
#ifdef UNITS_IMPERIAL
  unitStr = "mph";
#endif // end UNITS_IMPERIAL
  display.setFont(&FreeSans10pt7b);
  drawString(display.getCursorX(), 204 + 17 / 2 + (48 + 8) * 1 + 48 / 2, 
             unitStr, LEFT);

  // uv index
  display.setFont(&FreeSans12pt7b);
  uint uvi = static_cast<uint>(max(round(current.uvi), 0.0));
  dataStr = String(uvi);
  drawString(48, 204 + 17 / 2 + (48 + 8) * 2 + 48 / 2, dataStr, LEFT);
  display.setFont(&FreeSans6pt7b);
  dataStr = String(getUVIdesc(uvi));
  drawString(display.getCursorX() + 6, 204 + 17 / 2 + (48 + 8) * 2 + 48 / 2, 
             dataStr, LEFT);

  // air quality index
  display.setFont(&FreeSans12pt7b);
  int aqi = getAQI(owm_air_pollution);
  aqi = 120;// debug
  dataStr = String(aqi);
  drawString(48, 204 + 17 / 2 + (48 + 8) * 3 + 48 / 2, dataStr, LEFT);
  display.setFont(&FreeSans6pt7b);
  dataStr = String(getAQIdesc(aqi));
  int x = display.getCursorX() + 6; // debug
  if (getStringWidth(dataStr) < 100)
  { // Fits on a single line, draw along bottom
    drawString(display.getCursorX() + 6, 204 + 17 / 2 + (48 + 8) * 3 + 48 / 2, 
               dataStr, LEFT);
  }
  else
  { // Does not fit on a single line, draw higher to allow room for second line
    drawMultiLnString(display.getCursorX() + 6,
                      204 + 17 / 2 + (48 + 8) * 3 + 48 / 2 - 12, 
                      dataStr, LEFT, 120, 2, 12);
  }
  display.drawLine(x + 120, 0, x + 120, DISP_HEIGHT - 1, GxEPD_BLACK);

  // indoor temperature
  display.setFont(&FreeSans12pt7b);
  if (!isnan(inTemp))
  {
#ifdef UNITS_METRIC
    dataStr = String(round(inTemp), 0) + "`";
#endif // end UNITS_METRIC
#ifdef UNITS_IMPERIAL
    // C to F
    dataStr = String(round((inTemp * 9.0 / 5.0) + 32 ), 0) + "`";
#endif // end UNITS_IMPERIAL
  }
  else
  {
    dataStr = "-`";
  }
  drawString(48, 204 + 17 / 2 + (48 + 8) * 4 + 48 / 2, dataStr, LEFT);

  // sunset
  memset(timeBuffer, '\0', sizeof(timeBuffer));
  ts = current.sunset;
  timeInfo = localtime(&ts);
  strftime(timeBuffer, sizeof(timeBuffer), TIME_FORMAT, timeInfo);
  drawString(170 + 48, 204 + 17 / 2 + (48 + 8) * 0 + 48 / 2, timeBuffer, LEFT);

  // humidity
  dataStr = String(current.humidity);
  drawString(170 + 48, 204 + 17 / 2 + (48 + 8) * 1 + 48 / 2, dataStr, LEFT);
  display.setFont(&FreeSans10pt7b);
  drawString(display.getCursorX(), 204 + 17 / 2 + (48 + 8) * 1 + 48 / 2, 
             "%", LEFT);

  // pressure
  display.setFont(&FreeSans12pt7b);
#ifdef UNITS_METRIC
  dataStr = String(current.pressure);
  unitStr = "hPa";
#endif // end UNITS_METRIC
#ifdef UNITS_IMPERIAL
  // hPa to inHg (rounded to 2 decimal places)
  dataStr = String(round(100.0 * current.pressure * 0.02953) / 100.0, 2);
  unitStr = "in";
#endif // end UNITS_IMPERIAL
  drawString(170 + 48, 204 + 17 / 2 + (48 + 8) * 2 + 48 / 2, dataStr, LEFT);
  display.setFont(&FreeSans10pt7b);
  drawString(display.getCursorX(), 204 + 17 / 2 + (48 + 8) * 2 + 48 / 2, 
             unitStr, LEFT);

  // visibility
  display.setFont(&FreeSans12pt7b);
#ifdef UNITS_METRIC
  float vis = current.visibility / 1000.0; // m to km
  unitStr = "km";
#endif // end UNITS_METRIC
#ifdef UNITS_IMPERIAL
  float vis = current.visibility * 0.000621371; // m to mi
  unitStr = "mi";
#endif // end UNITS_IMPERIAL
  // if visibility is less than 1.95, round to 1 decimal place
  // else round to int
  if (vis < 1.95)
  {
    dataStr = String(round(10 * vis) / 10.0, 1);
  }
  else
  {
    dataStr = String(round(vis), 0);
  }
#ifdef UNITS_METRIC
  if (vis >= 10) {
#endif // end UNITS_METRIC
#ifdef UNITS_IMPERIAL
  if (vis >= 6) {
#endif // end UNITS_IMPERIAL
    dataStr = ">" + dataStr;
  }
  drawString(170 + 48, 204 + 17 / 2 + (48 + 8) * 3 + 48 / 2, dataStr, LEFT);
  display.setFont(&FreeSans10pt7b);
  drawString(display.getCursorX(), 204 + 17 / 2 + (48 + 8) * 3 + 48 / 2, 
             unitStr, LEFT);

  // indoor humidity
  display.setFont(&FreeSans12pt7b);
  if (!isnan(inHumidity))
  {
    dataStr = String(round(inHumidity), 0);
  }
  else
  {
    dataStr = "-";
  }
  drawString(170 + 48, 204 + 17 / 2 + (48 + 8) * 4 + 48 / 2, dataStr, LEFT);
  display.setFont(&FreeSans10pt7b);
  drawString(display.getCursorX(), 204 + 17 / 2 + (48 + 8) * 4 + 48 / 2, 
             "%", LEFT);

  return;
} // end drawCurrentConditions

/* This function is responsible for drawing the five day forecast.
 */
void drawForecast(owm_daily_t *const daily, tm timeInfo)
{
  // 5 day, forecast
  for (int i = 0; i < 5; ++i)
  {
    int x = 398 + (i * 82);
    // icons
    display.drawInvertedBitmap(x, 98 + 69 / 2 - 32 - 6,
                               getForecastBitmap64(daily[i]),
                               64, 64, GxEPD_BLACK);
    // day of week label
    display.setFont(&FreeSans11pt7b);
    char dayBuffer[8] = {};
    strftime(dayBuffer, sizeof(dayBuffer), "%a", &timeInfo); // abbreviated day
    drawString(x + 31 - 2, 98 + 69 / 2 - 32 - 26 - 6 + 16, dayBuffer, CENTER);
    timeInfo.tm_wday = (timeInfo.tm_wday + 1) % 7; // increment to next day

    // high | low
    String tempStr;
    display.setFont(&FreeSans8pt7b);
    drawString(x + 31, 98 + 69 / 2 + 38 - 6 + 12, "|", CENTER);
    tempStr = String(round(daily[i].temp.max), 0) + "`";
    drawString(x + 31 - 4, 98 + 69 / 2 + 38 - 6 + 12, tempStr, RIGHT);
    tempStr = String(round(daily[i].temp.min), 0) + "`";
    drawString(x + 31 + 5, 98 + 69 / 2 + 38 - 6 + 12, tempStr, LEFT);
  }

  return;
} // end drawForecast

/* This function is responsible for drawing the current alerts if any.
 */
void drawAlerts(std::vector<owm_alerts_t> &alerts)
{
  return;
} // end drawAlerts

/* This function is responsible for drawing the city string and date 
 * information in the top right corner.
 */
void drawLocationDate(const String &city, tm *timeInfo)
{
  char dateBuffer[48] = {};
  strftime(dateBuffer, sizeof(dateBuffer), DATE_FORMAT, timeInfo);
  String dateStr = dateBuffer;
  // remove double spaces. %e will add an extra space, ie. " 1" instead of "1"
  dateStr.replace("  ", " ");
  // alternatively...
  // snprintf(dateBuffer, sizeof(dateBuffer), "%s, %s %d",
  //          TXT_dddd[timeInfo->tm_wday],
  //          TXT_MMMM[timeInfo->tm_mon],
  //          timeInfo->tm_mday);

  // location, date
  display.setFont(&FreeSans16pt7b);
  drawString(DISP_WIDTH - 2, 23, city, RIGHT);
  display.setFont(&FreeSans12pt7b);
  drawString(DISP_WIDTH - 2, 30 + 4 + 17, dateStr, RIGHT);
  return;
} // end drawLocationDate

/* This function is responsible for drawing the outlook graph for the specified
 * number of hours(up to 47).
 */
void drawOutlookGraph(owm_hourly_t *const hourly)
{
  return;
} // end drawOutlookGraph

/* This function is responsible for drawing the status bar along the bottom of
 * the display.
 */
void drawStatusBar(char *const statusStr, int wifiRSSI, double batteryVoltage)
{
  return;
} // end drawStatusBar