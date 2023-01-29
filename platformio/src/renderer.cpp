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
#include "icons/icons_24x24.h"
#include "icons/icons_32x32.h"
#include "icons/icons_48x48.h"
#include "icons/icons_64x64.h"
#include "icons/icons_96x96.h"
#include "icons/icons_128x128.h"
#include "icons/icons_160x160.h"
#include "icons/icons_196x196.h"

// header files
#include "api_response.h"
#include "c_strftime.h"
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
  SPI.begin(PIN_EPD_SCK,
            PIN_EPD_MISO,
            PIN_EPD_MOSI,
            PIN_EPD_CS);

  display.setRotation(0);
  display.setTextSize(1);
  display.setTextColor(GxEPD_BLACK);
  display.setTextWrap(false);
  display.fillScreen(GxEPD_WHITE);
  display.setFullWindow();
} // end initDisplay

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
  dataStr = String(static_cast<int>(round(current.temp)));
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
            + String(static_cast<int>(round(current.feels_like))) + '`';
  drawString(196 + 164 / 2, 98 + 69 / 2 + 12 + 17, dataStr, CENTER);

  // line dividing top and bottom display areas
  // display.drawLine(0, 196, DISP_WIDTH - 1, 196, GxEPD_BLACK);

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
  c_strftime(timeBuffer, sizeof(timeBuffer), TIME_FORMAT, timeInfo);
  drawString(48, 204 + 17 / 2 + (48 + 8) * 0 + 48 / 2, timeBuffer, LEFT);

  // wind
  display.drawInvertedBitmap(48, 204 + 24 / 2 + (48 + 8) * 1, 
                             getWindBitmap24(current.wind_deg), 
                             24, 24, GxEPD_BLACK);
  dataStr =  String(static_cast<int>(round(current.wind_speed)));
  drawString(48 + 24, 204 + 17 / 2 + (48 + 8) * 1 + 48 / 2, dataStr, LEFT);
  #ifdef UNITS_METRIC
  unitStr = "m/s";
#endif // end UNITS_METRIC
#ifdef UNITS_IMPERIAL
  unitStr = "mph";
