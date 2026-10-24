#include "config_portal.h"
#include "config.h"
#include "settings.h"
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

static WebServer server(80);
static DNSServer dnsServer;

static const byte DNS_PORT = 53;
static bool shouldRestart = false;
static unsigned long restartTime = 0;

static String escapeHtml(const String &str) {
  String out = "";
  for (unsigned int i = 0; i < str.length(); i++) {
    char c = str.charAt(i);
    if (c == '&') out += "&amp;";
    else if (c == '<') out += "&lt;";
    else if (c == '>') out += "&gt;";
    else if (c == '"') out += "&quot;";
    else if (c == '\'') out += "&#39;";
    else out += c;
  }
  return out;
}

static void handleRoot() {
  String currentSSID = Settings::getWiFiSSID();
  String currentOwmKey = Settings::getOwmApiKey();
  String currentLat = Settings::getLatitude();
  String currentLon = Settings::getLongitude();
  String currentCity = Settings::getCityString();
  String currentTz = Settings::getTimezone();
  int currentSleep = Settings::getSleepDuration();
  int currentBed = Settings::getBedTime();
  int currentWake = Settings::getWakeTime();

  // Scan WiFi networks
  int n = WiFi.scanComplete();
  if (n == -2) {
    WiFi.scanNetworks(true); // start async scan
  }

  String page = "<!DOCTYPE html><html><head><meta charset='UTF-8'>"
    "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
    "<title>Weather Station Setup</title>"
    "<style>"
    "body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif; background: #f0f2f5; margin: 0; padding: 20px; color: #1c1e21; }"
    ".card { max-width: 480px; margin: 0 auto; background: #fff; padding: 24px; border-radius: 12px; box-shadow: 0 4px 12px rgba(0,0,0,0.08); }"
    "h2 { margin-top: 0; text-align: center; color: #1877f2; }"
    "fieldset { border: 1px solid #ddd; border-radius: 8px; margin-bottom: 16px; padding: 12px 16px; }"
    "legend { font-weight: bold; color: #333; padding: 0 6px; font-size: 0.9em; text-transform: uppercase; letter-spacing: 0.5px; }"
    "label { display: block; margin: 8px 0 4px; font-weight: 500; font-size: 0.9em; }"
    "input, select { width: 100%; box-sizing: border-box; padding: 10px; border: 1px solid #ccc; border-radius: 6px; font-size: 14px; margin-bottom: 8px; }"
    "input:focus, select:focus { border-color: #1877f2; outline: none; }"
    ".row { display: flex; gap: 8px; }"
    ".row > div { flex: 1; }"
    "button { width: 100%; padding: 12px; background: #1877f2; color: #fff; border: none; border-radius: 6px; font-size: 16px; font-weight: bold; cursor: pointer; margin-top: 10px; }"
    "button:hover { background: #166fe5; }"
    ".hint { font-size: 0.8em; color: #65676b; margin-top: -4px; margin-bottom: 8px; }"
    "</style></head><body><div class='card'>"
    "<h2>Weather Station Setup</h2>"
    "<form action='/save' method='POST'>";

  // Wi-Fi Configuration
  page += "<fieldset><legend>Wi-Fi Network</legend>";
  page += "<label for='ssid'>Network Name (SSID)</label>";
  page += "<input type='text' id='ssid' name='ssid' list='networks' value='" + escapeHtml(currentSSID) + "' required>";
  page += "<datalist id='networks'>";
  if (n > 0) {
    for (int i = 0; i < n; ++i) {
      page += "<option value='" + escapeHtml(WiFi.SSID(i)) + "'>";
    }
  }
  page += "</datalist>";
  page += "<label for='pass'>Password</label>";
  page += "<input type='password' id='pass' name='pass' placeholder='Leave blank to keep unchanged'>";
  page += "</fieldset>";

  // OpenWeatherMap & Location
  page += "<fieldset><legend>Weather & Location</legend>";
  page += "<label for='owm_key'>OpenWeatherMap API Key</label>";
  page += "<input type='text' id='owm_key' name='owm_key' value='" + escapeHtml(currentOwmKey) + "' required>";
  page += "<div class='row'>";
  page += "<div><label for='lat'>Latitude</label><input type='text' id='lat' name='lat' value='" + escapeHtml(currentLat) + "' required></div>";
  page += "<div><label for='lon'>Longitude</label><input type='text' id='lon' name='lon' value='" + escapeHtml(currentLon) + "' required></div>";
  page += "</div>";
  page += "<label for='city'>City Name</label>";
  page += "<input type='text' id='city' name='city' value='" + escapeHtml(currentCity) + "' required>";
  page += "<div class='hint'>Displayed on the top-right corner.</div>";
  page += "</fieldset>";

  // Time & Schedule
  page += "<fieldset><legend>Time & Power Schedule</legend>";
  page += "<label for='tz'>POSIX Timezone</label>";
  page += "<input type='text' id='tz' name='tz' value='" + escapeHtml(currentTz) + "' required>";
  page += "<div class='hint'>e.g., EST5EDT,M3.2.0,M11.1.0 or PST8PDT,M3.2.0,M11.1.0</div>";
  page += "<div class='row'>";
  page += "<div><label for='sleep'>Update (min)</label><input type='number' id='sleep' name='sleep' min='2' max='1440' value='" + String(currentSleep) + "'></div>";
  page += "<div><label for='bed'>Bed Hour</label><input type='number' id='bed' name='bed' min='0' max='23' value='" + String(currentBed) + "'></div>";
  page += "<div><label for='wake'>Wake Hour</label><input type='number' id='wake' name='wake' min='0' max='23' value='" + String(currentWake) + "'></div>";
  page += "</div>";
  page += "</fieldset>";

  page += "<button type='submit'>Save and Reboot</button>";
  page += "</form></div></body></html>";

  server.send(200, "text/html", page);
}

