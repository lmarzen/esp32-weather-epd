#include "settings.h"
#include "config.h"
#include <Preferences.h>

namespace Settings {
  static Preferences prefs;
  static bool initialized = false;
  static String cachedTimezone = "";

  void init() {
    if (!initialized) {
      prefs.begin(NVS_NAMESPACE, false);
      initialized = true;
    }
  }

  bool isConfigured() {
    init();
    return prefs.getBool("is_configured", false);
  }

  String getWiFiSSID() {
    init();
    String val = prefs.getString("wifi_ssid", "");
    if (val.length() > 0) {
      return val;
    }
    return String(WIFI_SSID);
  }

  String getWiFiPassword() {
    init();
    String val = prefs.getString("wifi_pass", "");
    if (val.length() > 0) {
      return val;
    }
    return String(WIFI_PASSWORD);
  }

  String getOwmApiKey() {
    init();
    String val = prefs.getString("owm_key", "");
    if (val.length() > 0) {
      return val;
    }
    return OWM_APIKEY;
  }

  String getLatitude() {
    init();
    String val = prefs.getString("lat", "");
    if (val.length() > 0) {
      return val;
    }
    return LAT;
  }

  String getLongitude() {
    init();
    String val = prefs.getString("lon", "");
    if (val.length() > 0) {
      return val;
    }
    return LON;
  }

  String getCityString() {
    init();
    String val = prefs.getString("city", "");
    if (val.length() > 0) {
      return val;
    }
    return CITY_STRING;
  }

  const char* getTimezone() {
    init();
    String val = prefs.getString("tz", "");
    if (val.length() > 0) {
      cachedTimezone = val;
      return cachedTimezone.c_str();
    }
    return TIMEZONE;
  }

  int getSleepDuration() {
    init();
    int val = prefs.getInt("sleep_min", -1);
    if (val > 0) {
      return val;
    }
    return SLEEP_DURATION;
  }

  int getBedTime() {
    init();
    int val = prefs.getInt("bed_time", -1);
    if (val >= 0 && val <= 23) {
      return val;
    }
    return BED_TIME;
  }

  int getWakeTime() {
    init();
    int val = prefs.getInt("wake_time", -1);
    if (val >= 0 && val <= 23) {
      return val;
    }
    return WAKE_TIME;
  }

  void saveSettings(const String &ssid, const String &pass,
                    const String &owmKey, const String &lat,
                    const String &lon, const String &city,
                    const String &tz, int sleepDur,
                    int bedTime, int wakeTime) {
    init();
    if (ssid.length() > 0) prefs.putString("wifi_ssid", ssid);
    if (pass.length() > 0) prefs.putString("wifi_pass", pass);
    if (owmKey.length() > 0) prefs.putString("owm_key", owmKey);
    if (lat.length() > 0) prefs.putString("lat", lat);
    if (lon.length() > 0) prefs.putString("lon", lon);
    if (city.length() > 0) prefs.putString("city", city);
    if (tz.length() > 0) prefs.putString("tz", tz);
    if (sleepDur > 0) prefs.putInt("sleep_min", sleepDur);
    if (bedTime >= 0 && bedTime <= 23) prefs.putInt("bed_time", bedTime);
    if (wakeTime >= 0 && wakeTime <= 23) prefs.putInt("wake_time", wakeTime);

    prefs.putBool("is_configured", true);
  }

  void resetToDefaults() {
    init();
    prefs.remove("wifi_ssid");
    prefs.remove("wifi_pass");
    prefs.remove("owm_key");
    prefs.remove("lat");
    prefs.remove("lon");
    prefs.remove("city");
    prefs.remove("tz");
    prefs.remove("sleep_min");
    prefs.remove("bed_time");
    prefs.remove("wake_time");
    prefs.putBool("is_configured", false);
  }
}