#endif // end UNITS_IMPERIAL
  display.setFont(&FreeSans8pt7b);
  drawString(display.getCursorX(), 204 + 17 / 2 + (48 + 8) * 1 + 48 / 2, 
             unitStr, LEFT);

  // uv and air quality indices
  // spacing between end of index value and start of descriptor text
  const int sp = 8;

  // uv index
  display.setFont(&FreeSans12pt7b);
  uint uvi = static_cast<uint>(max(round(current.uvi), 0.0f));
  dataStr = String(uvi);
  drawString(48, 204 + 17 / 2 + (48 + 8) * 2 + 48 / 2, dataStr, LEFT);
  display.setFont(&FreeSans7pt7b);
  dataStr = String(getUVIdesc(uvi));
  int max_w = 170 - (display.getCursorX() + sp);
  if (getStringWidth(dataStr) <= max_w)
  { // Fits on a single line, draw along bottom
    drawString(display.getCursorX() + sp, 204 + 17 / 2 + (48 + 8) * 2 + 48 / 2, 
               dataStr, LEFT);
  }
  else
  { // use smaller font
    display.setFont(&FreeSans5pt7b);
    if (getStringWidth(dataStr) <= max_w)
    { // Fits on a single line with smaller font, draw along bottom
      drawString(display.getCursorX() + sp, 
                 204 + 17 / 2 + (48 + 8) * 2 + 48 / 2, 
                 dataStr, LEFT);
    }
    else
    { // Does not fit on a single line, draw higher to allow room for 2nd line
      drawMultiLnString(display.getCursorX() + sp,
                        204 + 17 / 2 + (48 + 8) * 2 + 48 / 2 - 10, 
                        dataStr, LEFT, max_w, 2, 10);
    }
  }

  // air quality index
  display.setFont(&FreeSans12pt7b);
  int aqi = getAQI(owm_air_pollution);
  dataStr = String(aqi);
  drawString(48, 204 + 17 / 2 + (48 + 8) * 3 + 48 / 2, dataStr, LEFT);
  display.setFont(&FreeSans7pt7b);
  dataStr = String(getAQIdesc(aqi));
  max_w = 170 - (display.getCursorX() + sp);
  if (getStringWidth(dataStr) <= max_w)
  { // Fits on a single line, draw along bottom
    drawString(display.getCursorX() + sp, 204 + 17 / 2 + (48 + 8) * 3 + 48 / 2, 
               dataStr, LEFT);
  }
  else
  { // use smaller font
    display.setFont(&FreeSans5pt7b);
    if (getStringWidth(dataStr) <= max_w)
    { // Fits on a single line with smaller font, draw along bottom
      drawString(display.getCursorX() + sp, 
                 204 + 17 / 2 + (48 + 8) * 3 + 48 / 2, 
                 dataStr, LEFT);
    }
    else
    { // Does not fit on a single line, draw higher to allow room for 2nd line
      drawMultiLnString(display.getCursorX() + sp,
                        204 + 17 / 2 + (48 + 8) * 3 + 48 / 2 - 10, 
                        dataStr, LEFT, max_w, 2, 10);
    }
  }

  // indoor temperature
  display.setFont(&FreeSans12pt7b);
  if (!isnan(inTemp))
  {
#ifdef UNITS_METRIC
    dataStr = String(static_cast<int>(round(inTemp))) + "`";
#endif // end UNITS_METRIC
#ifdef UNITS_IMPERIAL
    // C to F
    dataStr = String(static_cast<int>(round((inTemp * 9.0 / 5.0) + 32 ))) + "`";
#endif // end UNITS_IMPERIAL
  }
  else
  {
    dataStr = "--`";
  }
  drawString(48, 204 + 17 / 2 + (48 + 8) * 4 + 48 / 2, dataStr, LEFT);

  // sunset
  memset(timeBuffer, '\0', sizeof(timeBuffer));
  ts = current.sunset;
  timeInfo = localtime(&ts);
  c_strftime(timeBuffer, sizeof(timeBuffer), TIME_FORMAT, timeInfo);
  drawString(170 + 48, 204 + 17 / 2 + (48 + 8) * 0 + 48 / 2, timeBuffer, LEFT);

  // humidity
  dataStr = String(current.humidity);
  drawString(170 + 48, 204 + 17 / 2 + (48 + 8) * 1 + 48 / 2, dataStr, LEFT);
  display.setFont(&FreeSans8pt7b);
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
  display.setFont(&FreeSans8pt7b);
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
    dataStr = String(static_cast<int>(round(vis)));
  }
