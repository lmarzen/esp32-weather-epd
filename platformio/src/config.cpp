/* Configuration options for esp32-weather-epd.
 * Copyright (C) 2022-2025  Luke Marzen
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

#include <Arduino.h>
#include "config.h"

// PINS
// The configuration below is intended for use with the project's official 
// wiring diagrams using the FireBeetle 2 ESP32-E microcontroller board.
//
// Note: LED_BUILTIN pin will be disabled to reduce power draw.  Refer to your
//       board's pinout to ensure you avoid using a pin with this shared 
//       functionality.
//
// ADC pin used to measure battery voltage
const uint8_t PIN_BAT_ADC  = A2; // A0 for micro-usb firebeetle
// Pins for E-Paper Driver Board
const uint8_t PIN_EPD_BUSY = 14; // 5 for micro-usb firebeetle
const uint8_t PIN_EPD_CS   = 13;
const uint8_t PIN_EPD_RST  = 21;
const uint8_t PIN_EPD_DC   = 22;
const uint8_t PIN_EPD_SCK  = 18;
const uint8_t PIN_EPD_MISO = 19; // 19 Master-In Slave-Out not used, as no data from display
const uint8_t PIN_EPD_MOSI = 23;
const uint8_t PIN_EPD_PWR  = 26; // Irrelevant if directly connected to 3.3V
// I2C Pins used for BME280
const uint8_t PIN_BME_SDA = 17;
const uint8_t PIN_BME_SCL = 16;
const uint8_t PIN_BME_PWR =  4;   // Irrelevant if directly connected to 3.3V
const uint8_t BME_ADDRESS = 0x76; // 0x76 if SDO -> GND; 0x77 if SDO -> VCC

// WIFI
const char *WIFI_SSID     = "ssid";
const char *WIFI_PASSWORD = "password";
const unsigned long WIFI_TIMEOUT = 10000; // ms, WiFi connection timeout.

// HTTP
// The following errors are likely the result of insuffient http client tcp 
// timeout:
//   -1   Connection Refused
//   -11  Read Timeout
//   -258 Deserialization Incomplete Input
const unsigned HTTP_CLIENT_TCP_TIMEOUT = 10000; // ms

// OPENWEATHERMAP API
// OpenWeatherMap API key, https://openweathermap.org/
const String OWM_APIKEY   = "abcdefghijklmnopqrstuvwxyz012345";
const String OWM_ENDPOINT = "api.openweathermap.org";
// OpenWeatherMap One Call 2.5 API is deprecated for all new free users
// (accounts created after Summer 2022).
//
// Please note, that One Call API 3.0 is included in the "One Call by Call"
// subscription only. This separate subscription includes 1,000 calls/day for
// free and allows you to pay only for the number of API calls made to this
// product.
//
// Here’s how to subscribe and avoid any credit card changes:
// - Go to https://home.openweathermap.org/subscriptions/billing_info/onecall_30/base?key=base&service=onecall_30
// - Follow the instructions to complete the subscription.
// - Go to https://home.openweathermap.org/subscriptions and set the "Calls per
//   day (no more than)" to 1,000. This ensures you will never overrun the free
//   calls.
const String OWM_ONECALL_VERSION = "3.0";

// LOCATION
// Set your latitude and longitude.
// (used to get weather data as part of API requests to OpenWeatherMap)
const String LAT = "40.7128";
const String LON = "-74.0060";
// City name that will be shown in the top-right corner of the display.
const String CITY_STRING = "New York";

// TIME
// For list of time zones see
// https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
const char *TIMEZONE = "EST5EDT,M3.2.0,M11.1.0";
// Time format used when displaying sunrise/set times. (Max 11 characters)
// For more information about formatting see
// https://man7.org/linux/man-pages/man3/strftime.3.html
// const char *TIME_FORMAT = "%l:%M%P"; // 12-hour ex: 1:23am  11:00pm
const char *TIME_FORMAT = "%H:%M";   // 24-hour ex: 01:23   23:00
// Time format used when displaying axis labels. (Max 11 characters)
// For more information about formatting see
// https://man7.org/linux/man-pages/man3/strftime.3.html
// const char *HOUR_FORMAT = "%l%P"; // 12-hour ex: 1am  11pm
const char *HOUR_FORMAT = "%H";      // 24-hour ex: 01   23
// Date format used when displaying date in top-right corner.
// For more information about formatting see
// https://man7.org/linux/man-pages/man3/strftime.3.html
const char *DATE_FORMAT = "%a, %B %e"; // ex: Sat, January 1
// Date/Time format used when displaying the last refresh time along the bottom
// of the screen.
// For more information about formatting see
// https://man7.org/linux/man-pages/man3/strftime.3.html
const char *REFRESH_TIME_FORMAT = "%x %H:%M";
// NTP_SERVER_1 is the primary time server, while NTP_SERVER_2 is a fallback.
// pool.ntp.org will find the closest available NTP server to you.
const char *NTP_SERVER_1 = "pool.ntp.org";
const char *NTP_SERVER_2 = "time.nist.gov";
// If you encounter the 'Failed To Fetch The Time' error, try increasing
// NTP_TIMEOUT or select closer/lower latency time servers.
const unsigned long NTP_TIMEOUT = 20000; // ms

// --- Schedule Array ---
//
// For example, we define:
// - Monday, Wednesday, Friday:
//     • 07:00 to 22:00  every 90 minutes
// - Tuesday, Thursday:
//     • 07:00-09:00 every 60 minutes
//     • 16:30-22:00 every 90 minutes
// - Saturday, Sunday
//     • 08:00-23:00 every 90 minutes
ScheduleSegment scheduleSegments[] = {
  {MONDAY | WEDNESDAY | FRIDAY, "07:00", "22:00", 90},
  {TUESDAY | THURSDAY, "07:00", "09:00", 60},
  {TUESDAY | THURSDAY, "16:30", "22:00", 90},
  {SATURDAY | SUNDAY, "08:00", "23:00", 90}
};
const int scheduleSegmentsCount = sizeof(scheduleSegments) / sizeof(scheduleSegments[0]);

// HOURLY OUTLOOK GRAPH
// Number of hours to display on the outlook graph. (range: [8-48])
const int HOURLY_GRAPH_MAX = 24;

// BATTERY
// To protect the battery upon LOW_BATTERY_VOLTAGE, the display will cease to
// update until battery is charged again. The ESP32 will deep-sleep (consuming
// < 11μA), waking briefly check the voltage at the corresponding interval (in
// minutes). Once the battery voltage has fallen to CRIT_LOW_BATTERY_VOLTAGE,
// the esp32 will hibernate and a manual press of the reset (RST) button to
// begin operating again.
const uint32_t WARN_BATTERY_VOLTAGE     = 3535; // (millivolts) ~20%
const uint32_t LOW_BATTERY_VOLTAGE      = 3462; // (millivolts) ~10%
const uint32_t VERY_LOW_BATTERY_VOLTAGE = 3442; // (millivolts)  ~8%
const uint32_t CRIT_LOW_BATTERY_VOLTAGE = 3404; // (millivolts)  ~5%
const unsigned long LOW_BATTERY_SLEEP_INTERVAL      = 30;  // (minutes)
const unsigned long VERY_LOW_BATTERY_SLEEP_INTERVAL = 120; // (minutes)
// Battery voltage calculations are based on a typical 3.7v LiPo.
const uint32_t MAX_BATTERY_VOLTAGE = 4200; // (millivolts)
const uint32_t MIN_BATTERY_VOLTAGE = 3000; // (millivolts)

// See config.h for the below options
// E-PAPER PANEL
// LOCALE
// UNITS
// WIND ICON PRECISION
// FONTS
// ALERTS
// BATTERY MONITORING

