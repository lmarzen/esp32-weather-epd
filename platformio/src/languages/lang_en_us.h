#ifndef __LANG_EN_US_H__
#define __LANG_EN_US_H__

#include <vector>
#include <Arduino.h>


// LANGUAGE
extern const String LANG;

// CURRENT CONDITIONS
extern const char *TXT_FEELS_LIKE;
extern const char *TXT_SUNRISE;
extern const char *TXT_SUNSET;
extern const char *TXT_WIND;
extern const char *TXT_HUMIDITY;
extern const char *TXT_UV_INDEX;
extern const char *TXT_PRESSURE;
extern const char *TXT_AIR_QUALITY_INDEX;
extern const char *TXT_VISIBILITY;
extern const char *TXT_INDOOR_TEMPERATURE;
extern const char *TXT_INDOOR_HUMIDITY;

// TIME/DATE
extern const char *TXT_ddd[];
extern const char *TXT_dddd[];
extern const char *TXT_MMM[];
extern const char *TXT_MMMM[];

// ALERTS
extern const std::vector<String> ALERT_URGENCY;
// ALERT TERMINOLOGY
extern const std::vector<String> TERM_SMOG;
extern const std::vector<String> TERM_SMOKE;
extern const std::vector<String> TERM_FOG;
extern const std::vector<String> TERM_METEOR;
extern const std::vector<String> TERM_NUCLEAR;
extern const std::vector<String> TERM_BIOHAZARD;
extern const std::vector<String> TERM_EARTHQUAKE;
extern const std::vector<String> TERM_TSUNAMI;
extern const std::vector<String> TERM_FIRE;
extern const std::vector<String> TERM_HEAT;
extern const std::vector<String> TERM_WINTER;
extern const std::vector<String> TERM_LIGHTNING;
extern const std::vector<String> TERM_SANDSTORM;
extern const std::vector<String> TERM_FLOOD;
extern const std::vector<String> TERM_VOLCANO;
extern const std::vector<String> TERM_AIR_QUALITY;
extern const std::vector<String> TERM_TORNADO;
extern const std::vector<String> TERM_SMALL_CRAFT_ADVISORY;
extern const std::vector<String> TERM_GALE_WARNING;
extern const std::vector<String> TERM_STORM_WARNING;
extern const std::vector<String> TERM_HURRICANE_WARNING;
extern const std::vector<String> TERM_HURRICANE;
extern const std::vector<String> TERM_DUST;
extern const std::vector<String> TERM_STRONG_WIND;

#endif