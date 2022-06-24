#ifndef __WEATHER_H__
#define __WEATHER_H__

#include <vector>
#include "api_response.h"

void prepareAlerts(std::vector<owm_alerts_t> *resp);

#endif