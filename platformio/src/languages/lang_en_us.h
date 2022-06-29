#ifndef __LANG_EN_US_H__
#define __LANG_EN_US_H__

#include <vector>
#include <Arduino.h>

// CURRENT CONDITIONS
extern const String TXT_SUNRISE;
extern const String TXT_SUNSET;
extern const String TXT_PRECIPITATION;
extern const String TXT_HUMIDITY;
extern const String TXT_WIND;
extern const String TXT_PRESSURE;
extern const String TXT_UV_INDEX;
extern const String TXT_VISIBILITY;
extern const String TXT_INDOOR_TEMPERATURE;
extern const String TXT_INDOOR_HUMIDITY;

// TIME/DATE
extern const char *TXT_ddd[];
extern const char *TXT_dddd[];
extern const char *TXT_MMM[];
extern const char *TXT_MMMM[];

// ALERTS
const std::vector<String> ALERT_URGENCY;
// ALERT TERMINOLOGY
const std::vector<String> TERM_SMOG;
const std::vector<String> TERM_SMOKE;
const std::vector<String> TERM_FOG;
const std::vector<String> TERM_METEOR;
const std::vector<String> TERM_NUCLEAR;
const std::vector<String> TERM_BIOHAZARD;
const std::vector<String> TERM_EARTHQUAKE;
const std::vector<String> TERM_TSUNAMI;
const std::vector<String> TERM_FIRE;
const std::vector<String> TERM_THERMOMETER_HOT;
const std::vector<String> TERM_SNOWFLAKE_COLD;
const std::vector<String> TERM_LIGHTNING;
const std::vector<String> TERM_SANDSTORM;
const std::vector<String> TERM_FLOOD;
const std::vector<String> TERM_VOLCANO;
const std::vector<String> TERM_AIR_QUALITY;
const std::vector<String> TERM_TORNADO;
const std::vector<String> TERM_SMALL_CRAFT_ADVISORY;
const std::vector<String> TERM_GALE_WARNING;
const std::vector<String> TERM_STORM_WARNING;
const std::vector<String> TERM_HURRICANE_WARNING;
const std::vector<String> TERM_HURRICANE;
const std::vector<String> TERM_DUST;
const std::vector<String> TERM_STRONG_WIND;

#endif