/* MQTT telemetry helpers for esp32-weather-epd.
 * Copyright (C) 2026  Luke Marzen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "config.h"

#if MQTT_ENABLED
#include <Arduino.h>
#include <ArduinoJson.h>
#include <MQTT.h>
#include <WiFi.h>

#include "mqtt_utils.h"

namespace
{
constexpr unsigned long MQTT_TIMEOUT_MS = 3000;
constexpr char HA_DISCOVERY_PREFIX[] = "homeassistant";

String hardwareDeviceId()
{
  const uint64_t mac = ESP.getEfuseMac();
  char id[17];
  snprintf(id, sizeof(id), "%04x%08x",
           static_cast<unsigned int>(mac >> 32),
           static_cast<unsigned int>(mac));
  return String(id);
}

#if MQTT_HOME_ASSISTANT_DISCOVERY
void publishDiscovery(MQTTClient &client, const String &deviceId,
                      const char *objectId, const char *name,
                      const char *deviceClass, const char *unit,
                      int precision, const char *valueKey)
{
  const String uniqueId = deviceId + "_" + objectId;
  const String configTopic = String(HA_DISCOVERY_PREFIX) + "/sensor/"
                           + deviceId + "/" + objectId + "/config";
  const String stateTopic = String(MQTT_BASE_TOPIC) + "/state";

  JsonDocument config;
  config["name"] = name;
  config["unique_id"] = uniqueId;
  config["state_topic"] = stateTopic;
  String valueTemplate = "{{ value_json.";
  valueTemplate += valueKey;
  valueTemplate += " }}";
  config["value_template"] = valueTemplate;
  config["device_class"] = deviceClass;
  config["unit_of_measurement"] = unit;
  config["state_class"] = "measurement";
  config["suggested_display_precision"] = precision;
  JsonArray identifiers = config["device"]["identifiers"].to<JsonArray>();
  identifiers.add(deviceId);
  config["device"]["name"] = MQTT_DEVICE_NAME;

  String payload;
  serializeJson(config, payload);
  client.publish(configTopic, payload, true, 0);
}
#endif
} // namespace

void publishMqttTelemetry(float temperature, float humidity,
                          uint32_t batteryVoltage, int wifiRSSI)
{
  WiFiClient network;
  network.setTimeout(MQTT_TIMEOUT_MS);
  MQTTClient client(1024);
  client.setTimeout(MQTT_TIMEOUT_MS);
  client.begin(MQTT_BROKER, MQTT_PORT, network);

  const String deviceId = hardwareDeviceId();
  const String clientId = String("weather-epd-") + deviceId;
  if (!client.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD))
  {
    Serial.println("MQTT: connection failed");
    return;
  }

  Serial.println("MQTT: connected");

#if MQTT_HOME_ASSISTANT_DISCOVERY
  publishDiscovery(client, deviceId, "temperature", "Temperature",
                   "temperature", "°C", 1, "temperature");
  publishDiscovery(client, deviceId, "humidity", "Humidity",
                   "humidity", "%", 1, "humidity");
  publishDiscovery(client, deviceId, "battery_voltage", "Battery Voltage",
                   "voltage", "V", 2, "battery_voltage");
  publishDiscovery(client, deviceId, "wifi_rssi", "Wi-Fi RSSI",
                   "signal_strength", "dBm", 0, "wifi_rssi");
#endif

  JsonDocument state;
  state["temperature"] = temperature;
  state["humidity"] = humidity;
  state["battery_voltage"] = batteryVoltage / 1000.0f;
  state["wifi_rssi"] = wifiRSSI;
  String payload;
  serializeJson(state, payload);

  const String stateTopic = String(MQTT_BASE_TOPIC) + "/state";
  const bool published = client.publish(stateTopic, payload, false, 1);
  Serial.println(published ? "MQTT: telemetry published" : "MQTT: publish failed");
  client.disconnect();
}
#endif // MQTT_ENABLED
