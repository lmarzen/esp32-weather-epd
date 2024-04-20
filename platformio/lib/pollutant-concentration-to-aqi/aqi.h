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

typedef enum {
  AUSTRALIA_AQI,
  CANADA_AQHI,
  CHINA_AQI,
  EUROPEAN_UNION_CAQI,
  HONG_KONG_AQHI,
  INDIA_AQI,
  SINGAPORE_PSI,
  SOUTH_KOREA_CAI,
  UNITED_KINGDOM_DAQI,
  UNITED_STATES_AQI,
  NUM_AQI_SCALES
} aqi_scale_t;

typedef enum {
  AIR_QUALITY_DESC,
  AIR_POLLUTION_DESC,
} aqi_desc_type_t;


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

int china_aqi(float co_1h,    float co_24h, float no2_1h, float no2_24h,
              float o3_1h,    float o3_8h,  float so2_1h, float so2_24h,
              float pm10_24h, float pm2_5_24h);

int european_union_caqi(float no2_1h, float o3_1h, float pm10_1h, float pm2_5_1h);

int hong_kong_aqhi(float no2_3h,  float o3_3h, float so2_3h,
                   float pm10_3h, float pm2_5_3h);

int india_aqi(float co_8h,  float nh3_24h, float no2_24h,  float o3_8h,
              float pb_24h, float so2_24h, float pm10_24h, float pm2_5_24h);


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

/* Given an hourly pollutant concentration samples, will return the Air Quality
 * Index, rounded to the nearest integer. The array of pollutant concentration
 * samples should be organized from least recent (index 0) to most recent
 * (index 23). This is important when calculating average pollutant
 * concentrations for scales that require the most recent X hours of samples.
 *
 * Pass NULL (or an array of 0's) to indicate that a concentration is not
 * available.
 *
 * Warning: The United Kingdom DAQI requires a 15min average concentration for
 *          so2. The UK's DAQI is the only scale that requires a 15min average
 *          concentration. (why!?) In this case the most recent hourly
 *          concentration will be used instead.
 */
int calc_australia_aqi(
             const float co[24],  const float nh3[24],  const float no[24],
             const float no2[24], const float o3[24],   const float pb[24],
             const float so2[24], const float pm10[24], const float pm2_5[24]);
int calc_canada_aqhi(
             const float co[24],  const float nh3[24],  const float no[24],
             const float no2[24], const float o3[24],   const float pb[24],
             const float so2[24], const float pm10[24], const float pm2_5[24]);
int calc_china_aqi(
             const float co[24],  const float nh3[24],  const float no[24],
             const float no2[24], const float o3[24],   const float pb[24],
             const float so2[24], const float pm10[24], const float pm2_5[24]);
int calc_european_union_caqi(
             const float co[24],  const float nh3[24],  const float no[24],
             const float no2[24], const float o3[24],   const float pb[24],
             const float so2[24], const float pm10[24], const float pm2_5[24]);
int calc_hong_kong_aqhi(
             const float co[24],  const float nh3[24],  const float no[24],
             const float no2[24], const float o3[24],   const float pb[24],
             const float so2[24], const float pm10[24], const float pm2_5[24]);
int calc_india_aqi(
             const float co[24],  const float nh3[24],  const float no[24],
             const float no2[24], const float o3[24],   const float pb[24],
             const float so2[24], const float pm10[24], const float pm2_5[24]);
int calc_singapore_psi(
             const float co[24],  const float nh3[24],  const float no[24],
             const float no2[24], const float o3[24],   const float pb[24],
             const float so2[24], const float pm10[24], const float pm2_5[24]);
int calc_south_korea_cai(
             const float co[24],  const float nh3[24],  const float no[24],
             const float no2[24], const float o3[24],   const float pb[24],
             const float so2[24], const float pm10[24], const float pm2_5[24]);