static void handleSave() {
  String ssid = server.hasArg("ssid") ? server.arg("ssid") : "";
  String pass = server.hasArg("pass") ? server.arg("pass") : "";
  String owmKey = server.hasArg("owm_key") ? server.arg("owm_key") : "";
  String lat = server.hasArg("lat") ? server.arg("lat") : "";
  String lon = server.hasArg("lon") ? server.arg("lon") : "";
  String city = server.hasArg("city") ? server.arg("city") : "";
  String tz = server.hasArg("tz") ? server.arg("tz") : "";
  int sleepDur = server.hasArg("sleep") ? server.arg("sleep").toInt() : 30;
  int bedTime = server.hasArg("bed") ? server.arg("bed").toInt() : 0;
  int wakeTime = server.hasArg("wake") ? server.arg("wake").toInt() : 6;

  ssid.trim();
  pass.trim();
  owmKey.trim();
  lat.trim();
  lon.trim();
  city.trim();
  tz.trim();

  Settings::saveSettings(ssid, pass, owmKey, lat, lon, city, tz, sleepDur, bedTime, wakeTime);

  String resp = "<!DOCTYPE html><html><head><meta charset='UTF-8'>"
    "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
    "<title>Settings Saved</title>"
    "<style>"
    "body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif; background: #f0f2f5; margin: 0; padding: 40px 20px; text-align: center; }"
    ".card { max-width: 400px; margin: 0 auto; background: #fff; padding: 24px; border-radius: 12px; box-shadow: 0 4px 12px rgba(0,0,0,0.08); }"
    "h2 { color: #2e7d32; }"
    "</style></head><body><div class='card'>"
    "<h2>Settings Saved!</h2>"
    "<p>The Weather Station is restarting with the new configuration.</p>"
    "<p>You can disconnect from this Wi-Fi network now.</p>"
    "</div></body></html>";

  server.send(200, "text/html", resp);
  shouldRestart = true;
  restartTime = millis() + 2000;
}

static void handleNotFound() {
  server.sendHeader("Location", String("http://") + server.client().localIP().toString() + "/", true);
  server.send(302, "text/plain", "");
}

void runConfigPortal() {
  Serial.println("Starting Dynamic Configuration Portal...");

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(CONFIG_AP_SSID);
  delay(100);

  Serial.print("AP IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Redirect all DNS requests to SoftAP IP for captive portal
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  // Web routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/save", HTTP_POST, handleSave);

  // Common captive portal probe paths
  server.on("/generate_204", HTTP_GET, handleRoot);
  server.on("/hotspot-detect.html", HTTP_GET, handleRoot);
  server.on("/canonical.html", HTTP_GET, handleRoot);
  server.on("/connecttest.txt", HTTP_GET, handleRoot);

  server.onNotFound(handleNotFound);
  server.begin();

  // Scan WiFi networks once in the background
  WiFi.scanNetworks(true);

  // Run portal loop with a 5-minute inactivity timeout
  unsigned long timeout = millis() + (5 * 60 * 1000);
  while (true) {
    dnsServer.processNextRequest();
    server.handleClient();

    if (shouldRestart && millis() >= restartTime) {
      Serial.println("Restarting ESP32...");
      delay(200);
      ESP.restart();
    }

    if (millis() > timeout) {
      Serial.println("Config portal timed out. Restarting...");
      delay(200);
      ESP.restart();
    }

    delay(5);
  }
}