#ifdef UNITS_METRIC
  if (vis >= 10) {
#endif // end UNITS_METRIC
#ifdef UNITS_IMPERIAL
  if (vis >= 6) {
#endif // end UNITS_IMPERIAL
    dataStr = "> " + dataStr;
  }
  drawString(170 + 48, 204 + 17 / 2 + (48 + 8) * 3 + 48 / 2, dataStr, LEFT);
  display.setFont(&FreeSans8pt7b);
  drawString(display.getCursorX(), 204 + 17 / 2 + (48 + 8) * 3 + 48 / 2, 
             unitStr, LEFT);

  // indoor humidity
  display.setFont(&FreeSans12pt7b);
  if (!isnan(inHumidity))
  {
    dataStr = String(static_cast<int>(round(inHumidity)));
  }
  else
  {
    dataStr = "--";
  }
  drawString(170 + 48, 204 + 17 / 2 + (48 + 8) * 4 + 48 / 2, dataStr, LEFT);
  display.setFont(&FreeSans8pt7b);
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
    c_strftime(dayBuffer, sizeof(dayBuffer), "%a", &timeInfo); // abbrv'd day
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
 * Up to 2 alerts can be drawn.
 */
void drawAlerts(std::vector<owm_alerts_t> &alerts, 
                const String &city, const String &date)
{
  if (alerts.size() == 0)
  { // no alerts to draw
    return;
  }

  // Converts all event text and tags to lowercase, removes extra information,
  // and filters out redundant alerts of lesser urgency.
  int ignore_list[alerts.size()] = {};
  filterAlerts(alerts, ignore_list);

  // limit alert text width so that is does not run into the location or date
  // strings
  display.setFont(&FreeSans16pt7b);
  int city_w = getStringWidth(city);
  display.setFont(&FreeSans12pt7b);
  int date_w = getStringWidth(date);
  int max_w = DISP_WIDTH - 2 - max(city_w, date_w) - (196 + 4) - 8;

  // find indicies of valid alerts
  int alert_indices[alerts.size()] = {};
  int num_valid_alerts = 0;
  for (int i = 0; i < alerts.size(); ++i)
  {
    if (!ignore_list[i])
    {
      alert_indices[num_valid_alerts] = i;
      ++num_valid_alerts;
    }
  }

  if (num_valid_alerts == 1)
  { // 1 alert
    // adjust max width to for 48x48 icons
    max_w -= 48;

    owm_alerts_t &cur_alert = alerts[alert_indices[0]];
    display.drawInvertedBitmap(196, 8, getAlertBitmap48(cur_alert), 48, 48, 
                               GxEPD_BLACK);
    // must be called after getAlertBitmap
    toTitleCase(cur_alert.event);

    display.setFont(&FreeSans14pt7b);
    if (getStringWidth(cur_alert.event) <= max_w)
    { // Fits on a single line, draw along bottom
      drawString(196 + 48 + 4, 24 + 8 - 12 + 20 + 1, cur_alert.event, LEFT);
    }
    else if (num_valid_alerts == 2)
    { // use smaller font
      display.setFont(&FreeSans12pt7b);
      if (getStringWidth(cur_alert.event) <= max_w)
      { // Fits on a single line with smaller font, draw along bottom
        drawString(196 + 48 + 4, 24 + 8 - 12 + 17 + 1, cur_alert.event, LEFT);
      }
      else
      { // Does not fit on a single line, draw higher to allow room for 2nd line
        drawMultiLnString(196 + 48 + 4, 24 + 8 - 12 + 17 - 11, 
                          cur_alert.event, LEFT, max_w, 2, 23);
      }
    }
  } // end 1 alert
  else
  { // 2 alerts
    // adjust max width to for 32x32 icons
    max_w -= 32;

    display.setFont(&FreeSans12pt7b);
    for (int i = 0; i < 2; ++i)
    {
      owm_alerts_t &cur_alert = alerts[alert_indices[i]];

      display.drawInvertedBitmap(196, (i * 32), getAlertBitmap32(cur_alert), 
                                 32, 32, GxEPD_BLACK);
      // must be called after getAlertBitmap
      toTitleCase(cur_alert.event);
      
      drawMultiLnString(196 + 32 + 3, 5 + 17 + (i * 32), 
                        cur_alert.event, LEFT, max_w, 1, 0);
    } // end for-loop
  } // end 2 alerts
  
  return;
} // end drawAlerts

/* This function is responsible for drawing the city string and date 
 * information in the top right corner.
 */
void drawLocationDate(const String &city, const String &date)
{
  // location, date
  display.setFont(&FreeSans16pt7b);
  drawString(DISP_WIDTH - 2, 23, city, RIGHT);
  display.setFont(&FreeSans12pt7b);
  drawString(DISP_WIDTH - 2, 30 + 4 + 17, date, RIGHT);
  return;
} // end drawLocationDate

/* This function is responsible for drawing the outlook graph for the specified
 * number of hours(up to 47).
 */
void drawOutlookGraph(owm_hourly_t *const hourly, tm timeInfo)
{

  const int xPos0 = 350;
  const int xPos1 = DISP_WIDTH - 46;
  const int yPos0 = 216;
  const int yPos1 = DISP_HEIGHT - 46;

  // x axis
  display.drawLine(xPos0, yPos1    , xPos1, yPos1    , GxEPD_BLACK);
  display.drawLine(xPos0, yPos1 - 1, xPos1, yPos1 - 1, GxEPD_BLACK);

  // calculate y max/min and intervals
  int yMajorTicks = 5;

  int yTempMajorTicks = 5;
  float tempMin = hourly[0].temp;
  float tempMax = hourly[0].temp;
  for (int i = 1; i < HOURLY_GRAPH_MAX; ++i)
  {
    tempMin = min(tempMin, hourly[i].temp);
    tempMax = max(tempMax, hourly[i].temp);
  }
  int tempBoundMin = static_cast<int>(tempMin - 1) 
                     - (static_cast<int>(tempMin - 1) % yTempMajorTicks);
  int tempBoundMax = static_cast<int>(tempMax + 1) 
        + (yTempMajorTicks - (static_cast<int>(tempMax + 1) % yTempMajorTicks));
  // while we have to many major ticks then increase the step
  while ((tempBoundMax - tempBoundMin) / yTempMajorTicks > yMajorTicks)
  {
    yTempMajorTicks += 5;
    tempBoundMin = static_cast<int>(tempMin - 1) 
                   - (static_cast<int>(tempMin - 1) % yTempMajorTicks);
    tempBoundMax = static_cast<int>(tempMax + 1) 
        + (yTempMajorTicks - (static_cast<int>(tempMax + 1) % yTempMajorTicks));
  }
  // while we have not enough major ticks add to either bound
  while ((tempBoundMax - tempBoundMin) / yTempMajorTicks < yMajorTicks)
  {
    // add to whatever bound is closer to the actual min/max
    if (tempMin - tempBoundMin <= tempBoundMax - tempMax)
    {
      tempBoundMin -= yTempMajorTicks;
    }
    else
    {
      tempBoundMax += yTempMajorTicks;
    }
  }

  // draw y axis
  float yInterval = (yPos1 - yPos0) / static_cast<float>(yMajorTicks);
  for (int i = 0; i <= yMajorTicks; ++i)
  {
    String dataStr;
    int yTick = static_cast<int>(yPos0 + (i * yInterval));
    display.setFont(&FreeSans8pt7b);
    // Temperature
    dataStr = String(tempBoundMax - (i * yTempMajorTicks)) + "`";
    drawString(xPos0 - 8, yTick + 4, dataStr, RIGHT);

    // PoP
    dataStr = String(100 - (i * 20));
    drawString(xPos1 + 8, yTick + 4, dataStr, LEFT);
    display.setFont(&FreeSans5pt7b);
    drawString(display.getCursorX(), yTick + 4, "%", LEFT);

    // draw dotted line
    if (i < yMajorTicks)
    {
      for (int x = xPos0; x <= xPos1 + 1; x += 3)
      {
        display.drawPixel(x, yTick + (yTick % 2), GxEPD_BLACK);
      }
    }
  }

  int xMaxTicks = 8;
  int hourInterval = static_cast<int>(ceil(HOURLY_GRAPH_MAX
                                           / static_cast<float>(xMaxTicks)));
  float xInterval = (xPos1 - xPos0 - 1) / static_cast<float>(HOURLY_GRAPH_MAX);
  display.setFont(&FreeSans8pt7b);
  for (int i = 0; i < HOURLY_GRAPH_MAX; ++i)
  {
    int xTick = static_cast<int>(xPos0 + (i * xInterval));
    int x0_t, x1_t, y0_t, y1_t;
    float yPxPerUnit;

    if (i > 0)
    {
      // temperature
      x0_t = static_cast<int>(round(xPos0 + ((i - 1) * xInterval) 
                              + (0.5 * xInterval) ));
      x1_t = static_cast<int>(round(xPos0 + (i * xInterval) 
                              + (0.5 * xInterval) ));
      yPxPerUnit = (yPos1 - yPos0) 
                   / static_cast<float>(tempBoundMax - tempBoundMin);
      y0_t = static_cast<int>(round(
                yPos1 - (yPxPerUnit * ((hourly[i - 1].temp) - tempBoundMin)) ));
      y1_t = static_cast<int>(round(
                yPos1 - (yPxPerUnit * ((hourly[i    ].temp) - tempBoundMin)) ));

      // graph temperature
      display.drawLine(x0_t    , y0_t    , x1_t    , y1_t    , GxEPD_BLACK);
      display.drawLine(x0_t    , y0_t + 1, x1_t    , y1_t + 1, GxEPD_BLACK);
      display.drawLine(x0_t - 1, y0_t    , x1_t - 1, y1_t    , GxEPD_BLACK);
    }

    // PoP
    x0_t = static_cast<int>(round( xPos0 + 1 + (i * xInterval)));
    x1_t = static_cast<int>(round( xPos0 + 1 + ((i + 1) * xInterval) ));
    yPxPerUnit = (yPos1 - yPos0) / 100.0;
    y0_t = static_cast<int>(round(
                            yPos1 - (yPxPerUnit * (hourly[i    ].pop * 100)) ));
    y1_t = yPos1;

    // graph PoP
    for (int y = y1_t - 1; y > y0_t; y -= 2)
    {
      for (int x = x0_t + (x0_t % 2); x < x1_t; x += 2)
      {
        display.drawPixel(x, y, GxEPD_BLACK);
      }
    }

    if ((i % hourInterval) == 0)
    {
      // draw x tick marks
      display.drawLine(xTick    , yPos1 + 1, xTick    , yPos1 + 4, GxEPD_BLACK);
      display.drawLine(xTick + 1, yPos1 + 1, xTick + 1, yPos1 + 4, GxEPD_BLACK);
      // draw x axis labels
      char timeBuffer[12] = {}; // big enough to accommodate "hh:mm:ss am"
      time_t ts = hourly[i].dt;
      tm *timeInfo = localtime(&ts);
      c_strftime(timeBuffer, sizeof(timeBuffer), HOUR_FORMAT, timeInfo);
      drawString(xTick, yPos1 + 1 + 12 + 4 + 3, timeBuffer, CENTER);
    }

  }

  // draw the last tick mark
  if ((HOURLY_GRAPH_MAX % hourInterval) == 0)
  {
    int xTick = static_cast<int>(round(xPos0 + (HOURLY_GRAPH_MAX * xInterval)));
    // draw x tick marks
    display.drawLine(xTick    , yPos1 + 1, xTick    , yPos1 + 4, GxEPD_BLACK);
    display.drawLine(xTick + 1, yPos1 + 1, xTick + 1, yPos1 + 4, GxEPD_BLACK);
    // draw x axis labels
    char timeBuffer[12] = {}; // big enough to accommodate "hh:mm:ss am"
    time_t ts = hourly[HOURLY_GRAPH_MAX - 1].dt + 3600;
    tm *timeInfo = localtime(&ts);
    c_strftime(timeBuffer, sizeof(timeBuffer), HOUR_FORMAT, timeInfo);
    drawString(xTick, yPos1 + 1 + 12 + 4 + 3, timeBuffer, CENTER);
  }

  return;
} // end drawOutlookGraph

/* This function is responsible for drawing the status bar along the bottom of
 * the display.
 */
void drawStatusBar(String statusStr, String refreshTimeStr, int rssi,
                   double batVoltage)
{
  String dataStr;
  display.setFont(&FreeSans6pt7b);
  int pos = DISP_WIDTH - 2;
  const int sp = 2;

  // battery
  int batPercent = calcBatPercent(batVoltage);
  dataStr = String(batPercent) + "% (" 
            + String( round(100.0 * batVoltage) / 100.0, 2 ) + "v)";
  drawString(pos, DISP_HEIGHT - 1 - 2, dataStr, RIGHT);
  pos -= getStringWidth(dataStr) + 25;
  display.drawInvertedBitmap(pos, DISP_HEIGHT - 1 - 17,
                             getBatBitmap24(batPercent), 24, 24, GxEPD_BLACK);
  pos -= sp + 9;

  // wifi
  dataStr = String(getWiFidesc(rssi));
  if (rssi != 0)
  {
    dataStr += " (" + String(rssi) + "dBm)";
  }
  drawString(pos, DISP_HEIGHT - 1 - 2, dataStr, RIGHT);
  pos -= getStringWidth(dataStr) + 19;
  display.drawInvertedBitmap(pos, DISP_HEIGHT - 1 - 13, getWiFiBitmap16(rssi),
                             16, 16, GxEPD_BLACK);
  pos -= sp + 8;

  // last refresh
  drawString(pos, DISP_HEIGHT - 1 - 2, refreshTimeStr, RIGHT);
  pos -= getStringWidth(refreshTimeStr) + 25;
  display.drawInvertedBitmap(pos, DISP_HEIGHT - 1 - 21, wi_refresh_32x32,
                             32, 32, GxEPD_BLACK);
  pos -= sp;

  // status
  if (!statusStr.isEmpty())
  {
    drawString(pos, DISP_HEIGHT - 1 - 2, statusStr, RIGHT);
    pos -= getStringWidth(statusStr) + 24;
    display.drawInvertedBitmap(pos, DISP_HEIGHT - 1 - 18, warning_icon_24x24, 
                               24, 24, GxEPD_BLACK);
  }

  return;
} // end drawStatusBar
