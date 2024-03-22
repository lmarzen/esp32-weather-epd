/* AQI library declarations for pollutant-concentration-to-aqi.
 * Copyright (C) 2022-2024  Luke Marzen
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

#ifndef __AQI_H__
#define __AQI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Each AQI scale has a maximum value, above which AQI is typically denoted by
 * ">{AQI_MAX}" or "{AQI_MAX}+".
 */
#define AUSTRALIA_AQI_MAX       200
#define CANADA_AQHI_MAX          10
#define EUROPE_CAQI_MAX         100
#define HONG_KONG_AQHI_MAX       10
#define INDIA_AQI_MAX           400
#define MAINLAND_CHINA_AQI_MAX  500
#define SINGAPORE_PSI_MAX       500
#define SOUTH_KOREA_CAI_MAX     500
#define UNITED_KINGDOM_DAQI_MAX  10
#define UNITED_STATES_AQI_MAX   500

/* Returns the Air Quality Index, rounded to the nearest integer
 *
 * All pollutants will be interpreted as a concentration, with units μg/m^3.
 * Pollutants:
 *   co    μg/m^3, Carbon Monoxide (CO)               1 ppb = 1.1456 μg/m^3
 *   nh3   μg/m^3, Ammonia (NH3)                      1 ppb = 0.6966 μg/m^3
 *   no    μg/m^3, Nitric Oxide (NO)                  1 ppb = 1.2274 μg/m^3
 *   no2   μg/m^3, Nitrogen Dioxide (NO2)             1 ppb = 1.8816 μg/m^3
 *   o3    μg/m^3, Ozone (O3)                         1 ppb = 1.9632 μg/m^3
 *   pb    μg/m^3, Lead (Pb)                          1 ppb = 1.9632 μg/m^3
 *   so2   μg/m^3, Sulfur Dioxide (SO2)               1 ppb = 8.4744 μg/m^3
 *   pm10  μg/m^3, Coarse Particulate Matter (<10μm)
 *   pm2_5 μg/m^3, Fine Particulate Matter (<2.5μm)
 * Note: Concentration (µg/m^3) = molecular weight * concentration (ppb) / 24.45
 *
 * Most scales require that samples are averaged over a specified period.
 * The parameters denote what time period samples should be averaged over with
 * '_Xh', where 'X' is the time period samples are averaged over in hours.
 * (or '_Xmin' for minutes)
 *
 * Ex: 'co_8h' is interpreted as, the 8 hour moving average concentration of
 *             Carbon Monoxide.
 *
 * Useful websites with more information about various aqi scales:
 * https://en.wikipedia.org/wiki/Air_quality_index
 * https://atmotube.com/blog/standards-for-air-quality-indices-in-different-countries-aqi
 */

int australia_aqi(float co_8h,  float no2_1h,   float o3_1h, float o3_4h,
                  float so2_1h, float pm10_24h, float pm2_5_24h);

int canada_aqhi(float no2_3h, float o3_3h, float pm2_5_3h);

int europe_caqi(float no2_1h, float o3_1h, float pm10_1h, float pm2_5_1h);

int hong_kong_aqhi(float no2_3h,  float o3_3h, float so2_3h,
                   float pm10_3h, float pm2_5_3h);

int india_aqi(float co_8h,  float nh3_24h, float no2_24h,  float o3_8h,
              float pb_24h, float so2_24h, float pm10_24h, float pm2_5_24h);

int mainland_china_aqi(float co_1h, float co_24h, float no2_1h, float no2_24h,
                       float o3_1h, float o3_8h,  float so2_1h, float so2_24h,
                       float pm10_24h, float pm2_5_24h);

int singapore_psi(float co_8h,   float no2_1h,   float o3_1h, float o3_8h,
                  float so2_24h, float pm10_24h, float pm2_5_24h);

int south_korea_cai(float co_1h,  float no2_1h,   float o3_1h,
                    float so2_1h, float pm10_24h, float pm2_5_24h);

int united_kingdom_daqi(float no2_1h,   float o3_8h, float so2_15min,
                        float pm10_24h, float pm2_5_24h);

int united_states_aqi(float co_8h,    float no2_1h,
                      float o3_1h,    float o3_8h,
                      float so2_1h,   float so2_24h,
                      float pm10_24h, float pm2_5_24h);

/* Returns the descriptor/category of an aqi value.
 *
 * Usage Example:
 *   united_states_aqi_desc(52);
 *   returns "Moderate"
 */

const char *australia_aqi_desc(      int aqi);
const char *canada_aqhi_desc(        int aqhi);
const char *europe_caqi_desc(        int caqi);
const char *hong_kong_aqhi_desc(     int aqhi);
const char *india_aqi_desc(          int aqi);
const char *mainland_china_aqi_desc( int aqi);
const char *singapore_psi_desc(      int psi);
const char *south_korea_cai_desc(    int cai);
const char *united_kingdom_daqi_desc(int daqi);
const char *united_states_aqi_desc(  int aqi);


/* If you do not want to use the default descriptors below can define the
 * AQI_EXTERN_TXT macro below and define them elsewhere.
 */
#define AQI_EXTERN_TXT

#ifndef AQI_EXTERN_TXT
static const char *AUSTRALIA_AQI_TXT[6] =
{
  "Very Good",
  "Good",
  "Fair",
  "Poor",
  "Very Poor",
  "Hazardous",
};
static const char *CANADA_AQHI_TXT[4] =
{
  "Low",
  "Moderate",
  "High",
  "Very High",
};
static const char *EUROPE_CAQI_TXT[5] =
{
  "Very Low",
  "Low",
  "Medium",
  "High",
  "Very High",
};
static const char *HONG_KONG_AQHI_TXT[5] =
{
  "Low",
  "Moderate",
  "High",
  "Very High",
  "Hazardous",
};
static const char *INDIA_AQI_TXT[6] =
{
  "Good",
  "Satisfactory",
  "Moderate",
  "Poor",
  "Very Poor",
  "Severe",
};
static const char *MAINLAND_CHINA_AQI_TXT[6] =
{
  "Excellent",
  "Good",
  "Lightly Polluted",
  "Moderately Polluted",
  "Heavily Polluted",
  "Severely Polluted",
};
static const char *SINGAPORE_PSI_TXT[5] =
{
  "Good",
  "Moderate",
  "Unhealthy",
  "Very Unhealthy",
  "Hazardous",
};
static const char *SOUTH_KOREA_CAI_TXT[4] =
{
  "Good",
  "Medium",
  "Unhealthy",
  "Very Unhealthy",
};
static const char *UNITED_KINGDOM_DAQI_TXT[4] =
{
  "Low",
  "Moderate",
  "High",
  "Very High",
};
static const char *UNITED_STATES_AQI_TXT[6] =
{
  "Good",
  "Moderate",
  "Unhealthy for Sensitive Groups",
  "Unhealthy",
  "Very Unhealthy",
  "Hazardous",
};
#else
extern const char *AUSTRALIA_AQI_TXT[6];
extern const char *CANADA_AQHI_TXT[4];
extern const char *EUROPE_CAQI_TXT[5];
extern const char *HONG_KONG_AQHI_TXT[5];
extern const char *INDIA_AQI_TXT[6];
extern const char *MAINLAND_CHINA_AQI_TXT[6];
extern const char *SINGAPORE_PSI_TXT[5];
extern const char *SOUTH_KOREA_CAI_TXT[4];
extern const char *UNITED_KINGDOM_DAQI_TXT[4];
extern const char *UNITED_STATES_AQI_TXT[6];
#endif // AQI_EXTERN_TXT

#ifdef __cplusplus
}
#endif

#endif
