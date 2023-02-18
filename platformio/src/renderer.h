#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <vector>
#include <Arduino.h>
#include <time.h>
#include "api_response.h"
#include "config.h"

#define DISP_WIDTH  800
#define DISP_HEIGHT 480

#ifdef DISP_BW
#include <GxEPD2_BW.h>
extern GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT> display;
#endif
#ifdef DISP_3C
#include <GxEPD2_3C.h>
extern GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT / 2> display;
#endif

typedef enum alignment
{
  LEFT,
  RIGHT,
  CENTER
} alignment_t;

uint16_t getStringWidth(String text);
uint16_t getStringHeight(String text);
void drawString(int16_t x, int16_t y, String text, alignment_t alignment);
void drawMultiLnString(int16_t x, int16_t y, String text, alignment_t alignment, 
                       uint16_t max_width, uint16_t max_lines, 
                       int16_t line_spacing);
void initDisplay();
void drawCurrentConditions(owm_current_t &current, owm_daily_t &today,
                           owm_resp_air_pollution_t &owm_air_pollution, 
                           float inTemp, float inHumidity);
void drawForecast(owm_daily_t *const daily, tm timeInfo);
void drawAlerts(std::vector<owm_alerts_t> &alerts,
                const String &city, const String &date);
void drawLocationDate(const String &city, const String &date);
void drawOutlookGraph(owm_hourly_t *const hourly, tm timeInfo);
void drawStatusBar(String statusStr, String refreshTimeStr, int rssi, 
                   double batVoltage);
void drawError(const uint8_t *bitmap_196x196, 
               const String &errMsgLn1, const String &errMsgLn2);

#endif