int calc_united_kingdom_daqi(
             const float co[24],  const float nh3[24],  const float no[24],
             const float no2[24], const float o3[24],   const float pb[24],
             const float so2[24], const float pm10[24], const float pm2_5[24]);
int calc_united_states_aqi(
             const float co[24],  const float nh3[24],  const float no[24],
             const float no2[24], const float o3[24],   const float pb[24],
             const float so2[24], const float pm10[24], const float pm2_5[24]);

/* Given a scale and hourly pollutant concentrations returns the Air Quality
 * Index.
 */
int calc_aqi(aqi_scale_t scale,
             const float co[24],  const float nh3[24],  const float no[24],
             const float no2[24], const float o3[24],   const float pb[24],
             const float so2[24], const float pm10[24], const float pm2_5[24]);

/* Each AQI scale has a maximum value, above which AQI is typically denoted by
 * ">{AQI_MAX}" or "{AQI_MAX}+".
 */
#define AUSTRALIA_AQI_MAX       200
#define CANADA_AQHI_MAX          10
#define CHINA_AQI_MAX           500
#define EUROPEAN_UNION_CAQI_MAX 100
#define HONG_KONG_AQHI_MAX       10
#define INDIA_AQI_MAX           400
#define SINGAPORE_PSI_MAX       500
#define SOUTH_KOREA_CAI_MAX     500
#define UNITED_KINGDOM_DAQI_MAX  10
#define UNITED_STATES_AQI_MAX   500

/* Returns the maximum value for the given AQI scale.
 */
int aqi_scale_max(aqi_scale_t scale);

/* Returns the descriptor/category of an AQI value.
 *
 * Usage Example:
 *   united_states_aqi_desc(52);
 *   returns "Moderate"
 */
const char *australia_aqi_desc(      int aqi);
const char *canada_aqhi_desc(        int aqhi);
const char *china_aqi_desc(          int aqi);
const char *european_union_caqi_desc(int caqi);
const char *hong_kong_aqhi_desc(     int aqhi);
const char *india_aqi_desc(          int aqi);
const char *singapore_psi_desc(      int psi);
const char *south_korea_cai_desc(    int cai);
const char *united_kingdom_daqi_desc(int daqi);
const char *united_states_aqi_desc(  int aqi);

/* Given an AQI scale and an index value, returns a pointer the corresponding
 * descriptor.
 */
const char *aqi_desc(aqi_scale_t scale, int val);

/* The descriptors for an AQI scale generally describe either
 *   (0) Air Quality
 * or
 *   (1) Air Pollution
 */
#define AUSTRALIA_AQI_DESC_TYPE       AIR_QUALITY_DESC
#define CANADA_AQHI_DESC_TYPE         AIR_POLLUTION_DESC
#define CHINA_AQI_DESC_TYPE           AIR_QUALITY_DESC
#define EUROPEAN_UNION_CAQI_DESC_TYPE AIR_POLLUTION_DESC
#define HONG_KONG_AQHI_DESC_TYPE      AIR_POLLUTION_DESC
#define INDIA_AQI_DESC_TYPE           AIR_QUALITY_DESC
#define SINGAPORE_PSI_DESC_TYPE       AIR_QUALITY_DESC
#define SOUTH_KOREA_CAI_DESC_TYPE     AIR_QUALITY_DESC
#define UNITED_KINGDOM_DAQI_DESC_TYPE AIR_POLLUTION_DESC
#define UNITED_STATES_AQI_DESC_TYPE   AIR_QUALITY_DESC

/* Given a AQI scale, returns what the descriptor text indicates air quality or
 * air pollution.
 */
aqi_desc_type_t aqi_desc_type(aqi_scale_t scale);

/* If you do not want to use the default descriptors, you may define the
 * AQI_EXTERN_TXT macro below and define the descriptor strings externally.
 */
#define AQI_EXTERN_TXT


#ifdef __cplusplus
}
#endif

#endif
