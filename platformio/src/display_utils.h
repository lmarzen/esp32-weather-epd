#ifndef __DISPLAY_UTILS_H__
#define __DISPLAY_UTILS_H__

#include <vector>
#include "api_response.h"

void toTitleCase(String &text);
void truncateExtraneousInfo(String &text);
void prepareAlerts(std::vector<owm_alerts_t> &resp);
const uint8_t *getForecastBitmap64(owm_daily_t &daily);
const uint8_t *getCurrentBitmap196(owm_current_t &current);
const uint8_t *getAlertBitmap48(owm_alerts_t &alert);
const uint8_t *getAlertBitmap32(owm_alerts_t &alert);

#endif