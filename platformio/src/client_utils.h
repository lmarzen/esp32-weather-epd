#ifndef __CLIENT_UTILS_H__
#define __CLIENT_UTILS_H__

wl_status_t startWiFi(int &wifiRSSI);
void killWiFi();
bool setupTime(tm *timeInfo);
void printLocalTime(tm *timeInfo);
bool getOWMonecall(WiFiClient &client, owm_resp_onecall_t &r);
bool getOWMairpollution(WiFiClient &client, owm_resp_air_pollution_t &r);

#endif