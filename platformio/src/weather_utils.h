#ifndef __WEATHER_UTILS_H__
#define __WEATHER_UTILS_H__

#include <vector>
#include "api_response.h"

void toTitleCase(String *text);
void truncateExtraneousInfo(String *text);
void prepareAlerts(std::vector<owm_alerts_t> *resp);

#endif