#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "parse.h"

bool deserializeOneCall(WiFiClient &json, owm_resp_onecall_t *results) {


  StaticJsonDocument<832> filter;
  filter["current"] = true;
  filter["minutely"] = false;
  filter["hourly"] = true;
  filter["daily"] = true;

  JsonArray filter_alerts = filter.createNestedArray("alerts");

  JsonObject filter_alerts_0 = filter_alerts.createNestedObject();
  filter_alerts_0["sender_name"] = false;
  filter_alerts_0["event"] = true;
  filter_alerts_0["start"] = true;
  filter_alerts_0["end"] = true;
  filter_alerts_0["description"] = false;
  filter_alerts_0["tags"] = true;
  JsonObject filter_alerts_1 = filter_alerts.createNestedObject();
  filter_alerts_1["sender_name"] = false;
  filter_alerts_1["event"] = true;
  filter_alerts_1["start"] = true;
  filter_alerts_1["end"] = true;
  filter_alerts_1["description"] = false;
  filter_alerts_1["tags"] = true;
  JsonObject filter_alerts_2 = filter_alerts.createNestedObject();
  filter_alerts_2["sender_name"] = false;
  filter_alerts_2["event"] = true;
  filter_alerts_2["start"] = true;
  filter_alerts_2["end"] = true;
  filter_alerts_2["description"] = false;
  filter_alerts_2["tags"] = true;
  JsonObject filter_alerts_3 = filter_alerts.createNestedObject();
  filter_alerts_3["sender_name"] = false;
  filter_alerts_3["event"] = true;
  filter_alerts_3["start"] = true;
  filter_alerts_3["end"] = true;
  filter_alerts_3["description"] = false;
  filter_alerts_3["tags"] = true;
  JsonObject filter_alerts_4 = filter_alerts.createNestedObject();
  filter_alerts_4["sender_name"] = false;
  filter_alerts_4["event"] = true;
  filter_alerts_4["start"] = true;
  filter_alerts_4["end"] = true;
  filter_alerts_4["description"] = false;
  filter_alerts_4["tags"] = true;
  JsonObject filter_alerts_5 = filter_alerts.createNestedObject();
  filter_alerts_5["sender_name"] = false;
  filter_alerts_5["event"] = true;
  filter_alerts_5["start"] = true;
  filter_alerts_5["end"] = true;
  filter_alerts_5["description"] = false;
  filter_alerts_5["tags"] = true;
  JsonObject filter_alerts_6 = filter_alerts.createNestedObject();
  filter_alerts_6["sender_name"] = false;
  filter_alerts_6["event"] = true;
  filter_alerts_6["start"] = true;
  filter_alerts_6["end"] = true;
  filter_alerts_6["description"] = false;
  filter_alerts_6["tags"] = true;
  JsonObject filter_alerts_7 = filter_alerts.createNestedObject();
  filter_alerts_7["sender_name"] = false;
  filter_alerts_7["event"] = true;
  filter_alerts_7["start"] = true;
  filter_alerts_7["end"] = true;
  filter_alerts_7["description"] = false;
  filter_alerts_7["tags"] = true;

  DynamicJsonDocument doc(32 * 1024);

  DeserializationError error = deserializeJson(doc, json, DeserializationOption::Filter(filter));

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return false;
  }

  return true;
}







bool deserializeAirQuality(WiFiClient& json, owm_resp_air_pollution_t *results);