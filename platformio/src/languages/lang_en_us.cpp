#include <vector>
#include <Arduino.h>

// CURRENT CONDITIONS
const String TXT_SUNRISE            = "Sunrise";
const String TXT_SUNSET             = "Sunset";
const String TXT_PRECIPITATION      = "Precipitation";
const String TXT_HUMIDITY           = "Humidity";
const String TXT_WIND               = "Wind";
const String TXT_PRESSURE           = "Pressure";
const String TXT_UV_INDEX           = "UV Index";
const String TXT_VISIBILITY         = "Visibility";
const String TXT_INDOOR_TEMPERATURE = "Temperature";
const String TXT_INDOOR_HUMIDITY    = "Humidity";

// TIME/DATE
// The abbreviated name of the day of the week.
const char *TXT_ddd[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
// The full name of the day of the week.
const char *TXT_dddd[] = {"Sunday", "Monday", "Tuesday", "Wednesday", 
                          "Thursday", "Friday", "Saturday"};
// The abbreviated name of the month.
const char *TXT_MMM[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", 
                         "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
// The full name of the month.
const char *TXT_MMMM[] = {"January",   "February", "March",    "April", 
                          "May",       "June",     "July",     "August", 
                          "September", "October",  "November", "December"};

// ALERTS
// The display can show up to 2 alerts, but alerts can be unpredictible in
// severity and number. If more than 2 alerts are active, the esp32 will attempt
// to interpret the urgency of each alert and prefer to display the most urgent 
// and recently issued alerts of each event type. Depending on your region 
// different keywords are used to convey the level of urgency.
//
// A vector array is used to store these keywords. Urgency is ranked from low to
// high where the first index of the vector is the least urgent keyword and the 
// last index is the most urgent keyword. Expected as all lowercase.
//
// Here are a few examples, uncomment the array for your region (or create your 
// own).
const std::vector<String> ALERT_URGENCY = {"statement", "watch", "advisory", "warning", "emergency"}; // US National Weather Service
// const std::vector<String> ALERT_URGENCY = {"yellow", "amber", "red"};                 // United Kingdom's national weather service (MET Office)
// const std::vector<String> ALERT_URGENCY = {"minor", "moderate", "severe", "extreme"}; // METEO
// const std::vector<String> ALERT_URGENCY = {}; // Disable urgency interpretation (algorithm will fallback to only prefer the most recently issued alerts)

// ALERT TERMINOLOGY
// Weather terminology associated with each alert icon
const std::vector<String> TERM_SMOG =
    {"smog"};
const std::vector<String> TERM_SMOKE =
    {"smoke"};
const std::vector<String> TERM_FOG =
    {"fog", "haar"};
const std::vector<String> TERM_METEOR =
    {"meteor", "asteroid"};
const std::vector<String> TERM_NUCLEAR =
    {"nuclear", "ionizing radiation"};
const std::vector<String> TERM_BIOHAZARD =
    {"biohazard", "biological hazard"};
const std::vector<String> TERM_EARTHQUAKE =
    {"earthquake"};
const std::vector<String> TERM_TSUNAMI =
    {"tsunami"};
const std::vector<String> TERM_FIRE =
    {"fire", "red flag"};
const std::vector<String> TERM_THERMOMETER_HOT =
    {"heat"};
const std::vector<String> TERM_SNOWFLAKE_COLD =
    {"blizzard", "winter", "ice", "snow", "sleet", "cold", "freezing rain", 
     "wind chill", "freeze", "frost", "hail"};
const std::vector<String> TERM_LIGHTNING =
    {"thunderstorm", "storm cell", "pulse storm", "squall line", "supercells",
     "lightning"};
const std::vector<String> TERM_SANDSTORM =
    {"sandstorm", "blowing dust", "dust storm"};
const std::vector<String> TERM_FLOOD =
    {"flood", "storm surge", "seiche", "swell", "high seas", "high tides",
     "tidal surge"};
const std::vector<String> TERM_VOLCANO =
    {"volcanic", "ash", "volcano", "eruption"};
const std::vector<String> TERM_AIR_QUALITY =
    {"air", "stagnation", "pollution"};
const std::vector<String> TERM_TORNADO =
    {"tornado"};
const std::vector<String> TERM_SMALL_CRAFT_ADVISORY =
    {"small craft", "wind advisory"};
const std::vector<String> TERM_GALE_WARNING =
    {"gale"};
const std::vector<String> TERM_STORM_WARNING =
    {"storm warning"};
const std::vector<String> TERM_HURRICANE_WARNING =
    {"hurricane force wind", "extreme wind", "high wind"};
const std::vector<String> TERM_HURRICANE =
    {"hurricane", "tropical storm", "typhoon", "cyclone"};
const std::vector<String> TERM_DUST =
    {"dust", "sand"};
const std::vector<String> TERM_STRONG_WIND =
    {"wind"};
