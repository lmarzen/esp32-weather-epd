#ifndef __CLIENT_UTILS_H__
#define __CLIENT_UTILS_H__

#include <Arduino.h>

wl_status_t startWiFi(int &wifiRSSI);
void killWiFi();
bool setupTime(tm *timeInfo);
bool printLocalTime(tm *timeInfo);
int getOWMonecall(WiFiClient &client, owm_resp_onecall_t &r);
int getOWMairpollution(WiFiClient &client, owm_resp_air_pollution_t &r);

#endif
