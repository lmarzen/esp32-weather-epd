#pragma once
#include <Arduino.h>

namespace Settings {
  void init();
  bool isConfigured();

  String getWiFiSSID();
  String getWiFiPassword();
  String getOwmApiKey();
  String getLatitude();
  String getLongitude();
  String getCityString();
  const char* getTimezone();
  int getSleepDuration();
  int getBedTime();
  int getWakeTime();

  void saveSettings(const String &ssid, const String &pass,
                    const String &owmKey, const String &lat,
                    const String &lon, const String &city,
                    const String &tz, int sleepDur,
                    int bedTime, int wakeTime);
  void resetToDefaults();
}
