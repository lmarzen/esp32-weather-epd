#ifndef __DISPLAY_UTILS_H__
#define __DISPLAY_UTILS_H__

#include <vector>
#include "api_response.h"

void toTitleCase(String &text);
void truncateExtraneousInfo(String &text);
void prepareAlerts(std::vector<owm_alerts_t> &resp);
const uint8_t *getForecastBitmap(owm_daily_t &daily);
const uint8_t *getCurrentBitmap(owm_current_t &current);

#endif