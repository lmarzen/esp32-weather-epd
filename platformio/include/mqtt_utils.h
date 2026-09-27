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
#ifndef __MQTT_UTILS_H__
#define __MQTT_UTILS_H__

#include "config.h"

#if MQTT_ENABLED
#include <cstdint>

void publishMqttTelemetry(float temperature, float humidity,
                          uint32_t batteryVoltage, int wifiRSSI);
#endif

#endif
