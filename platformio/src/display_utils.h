#ifndef __DISPLAY_UTILS_H__
#define __DISPLAY_UTILS_H__

#include <vector>
#include "api_response.h"

enum alert_category {
  NOT_FOUND = -1,
  SMOG,
  SMOKE,
  FOG,
  METEOR,
  NUCLEAR,
  BIOHAZARD,
  EARTHQUAKE,
  TSUNAMI,
  FIRE,
  HEAT,
  WINTER,
  LIGHTNING,
  SANDSTORM,
  FLOOD,
  VOLCANO,
  AIR_QUALITY,
  TORNADO,
  SMALL_CRAFT_ADVISORY,
  GALE_WARNING,
  STORM_WARNING,
  HURRICANE_WARNING,
  HURRICANE,
  DUST,
  STRONG_WIND
};

void toTitleCase(String &text);
void truncateExtraAlertInfo(String &text);
void filterAlerts(std::vector<owm_alerts_t> &resp);
const char *getUVIdesc(unsigned int uvi);
float getAvgConc(float pollutant[], int hours);
int getAQI(owm_resp_air_pollution_t &p);
const char *getAQIdesc(int aqi);
const uint8_t *getForecastBitmap64(owm_daily_t &daily);
const uint8_t *getCurrentConditionsBitmap196(owm_current_t &current, 
                                             owm_daily_t   &today);
const uint8_t *getAlertBitmap32(owm_alerts_t &alert);
const uint8_t *getAlertBitmap48(owm_alerts_t &alert);
enum alert_category getAlertCategory(owm_alerts_t &alert);
const uint8_t *getWindBitmap24(int windDeg);


#endif