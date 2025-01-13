/* AQI library definitions for pollutant-concentration-to-aqi.
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

#include "aqi.h"
#include <math.h>
#include <stddef.h>

#ifndef AQI_EXTERN_TXT
const char *AUSTRALIA_AQI_TXT[6] =
{
  "Very Good",
  "Good",
  "Fair",
  "Poor",
  "Very Poor",
  "Hazardous",
};
const char *CANADA_AQHI_TXT[4] =
{
  "Low",
  "Moderate",
  "High",
  "Very High",
};
const char *CHINA_AQI_TXT[6] =
{
  "Excellent",
  "Good",
  "Lightly Polluted",
  "Moderately Polluted",
  "Heavily Polluted",
  "Severely Polluted",
};
const char *EUROPEAN_UNION_CAQI_TXT[5] =
{
  "Very Low",
  "Low",
  "Medium",
  "High",
  "Very High",
};
const char *HONG_KONG_AQHI_TXT[5] =
{
  "Low",
  "Moderate",
  "High",
  "Very High",
  "Hazardous",
};
const char *INDIA_AQI_TXT[6] =
{
  "Good",
  "Satisfactory",
  "Moderate",
  "Poor",
  "Very Poor",
  "Severe",
};
const char *SINGAPORE_PSI_TXT[5] =
{
  "Good",
  "Moderate",
  "Unhealthy",
  "Very Unhealthy",
  "Hazardous",
};
const char *SOUTH_KOREA_CAI_TXT[4] =
{
  "Good",
  "Medium",
  "Unhealthy",
  "Very Unhealthy",
};
const char *UNITED_KINGDOM_DAQI_TXT[4] =
{
  "Low",
  "Moderate",
  "High",
  "Very High",
};
const char *UNITED_STATES_AQI_TXT[6] =
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
extern const char *CHINA_AQI_TXT[6];
extern const char *EUROPEAN_UNION_CAQI_TXT[5];
extern const char *HONG_KONG_AQHI_TXT[5];
extern const char *INDIA_AQI_TXT[6];
extern const char *SINGAPORE_PSI_TXT[5];
extern const char *SOUTH_KOREA_CAI_TXT[4];
extern const char *UNITED_KINGDOM_DAQI_TXT[4];
extern const char *UNITED_STATES_AQI_TXT[6];
#endif // AQI_EXTERN_TXT

int max(int a, int b) { return a >= b ? a : b; }
int min(int a, int b) { return a <= b ? a : b; }

float truncate_float(float val, int decimal_places)
{
  int n = pow(10, decimal_places);
  return floorf(val * n) / n;
} // end truncate_float

int compute_nepm_aqi(float std, float c)
{
  return (int)round(c / std * 100);
} // end compute_nepm_aqi

int compute_piecewise_aqi(float i_lo, float i_hi,
                          float c_lo, float c_hi, float c)
{
  return min(i_hi, max(i_lo, round(
                             ( ((float)(i_hi - i_lo)) / ((float)(c_hi - c_lo)) )
                             * (c - c_lo) + i_lo)));
} // end compute_piecewise_aqi

/* Australia (AQI)
 *
 * References:
 *   https://www.environment.nsw.gov.au/topics/air/understanding-air-quality-data/air-quality-categories/history-of-air-quality-reporting/about-the-air-quality-index
 */
int australia_aqi(float co_8h,  float no2_1h,   float o3_1h, float o3_4h,
                  float so2_1h, float pm10_24h, float pm2_5_24h)
{
  int aqi = 0;

  // co    μg/m^3, Carbon Monoxide (CO)
  // standard = 9.0ppm * 1000ppb * 1.1456 μg/m^3 = 10310.4
  aqi = max(aqi, compute_nepm_aqi(10310.4, co_8h));
  // no2   μg/m^3, Nitrogen Dioxide (NO2)
  // standard = 0.12ppm * 1000ppb * 1.8816 μg/m^3 = 225.792
  aqi = max(aqi, compute_nepm_aqi(225.792, no2_1h));
  // o3    μg/m^3, Ground-Level Ozone (O3)
  // standard = 0.10ppm * 1000ppb * 1.9632 μg/m^3 = 196.32
  aqi = max(aqi, compute_nepm_aqi(196.32, o3_1h));
  // standard = 0.08ppm * 1000ppb * 1.9632 μg/m^3 = 157.056
  aqi = max(aqi, compute_nepm_aqi(157.056, o3_4h));
  // so2   μg/m^3, Sulfur Dioxide (SO2)
  // standard = 0.20ppm * 1000ppb * 8.4744 μg/m^3 = 1694.88
  aqi = max(aqi, compute_nepm_aqi(1694.88, so2_1h));
  // pm10  μg/m^3, Coarse Particulate Matter (<10μm)
  aqi = max(aqi, compute_nepm_aqi(50, pm10_24h));
  // pm2_5 μg/m^3, Fine Particulate Matter (<2.5μm)
  aqi = max(aqi, compute_nepm_aqi(25, pm2_5_24h));

  return aqi;
} // end australia_aqi

/* Canada (AQHI)
 *
 * References:
 *   https://en.wikipedia.org/wiki/Air_Quality_Health_Index_(Canada)
 */
int canada_aqhi(float no2_3h, float o3_3h, float pm2_5_3h)
{
  return max(1, (int)round(
                    (1000 / 10.4) * ((exp(0.000273533 * o3_3h) - 1)    // 0.000537 * 1ppb/1.9632 μg/m^3 = 0.000273533
                                     + (exp(0.000462904 * no2_3h) - 1) // 0.000871 * 1ppb/1.8816 μg/m^3 = 0.000462904
                                     + (exp(0.000487 * pm2_5_3h) - 1))));
} // end canada_aqhi

/* China (AQI)
 *
 * References:
 *   https://web.archive.org/web/20180830110324/http://kjs.mep.gov.cn/hjbhbz/bzwb/jcffbz/201203/W020120410332725219541.pdf
 *   https://en.wikipedia.org/wiki/Air_quality_index#Mainland_China
 *   https://datadrivenlab.org/air-quality-2/chinas-new-air-quality-index-how-does-it-measure-up/
 */
int china_aqi(float co_1h, float co_24h, float no2_1h, float no2_24h,
              float o3_1h, float o3_8h,  float so2_1h, float so2_24h,
              float pm10_24h, float pm2_5_24h)
{
  int aqi = 0;
  float i_lo, i_hi;
  float c_lo, c_hi;

  // co    μg/m^3, Carbon Monoxide (CO)
  // 1mg/m^3 = 1000 μg/m^3
  if (co_1h <= 5000)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 5000;
  }
  else if (co_1h <= 10000)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 5000;
    c_hi = 10000;
  }
  else if (co_1h <= 35000)
  {
    i_lo = 101;
    i_hi = 150;
    c_lo = 10000;
    c_hi = 35000;
  }
  else if (co_1h <= 60000)
  {
    i_lo = 151;
    i_hi = 200;
    c_lo = 35000;
    c_hi = 60000;
  }
  else if (co_1h <= 90000)
  {
    i_lo = 201;
    i_hi = 300;
    c_lo = 60000;
    c_hi = 90000;
  }
  else if (co_1h <= 120000)
  {
    i_lo = 301;
    i_hi = 400;
    c_lo = 90000;
    c_hi = 120000;
  }
  else if (co_1h <= 150000)
  {
    i_lo = 401;
    i_hi = 500;
    c_lo = 120000;
    c_hi = 150000;
  }
  else
  {
    // index > 500
    return 501;
  }
  aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, co_1h));

  if (co_24h <= 2000)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 2000;
  }
  else if (co_24h <= 4000)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 2000;
    c_hi = 4000;
  }
  else if (co_24h <= 14000)
  {
    i_lo = 101;
    i_hi = 150;
    c_lo = 4000;
    c_hi = 14000;
  }
  else if (co_24h <= 24000)
  {
    i_lo = 151;
    i_hi = 200;
    c_lo = 14000;
    c_hi = 24000;
  }
  else if (co_24h <= 36000)
  {
    i_lo = 201;
    i_hi = 300;
    c_lo = 24000;
    c_hi = 36000;
  }
  else if (co_24h <= 48000)
  {
    i_lo = 301;
    i_hi = 400;
    c_lo = 36000;
    c_hi = 48000;
  }
  else if (co_24h <= 60000)
  {
    i_lo = 401;
    i_hi = 500;
    c_lo = 48000;
    c_hi = 60000;
  }
  else
  {
    // index > 500
    return 501;
  }
  aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, co_24h));

  // no2   μg/m^3, Nitrogen Dioxide (NO2)
  if (no2_1h <= 100)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 100;
  }
  else if (no2_1h <= 200)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 100;
    c_hi = 200;
  }
  else if (no2_1h <= 700)
  {
    i_lo = 101;
    i_hi = 150;
    c_lo = 200;
    c_hi = 700;
  }
  else if (no2_1h <= 1200)
  {
    i_lo = 151;
    i_hi = 200;
    c_lo = 700;
    c_hi = 1200;
  }
  else if (no2_1h <= 2340)
  {
    i_lo = 201;
    i_hi = 300;
    c_lo = 1200;
    c_hi = 2340;
  }
  else if (no2_1h <= 3090)
  {
    i_lo = 301;
    i_hi = 400;
    c_lo = 2340;
    c_hi = 3090;
  }
  else if (no2_1h <= 3840)
  {
    i_lo = 401;
    i_hi = 500;
    c_lo = 3090;
    c_hi = 3840;
  }
  else
  {
    // index > 500
    return 501;
  }
  aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, no2_1h));

  if (no2_24h <= 40)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 40;
  }
  else if (no2_24h <= 80)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 40;
    c_hi = 80;
  }
  else if (no2_24h <= 180)
  {
    i_lo = 101;
    i_hi = 150;
    c_lo = 80;
    c_hi = 180;
  }
  else if (no2_24h <= 280)
  {
    i_lo = 151;
    i_hi = 200;
    c_lo = 180;
    c_hi = 280;
  }
  else if (no2_24h <= 565)
  {
    i_lo = 201;
    i_hi = 300;
    c_lo = 280;
    c_hi = 565;
  }
  else if (no2_24h <= 750)
  {
    i_lo = 301;
    i_hi = 400;
    c_lo = 565;
    c_hi = 750;
  }
  else if (no2_24h <= 940)
  {
    i_lo = 401;
    i_hi = 500;
    c_lo = 750;
    c_hi = 940;
  }
  else
  {
    // index > 500
    return 501;
  }
  aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, no2_24h));

  // o3    μg/m^3, Ozone (O3)
  if (o3_1h <= 160)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 160;
  }
  else if (o3_1h <= 200)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 160;
    c_hi = 200;
  }
  else if (o3_1h <= 300)
  {
    i_lo = 101;
    i_hi = 150;
    c_lo = 200;
    c_hi = 300;
  }
  else if (o3_1h <= 400)
  {
    i_lo = 151;
    i_hi = 200;
    c_lo = 300;
    c_hi = 400;
  }
  else if (o3_1h <= 800)
  {
    i_lo = 201;
    i_hi = 300;
    c_lo = 400;
    c_hi = 800;
  }
  else if (o3_1h <= 1000)
  {
    i_lo = 301;
    i_hi = 400;
    c_lo = 800;
    c_hi = 1000;
  }
  else if (o3_1h <= 1200)
  {
    i_lo = 401;
    i_hi = 500;
    c_lo = 1000;
    c_hi = 1200;
  }
  else
  {
    // index > 500
    return 501;
  }
  aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, o3_1h));

  // If 8 hour average of o3 is > 800 μg/m^3 don't calculate it.
  if (o3_8h <= 800)
  {
    if (o3_8h <= 100)
    {
      i_lo = 0;
      i_hi = 50;
      c_lo = 0;
      c_hi = 100;
    }
    else if (o3_8h <= 160)
    {
      i_lo = 51;
      i_hi = 100;
      c_lo = 100;
      c_hi = 160;
    }
    else if (o3_8h <= 215)
    {
      i_lo = 101;
      i_hi = 150;
      c_lo = 160;
      c_hi = 215;
    }
    else if (o3_8h <= 265)
    {
      i_lo = 151;
      i_hi = 200;
      c_lo = 215;
      c_hi = 265;
    }
    else
    {
      // 265 < o3_8h <= 800
      i_lo = 201;
      i_hi = 300;
      c_lo = 265;
      c_hi = 800;
    }
    aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, o3_8h));
  }

  // so2   μg/m^3, Sulfur Dioxide (SO2)
  // If 1 hour average of so2 is > 800 μg/m^3 don't calculate it.
  if (so2_1h <= 800)
  {
    if (so2_1h <= 150)
    {
      i_lo = 0;
      i_hi = 50;
      c_lo = 0;
      c_hi = 150;
    }
    else if (so2_1h <= 500)
    {
      i_lo = 51;
      i_hi = 100;
      c_lo = 150;
      c_hi = 500;
    }
    else if (so2_1h <= 650)
    {
      i_lo = 101;
      i_hi = 150;
      c_lo = 500;
      c_hi = 650;
    }
    else
    {
      // 650 < so2_1h <= 800
      i_lo = 151;
      i_hi = 200;
      c_lo = 650;
      c_hi = 800;
    }
    aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, so2_1h));
  }

  if (so2_24h <= 50)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 50;
  }
  else if (so2_24h <= 150)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 50;
    c_hi = 150;
  }
  else if (so2_24h <= 475)
  {
    i_lo = 101;
    i_hi = 150;
    c_lo = 150;
    c_hi = 475;
  }
  else if (so2_24h <= 800)
  {
    i_lo = 151;
    i_hi = 200;
    c_lo = 475;
    c_hi = 800;
  }
  else if (so2_24h <= 1600)
  {
    i_lo = 201;
    i_hi = 300;
    c_lo = 800;
    c_hi = 1600;
  }
  else if (so2_24h <= 2100)
  {
    i_lo = 301;
    i_hi = 400;
    c_lo = 1600;
    c_hi = 2100;
  }
  else if (so2_24h <= 2620)
  {
    i_lo = 401;
    i_hi = 500;
    c_lo = 2100;
    c_hi = 2620;
  }
  else
  {
    // index > 500
    return 501;
  }
  aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, so2_24h));

  // pm10  μg/m^3, Coarse Particulate Matter (<10μm)
  if (pm10_24h <= 50)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 50;
  }
  else if (pm10_24h <= 150)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 50;
    c_hi = 150;
  }
  else if (pm10_24h <= 250)
  {
    i_lo = 101;
    i_hi = 150;
    c_lo = 150;
    c_hi = 250;
  }
  else if (pm10_24h <= 350)
  {
    i_lo = 151;
    i_hi = 200;
    c_lo = 250;
    c_hi = 350;
  }
  else if (pm10_24h <= 420)
  {
    i_lo = 201;
    i_hi = 300;
    c_lo = 350;
    c_hi = 420;
  }
  else if (pm10_24h <= 500)
  {
    i_lo = 301;
    i_hi = 400;
    c_lo = 420;
    c_hi = 500;
  }
  else if (pm10_24h <= 600)
  {
    i_lo = 401;
    i_hi = 500;
    c_lo = 500;
    c_hi = 600;
  }
  else
  {
    // index > 500
    return 501;
  }
  aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, pm10_24h));

  // pm2_5 μg/m^3, Fine Particulate Matter (<2.5μm)
  if (pm2_5_24h <= 35)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 35;
  }
  else if (pm2_5_24h <= 75)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 35;
    c_hi = 75;
  }
  else if (pm2_5_24h <= 115)
  {
    i_lo = 101;
    i_hi = 150;
    c_lo = 75;
    c_hi = 115;
  }
  else if (pm2_5_24h <= 150)
  {
    i_lo = 151;
    i_hi = 200;
    c_lo = 115;
    c_hi = 150;
  }
  else if (pm2_5_24h <= 250)
  {
    i_lo = 201;
    i_hi = 300;
    c_lo = 150;
    c_hi = 250;
  }
  else if (pm2_5_24h <= 350)
  {
    i_lo = 301;
    i_hi = 400;
    c_lo = 250;
    c_hi = 350;
  }
  else if (pm2_5_24h <= 500)
  {
    i_lo = 401;
    i_hi = 500;
    c_lo = 350;
    c_hi = 500;
  }
  else
  {
    // index > 500
    return 501;
  }
  aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, pm2_5_24h));

  return aqi;
} // end china_aqi

/* European Union (CAQI)
 *
 * References:
 *   http://airqualitynow.eu/about_indices_definition.php
 *   https://en.wikipedia.org/wiki/Air_quality_index#CAQI
 */
int european_union_caqi(float no2_1h, float o3_1h, float pm10_1h, float pm2_5_1h)
{
  int caqi = 0;
  float i_lo, i_hi;
  float c_lo, c_hi;

  // no2   μg/m^3, Nitrogen Dioxide (NO2)
  if (no2_1h <= 50)
  {
    i_lo = 0;
    i_hi = 25;
    c_lo = 0;
    c_hi = 50;
  }
  else if (no2_1h <= 100)
  {
    i_lo = 26;
    i_hi = 50;
    c_lo = 50;
    c_hi = 100;
  }
  else if (no2_1h <= 200)
  {
    i_lo = 51;
    i_hi = 75;
    c_lo = 100;
    c_hi = 200;
  }
  else if (no2_1h <= 400)
  {
    i_lo = 76;
    i_hi = 100;
    c_lo = 200;
    c_hi = 400;
  }
  else
  {
    // index > 100
    return 101;
  }
  caqi = max(caqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, no2_1h));

  // o3    μg/m^3, Ground-Level Ozone (O3)
  if (o3_1h <= 60)
  {
    i_lo = 0;
    i_hi = 25;
    c_lo = 0;
    c_hi = 60;
  }
  else if (o3_1h <= 120)
  {
    i_lo = 25;
    i_hi = 50;
    c_lo = 60;
    c_hi = 120;
  }
  else if (o3_1h <= 180)
  {
    i_lo = 51;
    i_hi = 75;
    c_lo = 120;
    c_hi = 180;
  }
  else if (o3_1h <= 240)
  {
    i_lo = 76;
    i_hi = 100;
    c_lo = 180;
    c_hi = 240;
  }
  else
  {
    // index > 100
    return 101;
  }
  caqi = max(caqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, o3_1h));

  // pm10  μg/m^3, Coarse Particulate Matter (<10μm)
  if (pm10_1h <= 25)
  {
    i_lo = 0;
    i_hi = 25;
    c_lo = 0;
    c_hi = 25;
  }
  else if (pm10_1h <= 50)
  {
    i_lo = 26;
    i_hi = 50;
    c_lo = 25;
    c_hi = 50;
  }
  else if (pm10_1h <= 90)
  {
    i_lo = 51;
    i_hi = 75;
    c_lo = 50;
    c_hi = 90;
  }
  else if (pm10_1h <= 180)
  {
    i_lo = 76;
    i_hi = 100;
    c_lo = 90;
    c_hi = 180;
  }
  else
  {
    // index > 100
    return 101;
  }
  caqi = max(caqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, pm10_1h));

  // pm2_5 μg/m^3, Fine Particulate Matter (<2.5μm)
  if (pm2_5_1h <= 15)
  {
    i_lo = 0;
    i_hi = 25;
    c_lo = 0;
    c_hi = 15;
  }
  else if (pm2_5_1h <= 30)
  {
    i_lo = 26;
    i_hi = 50;
    c_lo = 15;
    c_hi = 30;
  }
  else if (pm2_5_1h <= 55)
  {
    i_lo = 51;
    i_hi = 75;
    c_lo = 30;
    c_hi = 55;
  }
  else if (pm2_5_1h <= 110)
  {
    i_lo = 76;
    i_hi = 100;
    c_lo = 55;
    c_hi = 110;
  }
  else
  {
    // index > 100
    return 101;
  }
  caqi = max(caqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, pm2_5_1h));

  return caqi;
} // end european_union_caqi

/* Hong Kong (AQHI)
 *
 * References:
 *   https://www.aqhi.gov.hk/en/what-is-aqhi/faqs.html
 *   https://aqicn.org/faq/2015-06-03/overview-of-hong-kongs-air-quality-health-index/
 */
int hong_kong_aqhi(float no2_3h,  float o3_3h, float so2_3h,
                   float pm10_3h, float pm2_5_3h)
{
  float ar = ((exp(0.0004462559 * no2_3h) - 1) * 100) + ((exp(0.0001393235 * so2_3h) - 1) * 100) + ((exp(0.0005116328 * o3_3h) - 1) * 100) + fmax(((exp(0.0002821751 * pm10_3h) - 1) * 100), ((exp(0.0002180567 * pm2_5_3h) - 1) * 100));
  if (ar <= 1.88)
  {
    return 1;
  }
  else if (ar <= 3.76)
  {
    return 2;
  }
  else if (ar <= 5.64)
  {
    return 3;
  }
  else if (ar <= 7.52)
  {
    return 4;
  }
  else if (ar <= 9.41)
  {
    return 5;
  }
  else if (ar <= 11.29)
  {
    return 6;
  }
  else if (ar <= 12.91)
  {
    return 7;
  }
  else if (ar <= 15.07)
  {
    return 8;
  }
  else if (ar <= 17.22)
  {
    return 9;
  }
  else if (ar <= 19.37)
  {
    return 10;
  }
  else
  {
    // index > 10
    return 11;
  }
} // end hong_kong_aqhi

/* India (AQI)
 *
 * References:
 *   https://www.aqi.in/blog/aqi/
 *   https://www.pranaair.com/blog/what-is-air-quality-index-aqi-and-its-calculation/
 */
int india_aqi(float co_8h,  float nh3_24h, float no2_24h,  float o3_8h,
              float pb_24h, float so2_24h, float pm10_24h, float pm2_5_24h)
{
  int aqi = 0;
  float i_lo, i_hi;
  float c_lo, c_hi;

  // co    μg/m^3, Carbon Monoxide (CO)
  // 1mg/m^3 = 1000 μg/m^3
  if (co_8h < 1050)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 1000;
  }
  else if (co_8h < 2050)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 1100;
    c_hi = 2000;
  }
  else if (co_8h < 10050)
  {
    i_lo = 101;
    i_hi = 200;
    c_lo = 2100;
    c_hi = 10000;
  }
  else if (co_8h < 17050)
  {
    i_lo = 201;
    i_hi = 300;
    c_lo = 10100;
    c_hi = 17000;
  }
  else if (co_8h < 34050)
  {
    i_lo = 301;
    i_hi = 400;
    c_lo = 17100;
    c_hi = 34000;
  }
  else
  {
    // index > 400
    return 401;
  }
  aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, co_8h));

  // nh3   μg/m^3, Ammonia (NH3)
  if (nh3_24h < 200.5)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 200;
  }
  else if (nh3_24h < 400.5)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 201;
    c_hi = 400;
  }
  else if (nh3_24h < 800.5)
  {
    i_lo = 101;
    i_hi = 200;
    c_lo = 401;
    c_hi = 800;
  }
  else if (nh3_24h < 1200.5)
  {
    i_lo = 201;
    i_hi = 300;
    c_lo = 801;
    c_hi = 1200;
  }
  else if (nh3_24h < 1800.5)
  {
    i_lo = 301;
    i_hi = 400;
    c_lo = 1201;
    c_hi = 1800;
  }
  else
  {
    // index > 400
    return 401;
  }
  aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, nh3_24h));

  // no2   μg/m^3, Nitrogen Dioxide (NO2)
  if (no2_24h < 40.5)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 40;
  }
  else if (no2_24h < 80.5)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 41;
    c_hi = 80;
  }
  else if (no2_24h < 180.5)
  {
    i_lo = 101;
    i_hi = 200;
    c_lo = 81;
    c_hi = 180;
  }
  else if (no2_24h < 280.5)
  {
    i_lo = 201;
    i_hi = 300;
    c_lo = 181;
    c_hi = 280;
  }
  else if (no2_24h < 400.5)
  {
    i_lo = 301;
    i_hi = 400;
    c_lo = 281;
    c_hi = 400;
  }
  else
  {
    // index > 400
    return 401;
  }
  aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, no2_24h));

  // o3    μg/m^3, Ozone (O3)
  if (o3_8h < 50.5)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 50;
  }
  else if (o3_8h < 100.5)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 51;
    c_hi = 100;
  }
  else if (o3_8h < 168.5)
  {
    i_lo = 101;
    i_hi = 200;
    c_lo = 101;
    c_hi = 168;
  }
  else if (o3_8h < 208.5)
  {
    i_lo = 201;
    i_hi = 300;
    c_lo = 169;
    c_hi = 208;
  }
  else if (o3_8h < 748.5)
  {
    i_lo = 301;
    i_hi = 400;
    c_lo = 209;
    c_hi = 748;
  }
  else
  {
    // index > 400
    return 401;
  }
  aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, o3_8h));

  // pb    μg/m^3, Lead (Pb)
  if (pb_24h < 0.55)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 0.5;
  }
  else if (pb_24h < 1.05)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 0.6;
    c_hi = 1.0;
  }
  else if (pb_24h < 2.05)
  {
    i_lo = 101;
    i_hi = 200;
    c_lo = 1.1;
    c_hi = 2.0;
  }
  else if (pb_24h < 3.05)
  {
    i_lo = 201;
    i_hi = 300;
    c_lo = 2.1;
    c_hi = 3.0;
  }
  else if (pb_24h < 3.55)
  {
    i_lo = 301;
    i_hi = 400;
    c_lo = 3.1;
    c_hi = 3.5;
  }
  else
  {
    // index > 400
    return 401;
  }
  aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, pb_24h));

  // so2   μg/m^3, Sulfur Dioxide (SO2)
  if (so2_24h < 40.5)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 40;
  }
  else if (so2_24h < 80.5)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 41;
    c_hi = 80;
  }
  else if (so2_24h < 380.5)
  {
    i_lo = 101;
    i_hi = 200;
    c_lo = 81;
    c_hi = 380;
  }
  else if (so2_24h < 800.5)
  {
    i_lo = 201;
    i_hi = 300;
    c_lo = 381;
    c_hi = 800;
  }
  else if (so2_24h < 1600.5)
  {
    i_lo = 301;
    i_hi = 400;
    c_lo = 801;
    c_hi = 1600;
  }
  else
  {
    // index > 400
    return 401;
  }
  aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, so2_24h));

  // pm10  μg/m^3, Coarse Particulate Matter (<10μm)
  if (pm10_24h < 50.5)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 50;
  }
  else if (pm10_24h < 100.5)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 51;
    c_hi = 100;
  }
  else if (pm10_24h < 250.5)
  {
    i_lo = 101;
    i_hi = 200;
    c_lo = 101;
    c_hi = 250;
  }
  else if (pm10_24h < 350.5)
  {
    i_lo = 201;
    i_hi = 300;
    c_lo = 251;
    c_hi = 350;
  }
  else if (pm10_24h < 430.5)
  {
    i_lo = 301;
    i_hi = 400;
    c_lo = 351;
    c_hi = 430;
  }
  else
  {
    // index > 400
    return 401;
  }
  aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, pm10_24h));

  // pm2_5 μg/m^3, Fine Particulate Matter (<2.5μm)
  if (pm2_5_24h < 30.5)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 30;
  }
  else if (pm2_5_24h < 60.5)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 31;
    c_hi = 60;
  }
  else if (pm2_5_24h < 90.5)
  {
    i_lo = 101;
    i_hi = 200;
    c_lo = 61;
    c_hi = 90;
  }
  else if (pm2_5_24h < 120.5)
  {
    i_lo = 201;
    i_hi = 300;
    c_lo = 91;
    c_hi = 120;
  }
  else if (pm2_5_24h < 250.5)
  {
    i_lo = 301;
    i_hi = 400;
    c_lo = 121;
    c_hi = 250;
  }
  else
  {
    // index > 400
    return 401;
  }
  aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, pm2_5_24h));

  return aqi;
} // end india_aqi

/* Singapore (PSI)
 *
 * References:
 *   https://www.haze.gov.sg/
 *   http://www.haze.gov.sg/docs/default-source/faq/computation-of-the-pollutant-standards-index-%28psi%29.pdf
 */
int singapore_psi(float co_8h,   float no2_1h,   float o3_1h, float o3_8h,
                  float so2_24h, float pm10_24h, float pm2_5_24h)
{
  int psi = 0;
  float i_lo, i_hi;
  float c_lo, c_hi;

  // co    μg/m^3, Carbon Monoxide (CO)
  // 1mg/m^3 = 1000 μg/m^3
  if (co_8h < 5050)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 5000;
  }
  else if (co_8h < 10050)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 5100;
    c_hi = 10000;
  }
  else if (co_8h < 17050)
  {
    i_lo = 101;
    i_hi = 200;
    c_lo = 10100;
    c_hi = 17000;
  }
  else if (co_8h < 34050)
  {
    i_lo = 201;
    i_hi = 300;
    c_lo = 17100;
    c_hi = 34000;
  }
  else if (co_8h < 46050)
  {
    i_lo = 301;
    i_hi = 400;
    c_lo = 34100;
    c_hi = 46000;
  }
  else if (co_8h < 57550)
  {
    i_lo = 401;
    i_hi = 500;
    c_lo = 46100;
    c_hi = 57500;
  }
  else
  {
    // index > 500
    return 501;
  }
  psi = max(psi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, co_8h));

  // no2   μg/m^3, Nitrogen Dioxide (NO2)
  // only calculated if >= 1130 μg/m^3
  if (no2_1h >= 1129.5)
  {
    if (no2_1h < 2260.5)
    {
      i_lo = 201;
      i_hi = 300;
      c_lo = 1131;
      c_hi = 2260;
    }
    else if (no2_1h < 3000.5)
    {
      i_lo = 301;
      i_hi = 400;
      c_lo = 2261;
      c_hi = 3000;
    }
    else if (no2_1h < 3750.5)
    {
      i_lo = 401;
      i_hi = 500;
      c_lo = 3001;
      c_hi = 3750;
    }
    else
    {
      // index > 500
      return 501;
    }
    if (no2_1h >= 1129.5 && no2_1h < 1130.5)
    {
      psi = max(psi, 200);
    }
    else
    {
      psi = max(psi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, no2_1h));
    }
  }

  // o3    μg/m^3, Ozone (O3)
  // When 8-hour o3 concentration is > 785 μg/m^3, then the PSI sub-index is
  // calculated using the 1 hour concentration.
  if (o3_8h <= 785)
  {
    if (o3_8h < 118.5)
    {
      i_lo = 0;
      i_hi = 50;
      c_lo = 0;
      c_hi = 118;
    }
    else if (o3_8h < 157.5)
    {
      i_lo = 51;
      i_hi = 100;
      c_lo = 119;
      c_hi = 157;
    }
    else if (o3_8h < 235.5)
    {
      i_lo = 101;
      i_hi = 200;
      c_lo = 158;
      c_hi = 235;
    }
    else
    {
      // o3_8h <= 785
      i_lo = 201;
      i_hi = 300;
      c_lo = 236;
      c_hi = 785;
    }
    psi = max(psi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, o3_8h));
  }
  else
  {
    if (o3_1h < 118.5)
    {
      i_lo = 0;
      i_hi = 50;
      c_lo = 0;
      c_hi = 118;
    }
    else if (o3_1h < 157.5)
    {
      i_lo = 51;
      i_hi = 100;
      c_lo = 119;
      c_hi = 157;
    }
    else if (o3_1h < 235.5)
    {
      i_lo = 101;
      i_hi = 200;
      c_lo = 158;
      c_hi = 235;
    }
    else if (o3_1h < 785.5)
    {
      i_lo = 201;
      i_hi = 300;
      c_lo = 236;
      c_hi = 785;
    }
    else if (o3_1h < 980.5)
    {
      i_lo = 301;
      i_hi = 400;
      c_lo = 786;
      c_hi = 980;
    }
    else if (o3_1h < 1180.5)
    {
      i_lo = 401;
      i_hi = 500;
      c_lo = 981;
      c_hi = 1180;
    }
    else
    {
      // index > 500
      return 501;
    }
    psi = max(psi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, o3_1h));
  }

  // so2   μg/m^3, Sulfur Dioxide (SO2)
  if (so2_24h < 80.5)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 80;
  }
  else if (so2_24h < 365.5)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 81;
    c_hi = 365;
  }
  else if (so2_24h < 800.5)
  {
    i_lo = 101;
    i_hi = 200;
    c_lo = 366;
    c_hi = 800;
  }
  else if (so2_24h < 1600.5)
  {
    i_lo = 201;
    i_hi = 300;
    c_lo = 801;
    c_hi = 1600;
  }
  else if (so2_24h < 2100.5)
  {
    i_lo = 301;
    i_hi = 400;
    c_lo = 1601;
    c_hi = 2100;
  }
  else if (so2_24h < 2620.5)
  {
    i_lo = 401;
    i_hi = 500;
    c_lo = 2101;
    c_hi = 2620;
  }
  else
  {
    // index > 500
    return 501;
  }
  psi = max(psi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, so2_24h));

  // pm10  μg/m^3, Coarse Particulate Matter (<10μm)
  if (pm10_24h < 50.5)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 50;
  }
  else if (pm10_24h < 150.5)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 51;
    c_hi = 150;
  }
  else if (pm10_24h < 350.5)
  {
    i_lo = 101;
    i_hi = 200;
    c_lo = 151;
    c_hi = 350;
  }
  else if (pm10_24h < 420.5)
  {
    i_lo = 201;
    i_hi = 300;
    c_lo = 351;
    c_hi = 420;
  }
  else if (pm10_24h < 500.5)
  {
    i_lo = 301;
    i_hi = 400;
    c_lo = 421;
    c_hi = 500;
  }
  else if (pm10_24h < 600.5)
  {
    i_lo = 401;
    i_hi = 500;
    c_lo = 501;
    c_hi = 600;
  }
  else
  {
    // index > 500
    return 501;
  }
  psi = max(psi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, pm10_24h));

  // pm2_5 μg/m^3, Fine Particulate Matter (<2.5μm)
  if (pm2_5_24h < 12.5)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 12;
  }
  else if (pm2_5_24h < 55.5)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 13;
    c_hi = 55;
  }
  else if (pm2_5_24h < 150.5)
  {
    i_lo = 101;
    i_hi = 200;
    c_lo = 56;
    c_hi = 150;
  }
  else if (pm2_5_24h < 250.5)
  {
    i_lo = 201;
    i_hi = 300;
    c_lo = 151;
    c_hi = 250;
  }
  else if (pm2_5_24h < 350.5)
  {
    i_lo = 301;
    i_hi = 400;
    c_lo = 251;
    c_hi = 350;
  }
  else if (pm2_5_24h < 500.5)
  {
    i_lo = 401;
    i_hi = 500;
    c_lo = 351;
    c_hi = 500;
  }
  else
  {
    // index > 500
    return 501;
  }
  psi = max(psi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, pm2_5_24h));

  return psi;
} // end singapore_psi

/* South Korea (CAI)
 *
 * References:
 *   https://www.airkorea.or.kr/eng/khaiInfo?pMENU_NO=166
 */
int south_korea_cai(float co_1h,  float no2_1h,   float o3_1h,
                    float so2_1h, float pm10_24h, float pm2_5_24h)
{
  int cai = 0;
  float i_lo, i_hi;
  float c_lo, c_hi;

  // co    μg/m^3, Carbon Monoxide (CO)
  // 1ppm * 1000ppb/1ppm * 1.1456 μg/m^3/ppb = 1145.6 μg/m^3
  if (co_1h < 2348.48)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 2291.2;
  }
  else if (co_1h < 10367.68)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 2405.76;
    c_hi = 10310.4;
  }
  else if (co_1h < 17241.28)
  {
    i_lo = 101;
    i_hi = 250;
    c_lo = 10424.96;
    c_hi = 17184;
  }
  else if (co_1h < 57337.28)
  {
    i_lo = 251;
    i_hi = 500;
    c_lo = 17298.56;
    c_hi = 57280;
  }
  else
  {
    // index > 500
    return 501;
  }
  cai = max(cai, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, co_1h));

  // no2   μg/m^3, Nitrogen Dioxide (NO2)
  // 1ppm * 1000ppb/1ppm * 1.8816 μg/m^3/ppb = 1881.6 μg/m^3
  if (no2_1h < 57.3888)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 56.448;
  }
  else if (no2_1h < 113.8368)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 58.3296;
    c_hi = 112.896;
  }
  else if (no2_1h < 377.2608)
  {
    i_lo = 101;
    i_hi = 250;
    c_lo = 114.7776;
    c_hi = 376.32;
  }
  else if (no2_1h < 3772.608)
  {
    i_lo = 251;
    i_hi = 500;
    c_lo = 378.2016;
    c_hi = 3763.2;
  }
  else
  {
    // index > 500
    return 501;
  }
  cai = max(cai, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, no2_1h));

  // o3    μg/m^3, Ozone (O3)
  // 1ppm * 1000ppb/1ppm * 1.9632 μg/m^3/ppb = 1963.2 μg/m^3
  if (o3_1h < 59.8776)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 58.896;
  }
  else if (o3_1h < 177.6696)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 60.8592;
    c_hi = 176.688;
  }
  else if (o3_1h < 295.4616)
  {
    i_lo = 101;
    i_hi = 250;
    c_lo = 178.6512;
    c_hi = 294.48;
  }
  else if (o3_1h < 1178.9016)
  {
    i_lo = 251;
    i_hi = 500;
    c_lo = 296.4432;
    c_hi = 1177.92;
  }
  else
  {
    // index > 500
    return 501;
  }
  cai = max(cai, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, o3_1h));

  // so2   μg/m^3, Sulfur Dioxide (SO2)
  // 1ppm * 1000ppb/1ppm * 8.4744 μg/m^3/ppb = 8474.4 μg/m^3
  if (so2_1h < 173.7252)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 169.488;
  }
  else if (so2_1h < 427.9572)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 177.9624;
    c_hi = 423.72;
  }
  else if (so2_1h < 1271.16)
  {
    i_lo = 101;
    i_hi = 250;
    c_lo = 432.1944;
    c_hi = 1271.16;
  }
  else if (so2_1h < 8478.6372)
  {
    i_lo = 251;
    i_hi = 500;
    c_lo = 1279.6344;
    c_hi = 8474.4;
  }
  else
  {
    // index > 500
    return 501;
  }
  cai = max(cai, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, so2_1h));

  // pm10  μg/m^3, Coarse Particulate Matter (<10μm)
  if (pm10_24h < 30.5)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 30;
  }
  else if (pm10_24h < 80.5)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 31;
    c_hi = 80;
  }
  else if (pm10_24h < 150.5)
  {
    i_lo = 101;
    i_hi = 250;
    c_lo = 81;
    c_hi = 150;
  }
  else if (pm10_24h < 600.5)
  {
    i_lo = 251;
    i_hi = 500;
    c_lo = 151;
    c_hi = 600;
  }
  else
  {
    // index > 500
    return 501;
  }
  cai = max(cai, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, pm10_24h));

  // pm2_5 μg/m^3, Fine Particulate Matter (<2.5μm)
  if (pm2_5_24h < 15.5)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 15;
  }
  else if (pm2_5_24h < 35.5)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 16;
    c_hi = 35;
  }
  else if (pm2_5_24h < 75.5)
  {
    i_lo = 101;
    i_hi = 250;
    c_lo = 36;
    c_hi = 75;
  }
  else if (pm2_5_24h < 500.5)
  {
    i_lo = 251;
    i_hi = 500;
    c_lo = 76;
    c_hi = 500;
  }
  else
  {
    // index > 500
    return 501;
  }
  cai = max(cai, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, pm2_5_24h));

  return cai;
} // end south_korea_cai

/* United Kingdom (DAQI)
 *
 * References:
 *   https://uk-air.defra.gov.uk/air-pollution/daqi?view=more-info
 *   https://en.wikipedia.org/wiki/Air_quality_index#United_Kingdom
 *   https://uk-air.defra.gov.uk/library/reports?report_id=750
 */
int united_kingdom_daqi(float no2_1h,   float o3_8h, float so2_15min,
                        float pm10_24h, float pm2_5_24h)
{
  // Pollutant averages are rounded to nearest integer
  if (o3_8h >= 240.5 || no2_1h >= 600.5 || so2_15min >= 1064.5 ||
      pm2_5_24h >= 70.5 || pm10_24h >= 100.5)
  {
    return 10;
  }
  else if (o3_8h >= 213.5 || no2_1h >= 534.5 || so2_15min >= 887.5 ||
           pm2_5_24h >= 64.5 || pm10_24h >= 91.5)
  {
    return 9;
  }
  else if (o3_8h >= 187.5 || no2_1h >= 467.5 || so2_15min >= 710.5 ||
           pm2_5_24h >= 58.5 || pm10_24h >= 83.5)
  {
    return 8;
  }
  else if (o3_8h >= 160.5 || no2_1h >= 400.5 || so2_15min >= 532.5 ||
           pm2_5_24h >= 53.5 || pm10_24h >= 75.5)
  {
    return 7;
  }
  else if (o3_8h >= 140.5 || no2_1h >= 334.5 || so2_15min >= 443.5 ||
           pm2_5_24h >= 47.5 || pm10_24h >= 66.5)
  {
    return 6;
  }
  else if (o3_8h >= 120.5 || no2_1h >= 267.5 || so2_15min >= 354.5 ||
           pm2_5_24h >= 41.5 || pm10_24h >= 58.5)
  {
    return 5;
  }
  else if (o3_8h >= 100.5 || no2_1h >= 200.5 || so2_15min >= 266.5 ||
           pm2_5_24h >= 35.5 || pm10_24h >= 50.5)
  {
    return 4;
  }
  else if (o3_8h >= 66.5 || no2_1h >= 134.5 || so2_15min >= 177.5 ||
           pm2_5_24h >= 23.5 || pm10_24h >= 33.5)
  {
    return 3;
  }
  else if (o3_8h >= 33.5 || no2_1h >= 67.5 || so2_15min >= 88.5 ||
           pm2_5_24h >= 11.5 || pm10_24h >= 16.5)
  {
    return 2;
  }
  else
  {
    return 1;
  }
} // end united_kingdom_daqi

/* United States (AQI)
 *
 * References:
 *   https://www.epa.gov/outdoor-air-quality-data/how-aqi-calculated
 *   https://www.airnow.gov/sites/default/files/2020-05/aqi-technical-assistance-document-sept2018.pdf
 *   https://en.wikipedia.org/wiki/Air_quality_index#United_States
 */
int united_states_aqi(float co_8h,    float no2_1h,
                      float o3_1h,    float o3_8h,
                      float so2_1h,   float so2_24h,
                      float pm10_24h, float pm2_5_24h)
{
  int aqi = 0;
  float i_lo, i_hi;
  float c_lo, c_hi;

  // Pollutant averages are truncated
  co_8h = truncate_float(co_8h / 1145.6, 1); // (ppm) truncate to 1 decimal place
  no2_1h = (int)(no2_1h / 1.8816);           // (ppb) truncate to integer
  o3_1h = truncate_float(o3_1h / 1963.2, 3); // (ppm) truncate to 3 decimal places
  o3_8h = truncate_float(o3_8h / 1963.2, 3); // (ppm) truncate to 3 decimal places
  so2_1h = (int)(so2_1h / 8.4744);           // (ppb) truncate to integer
  pm10_24h = (int)pm10_24h;                  // (μg/m^3) truncate to integer
  pm2_5_24h = truncate_float(pm2_5_24h, 1);  // (μg/m^3) truncate to 1 decimal place

  // co    μg/m^3, Carbon Monoxide (CO)
  if (co_8h <= 4.4)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 4.4;
  }
  else if (co_8h <= 9.4)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 4.5;
    c_hi = 9.4;
  }
  else if (co_8h <= 12.4)
  {
    i_lo = 101;
    i_hi = 150;
    c_lo = 9.5;
    c_hi = 12.4;
  }
  else if (co_8h <= 15.4)
  {
    i_lo = 151;
    i_hi = 200;
    c_lo = 12.5;
    c_hi = 15.4;
  }
  else if (co_8h <= 30.4)
  {
    i_lo = 201;
    i_hi = 300;
    c_lo = 15.5;
    c_hi = 30.4;
  }
  else if (co_8h <= 40.4)
  {
    i_lo = 301;
    i_hi = 400;
    c_lo = 30.5;
    c_hi = 40.4;
  }
  else if (co_8h <= 50.4)
  {
    i_lo = 401;
    i_hi = 500;
    c_lo = 40.5;
    c_hi = 50.4;
  }
  else
  {
    // index > 500
    return 501;
  }
  aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, co_8h));

  // no2   μg/m^3, Nitrogen Dioxide (NO2)
  if (no2_1h <= 53)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 53;
  }
  else if (no2_1h <= 100)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 54;
    c_hi = 100;
  }
  else if (no2_1h <= 360)
  {
    i_lo = 101;
    i_hi = 150;
    c_lo = 101;
    c_hi = 360;
  }
  else if (no2_1h <= 649)
  {
    i_lo = 151;
    i_hi = 200;
    c_lo = 361;
    c_hi = 649;
  }
  else if (no2_1h <= 1249)
  {
    i_lo = 201;
    i_hi = 300;
    c_lo = 350;
    c_hi = 1249;
  }
  else if (no2_1h <= 1649)
  {
    i_lo = 301;
    i_hi = 400;
    c_lo = 1250;
    c_hi = 1649;
  }
  else if (no2_1h <= 2049)
  {
    i_lo = 401;
    i_hi = 500;
    c_lo = 1650;
    c_hi = 2049;
  }
  else
  {
    // index > 500
    return 501;
  }
  aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, no2_1h));

  // o3    μg/m^3, Ground-Level Ozone (O3)
  if (o3_1h >= 0.125)
  {
    if (o3_1h <= 0.164)
    {
      i_lo = 101;
      i_hi = 150;
      c_lo = 0.125;
      c_hi = 0.164;
    }
    else if (o3_1h <= 0.204)
    {
      i_lo = 151;
      i_hi = 200;
      c_lo = 0.165;
      c_hi = 0.204;
    }
    else if (o3_1h <= 0.404)
    {
      i_lo = 201;
      i_hi = 300;
      c_lo = 0.205;
      c_hi = 0.404;
    }
    else if (o3_1h <= 1649)
    {
      i_lo = 301;
      i_hi = 400;
      c_lo = 1250;
      c_hi = 1649;
    }
    else if (o3_1h <= 2049)
    {
      i_lo = 401;
      i_hi = 500;
      c_lo = 1650;
      c_hi = 2049;
    }
    else
    {
      // index > 500
      return 501;
    }
    aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, o3_1h));
  }
  if (o3_8h <= 0.200)
  {
    if (o3_8h <= 0.054)
    {
      i_lo = 0;
      i_hi = 50;
      c_lo = 0;
      c_hi = 0.054;
    }
    else if (o3_8h <= 0.070)
    {
      i_lo = 51;
      i_hi = 100;
      c_lo = 0.055;
      c_hi = 0.070;
    }
    else if (o3_8h <= 0.085)
    {
      i_lo = 101;
      i_hi = 150;
      c_lo = 0.071;
      c_hi = 0.085;
    }
    else if (o3_8h <= 0.105)
    {
      i_lo = 151;
      i_hi = 200;
      c_lo = 0.086;
      c_hi = 0.105;
    }
    else
    {
      // 0.106 <= o3_8h <= 0.200
      i_lo = 201;
      i_hi = 300;
      c_lo = 0.106;
      c_hi = 0.200;
    }
    aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, o3_8h));
  }

  // so2   μg/m^3, Sulfur Dioxide (SO2)
  if (so2_1h <= 185)
  {
    if (so2_1h <= 35)
    {
      i_lo = 0;
      i_hi = 50;
      c_lo = 0;
      c_hi = 35;
    }
    else if (so2_1h <= 75)
    {
      i_lo = 51;
      i_hi = 100;
      c_lo = 36;
      c_hi = 75;
    }
    else
    {
      // 76 <= so2_1h <= 185
      i_lo = 101;
      i_hi = 150;
      c_lo = 76;
      c_hi = 185;
    }
    aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, so2_1h));
  }
  else
  {
    if (so2_24h <= 35)
    {
      i_lo = 0;
      i_hi = 50;
      c_lo = 0;
      c_hi = 35;
    }
    else if (so2_24h <= 75)
    {
      i_lo = 51;
      i_hi = 100;
      c_lo = 36;
      c_hi = 75;
    }
    else if (so2_24h <= 185)
    {
      i_lo = 101;
      i_hi = 150;
      c_lo = 76;
      c_hi = 185;
    }
    else if (so2_24h <= 304)
    {
      i_lo = 151;
      i_hi = 200;
      c_lo = 186;
      c_hi = 304;
    }
    else if (so2_24h <= 604)
    {
      i_lo = 201;
      i_hi = 300;
      c_lo = 305;
      c_hi = 604;
    }
    else if (so2_24h <= 804)
    {
      i_lo = 301;
      i_hi = 400;
      c_lo = 605;
      c_hi = 804;
    }
    else if (so2_24h <= 1004)
    {
      i_lo = 401;
      i_hi = 500;
      c_lo = 805;
      c_hi = 1004;
    }
    else
    {
      // index > 500
      return 501;
    }
    aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, so2_24h));
  }

  // pm10  μg/m^3, Coarse Particulate Matter (<10μm)
  if (pm10_24h <= 54)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 54;
  }
  else if (pm10_24h <= 154)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 55;
    c_hi = 154;
  }
  else if (pm10_24h <= 254)
  {
    i_lo = 101;
    i_hi = 150;
    c_lo = 155;
    c_hi = 254;
  }
  else if (pm10_24h <= 354)
  {
    i_lo = 151;
    i_hi = 200;
    c_lo = 255;
    c_hi = 354;
  }
  else if (pm10_24h <= 424)
  {
    i_lo = 201;
    i_hi = 300;
    c_lo = 355;
    c_hi = 424;
  }
  else if (pm10_24h <= 504)
  {
    i_lo = 301;
    i_hi = 400;
    c_lo = 425;
    c_hi = 504;
  }
  else if (pm10_24h <= 604)
  {
    i_lo = 401;
    i_hi = 500;
    c_lo = 505;
    c_hi = 604;
  }
  else
  {
    // index > 500
    return 501;
  }
  aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, pm10_24h));

  // pm2_5 μg/m^3, Fine Particulate Matter (<2.5μm)
  if (pm2_5_24h <= 12.0)
  {
    i_lo = 0;
    i_hi = 50;
    c_lo = 0;
    c_hi = 12.0;
  }
  else if (pm2_5_24h <= 35.4)
  {
    i_lo = 51;
    i_hi = 100;
    c_lo = 12.1;
    c_hi = 35.4;
  }
  else if (pm2_5_24h <= 55.4)
  {
    i_lo = 101;
    i_hi = 150;
    c_lo = 35.5;
    c_hi = 55.4;
  }
  else if (pm2_5_24h <= 150.4)
  {
    i_lo = 151;
    i_hi = 200;
    c_lo = 55.5;
    c_hi = 150.4;
  }
  else if (pm2_5_24h <= 250.4)
  {
    i_lo = 201;
    i_hi = 300;
    c_lo = 150.5;
    c_hi = 250.4;
  }
  else if (pm2_5_24h <= 350.4)
  {
    i_lo = 301;
    i_hi = 400;
    c_lo = 250.5;
    c_hi = 350.4;
  }
  else if (pm2_5_24h <= 500.4)
  {
    i_lo = 401;
    i_hi = 500;
    c_lo = 350.5;
    c_hi = 500.4;
  }
  else
  {
    // index > 500
    return 501;
  }
  aqi = max(aqi, compute_piecewise_aqi(i_lo, i_hi, c_lo, c_hi, pm2_5_24h));

  return aqi;
} // end united_states_aqi

/*
 * Indicates Air Quality
 */
const char *australia_aqi_desc(int aqi)
{
  if (aqi <= 33)
  {
    return AUSTRALIA_AQI_TXT[0];
  }
  else if (aqi <= 66)
  {
    return AUSTRALIA_AQI_TXT[1];
  }
  else if (aqi <= 99)
  {
    return AUSTRALIA_AQI_TXT[2];
  }
  else if (aqi <= 149)
  {
    return AUSTRALIA_AQI_TXT[3];
  }
  else if (aqi <= 200)
  {
    return AUSTRALIA_AQI_TXT[4];
  }
  else
  {
    return AUSTRALIA_AQI_TXT[5];
  }
} // end australia_aqi_desc

/*
 * Indicates Health Risk
 */
const char *canada_aqhi_desc(int aqhi)
{
  if (aqhi <= 4)
  {
    return CANADA_AQHI_TXT[0];
  }
  else if (aqhi <= 6)
  {
    return CANADA_AQHI_TXT[1];
  }
  else if (aqhi <= 10)
  {
    return CANADA_AQHI_TXT[2];
  }
  else
  {
    return CANADA_AQHI_TXT[3];
  }
} // end canada_aqhi_desc

/*
 * Indicates Air Pollution
 */
const char *china_aqi_desc(int aqi)
{
  if (aqi <= 50)
  {
    return CHINA_AQI_TXT[0];
  }
  else if (aqi <= 100)
  {
    return CHINA_AQI_TXT[1];
  }
  else if (aqi <= 150)
  {
    return CHINA_AQI_TXT[2];
  }
  else if (aqi <= 200)
  {
    return CHINA_AQI_TXT[3];
  }
  else if (aqi <= 300)
  {
    return CHINA_AQI_TXT[4];
  }
  else
  {
    return CHINA_AQI_TXT[5];
  }
} // end china_aqi_desc

/*
 * Indicates Air Pollution
 */
const char *european_union_caqi_desc(int caqi)
{
  if (caqi <= 25)
  {
    return EUROPEAN_UNION_CAQI_TXT[0];
  }
  else if (caqi <= 50)
  {
    return EUROPEAN_UNION_CAQI_TXT[1];
  }
  else if (caqi <= 75)
  {
    return EUROPEAN_UNION_CAQI_TXT[2];
  }
  else if (caqi <= 100)
  {
    return EUROPEAN_UNION_CAQI_TXT[3];
  }
  else
  {
    return EUROPEAN_UNION_CAQI_TXT[4];
  }
} // end european_union_caqi_desc

/*
 * Indicates Health Risk
 */
const char *hong_kong_aqhi_desc(int aqhi)
{
  if (aqhi <= 3)
  {
    return HONG_KONG_AQHI_TXT[0];
  }
  else if (aqhi <= 6)
  {
    return HONG_KONG_AQHI_TXT[1];
  }
  else if (aqhi <= 7)
  {
    return HONG_KONG_AQHI_TXT[2];
  }
  else if (aqhi <= 10)
  {
    return HONG_KONG_AQHI_TXT[3];
  }
  else
  {
    return HONG_KONG_AQHI_TXT[4];
  }
} // end hong_kong_aqhi_desc

/*
 * Indicates Air Quality
 */
const char *india_aqi_desc(int aqi)
{
  if (aqi <= 50)
  {
    return INDIA_AQI_TXT[0];
  }
  else if (aqi <= 100)
  {
    return INDIA_AQI_TXT[1];
  }
  else if (aqi <= 200)
  {
    return INDIA_AQI_TXT[2];
  }
  else if (aqi <= 300)
  {
    return INDIA_AQI_TXT[3];
  }
  else if (aqi <= 400)
  {
    return INDIA_AQI_TXT[4];
  }
  else
  {
    return INDIA_AQI_TXT[5];
  }
} // end india_aqi_desc

/*
 * Indicates Health Risk
 */
const char *singapore_psi_desc(int psi)
{
  if (psi <= 50)
  {
    return SINGAPORE_PSI_TXT[0];
  }
  else if (psi <= 100)
  {
    return SINGAPORE_PSI_TXT[1];
  }
  else if (psi <= 200)
  {
    return SINGAPORE_PSI_TXT[2];
  }
  else if (psi <= 300)
  {
    return SINGAPORE_PSI_TXT[3];
  }
  else
  {
    return SINGAPORE_PSI_TXT[4];
  }
} // end singapore_psi_desc

/*
 * Indicates Health Risk
 */
const char *south_korea_cai_desc(int cai)
{
  if (cai <= 50)
  {
    return SOUTH_KOREA_CAI_TXT[0];
  }
  else if (cai <= 100)
  {
    return SOUTH_KOREA_CAI_TXT[1];
  }
  else if (cai <= 250)
  {
    return SOUTH_KOREA_CAI_TXT[2];
  }
  else
  {
    return SOUTH_KOREA_CAI_TXT[3];
  }
} // end south_korea_cai_desc

/*
 * Indicates Air Pollution
 */
const char *united_kingdom_daqi_desc(int daqi)
{
  if (daqi <= 3)
  {
    return UNITED_KINGDOM_DAQI_TXT[0];
  }
  else if (daqi <= 6)
  {
    return UNITED_KINGDOM_DAQI_TXT[1];
  }
  else if (daqi <= 9)
  {
    return UNITED_KINGDOM_DAQI_TXT[2];
  }
  else
  {
    return UNITED_KINGDOM_DAQI_TXT[3];
  }
} // end united_kingdom_daqi_desc

/*
 * Indicates Health Risk
 */
const char *united_states_aqi_desc(int aqi)
{
  if (aqi <= 50)
  {
    return UNITED_STATES_AQI_TXT[0];
  }
  else if (aqi <= 100)
  {
    return UNITED_STATES_AQI_TXT[1];
  }
  else if (aqi <= 150)
  {
    return UNITED_STATES_AQI_TXT[2];
  }
  else if (aqi <= 200)
  {
    return UNITED_STATES_AQI_TXT[3];
  }
  else if (aqi <= 300)
  {
    return UNITED_STATES_AQI_TXT[4];
  }
  else
  {
    return UNITED_STATES_AQI_TXT[5];
  }
} // end united_states_aqi_desc

/* Returns the average pollutant concentration over a given number of previous
 * hours.
 *
 * 'pollutant' is an array of hourly concentrations. The last element in
 * pollutant is the most recent hourly concentration. 'hours' must be a positive
 * integer.
 *
 * Passing NULL will return 0.
 */
float avg_conc(const float pollutant[24], int hours)
{
  if (pollutant == NULL)
  {
    return 0.f;
  }

  float avg = 0;
  // index (size - 1) is most recent hourly concentration
  for (int h = (24 - 1) - (hours - 1) ; h < 24 ; ++h)
  {
    avg += pollutant[h];
  }

  avg = avg / (float) hours;
  return avg;
}

int calc_australia_aqi(
             const float co[24],  const float nh3[24],  const float no[24],
             const float no2[24], const float o3[24],   const float pb[24],
             const float so2[24], const float pm10[24], const float pm2_5[24])
{
  float co_8h     = avg_conc(co,     8);
  float no2_1h    = avg_conc(no2,    1);
  float o3_1h     = avg_conc(o3,     1);
  float o3_4h     = avg_conc(o3,     4);
  float so2_1h    = avg_conc(so2,    1);
  float pm10_24h  = avg_conc(pm10,  24);
  float pm2_5_24h = avg_conc(pm2_5, 24);
  return australia_aqi(co_8h, no2_1h, o3_1h, o3_4h, so2_1h, pm10_24h,
                       pm2_5_24h);
} // end calc_australia_aqi

int calc_canada_aqhi(
             const float co[24],  const float nh3[24],  const float no[24],
             const float no2[24], const float o3[24],   const float pb[24],
             const float so2[24], const float pm10[24], const float pm2_5[24])
{
  float no2_3h    = avg_conc(no2,    3);
  float o3_3h     = avg_conc(o3,     3);
  float pm2_5_3h  = avg_conc(pm2_5,  3);
  return canada_aqhi(no2_3h, o3_3h, pm2_5_3h);
} // end calc_canada_aqhi

int calc_china_aqi(
             const float co[24],  const float nh3[24],  const float no[24],
             const float no2[24], const float o3[24],   const float pb[24],
             const float so2[24], const float pm10[24], const float pm2_5[24])
{
  float co_1h     = avg_conc(co,     1);
  float co_24h    = avg_conc(co,    24);
  float no2_1h    = avg_conc(no2,    1);
  float no2_24h   = avg_conc(no2,   24);
  float o3_1h     = avg_conc(o3,     1);
  float o3_8h     = avg_conc(o3,     8);
  float so2_1h    = avg_conc(so2,    1);
  float so2_24h   = avg_conc(so2,   24);
  float pm10_24h  = avg_conc(pm10,  24);
  float pm2_5_24h = avg_conc(pm2_5, 24);
  return china_aqi(co_1h, co_24h, no2_1h, no2_24h, o3_1h, o3_8h, so2_1h,
                   so2_24h, pm10_24h, pm2_5_24h);
} // end calc_china_aqi

int calc_european_union_caqi(
             const float co[24],  const float nh3[24],  const float no[24],
             const float no2[24], const float o3[24],   const float pb[24],
             const float so2[24], const float pm10[24], const float pm2_5[24])
{
  float no2_1h    = avg_conc(no2,    1);
  float o3_1h     = avg_conc(o3,     1);
  float pm10_1h   = avg_conc(pm10,   1);
  float pm2_5_1h  = avg_conc(pm2_5,  1);
  return european_union_caqi(no2_1h, o3_1h, pm10_1h, pm2_5_1h);
} // end calc_european_union_caqi

int calc_hong_kong_aqhi(
             const float co[24],  const float nh3[24],  const float no[24],
             const float no2[24], const float o3[24],   const float pb[24],
             const float so2[24], const float pm10[24], const float pm2_5[24])
{
  float no2_3h    = avg_conc(no2,    3);
  float o3_3h     = avg_conc(o3,     3);
  float so2_3h    = avg_conc(so2,    3);
  float pm10_3h   = avg_conc(pm10,   3);
  float pm2_5_3h  = avg_conc(pm2_5,  3);
  return hong_kong_aqhi(no2_3h,  o3_3h, so2_3h, pm10_3h, pm2_5_3h);
} // end calc_hong_kong_aqhi

int calc_india_aqi(
             const float co[24],  const float nh3[24],  const float no[24],
             const float no2[24], const float o3[24],   const float pb[24],
             const float so2[24], const float pm10[24], const float pm2_5[24])
{
  float co_8h     = avg_conc(co,     8);
  float nh3_24h   = avg_conc(nh3,   24);
  float no2_24h   = avg_conc(no2,   24);
  float o3_8h     = avg_conc(o3,     8);
  float pb_24h    = avg_conc(pb,    24);
  float so2_24h   = avg_conc(so2,   24);
  float pm10_24h  = avg_conc(pm10,  24);
  float pm2_5_24h = avg_conc(pm2_5, 24);
  return india_aqi(co_8h, nh3_24h, no2_24h, o3_8h, pb_24h, so2_24h, pm10_24h,
                   pm2_5_24h);
} // end calc_india_aqi

int calc_singapore_psi(
             const float co[24],  const float nh3[24],  const float no[24],
             const float no2[24], const float o3[24],   const float pb[24],
             const float so2[24], const float pm10[24], const float pm2_5[24])
{
  float co_8h     = avg_conc(co,     8);
  float no2_1h    = avg_conc(no2,    1);
  float o3_1h     = avg_conc(o3,     1);
  float o3_8h     = avg_conc(o3,     8);
  float so2_24h   = avg_conc(so2,   24);
  float pm10_24h  = avg_conc(pm10,  24);
  float pm2_5_24h = avg_conc(pm2_5, 24);
  return singapore_psi(co_8h, no2_1h, o3_1h, o3_8h, so2_24h, pm10_24h,
                       pm2_5_24h);
} // end calc_singapore_psi

int calc_south_korea_cai(
             const float co[24],  const float nh3[24],  const float no[24],
             const float no2[24], const float o3[24],   const float pb[24],
             const float so2[24], const float pm10[24], const float pm2_5[24])
{
  float co_1h     = avg_conc(co,     1);
  float no2_1h    = avg_conc(no2,    1);
  float o3_1h     = avg_conc(o3,     1);
  float so2_1h    = avg_conc(so2,    1);
  float pm10_24h  = avg_conc(pm10,  24);
  float pm2_5_24h = avg_conc(pm2_5, 24);
  return south_korea_cai(co_1h, no2_1h, o3_1h, so2_1h, pm10_24h, pm2_5_24h);
} // end calc_south_korea_cai

int calc_united_kingdom_daqi(
             const float co[24],  const float nh3[24],  const float no[24],
             const float no2[24], const float o3[24],   const float pb[24],
             const float so2[24], const float pm10[24], const float pm2_5[24])
{
  float no2_1h    = avg_conc(no2,    1);
  float o3_8h     = avg_conc(o3,     8);
  float so2_15min = avg_conc(so2,    1); // USING LAST HOURLY CONCENTRATION!!!
  float pm10_24h  = avg_conc(pm10,  24);
  float pm2_5_24h = avg_conc(pm2_5, 24);
  return united_kingdom_daqi(no2_1h, o3_8h, so2_15min, pm10_24h, pm2_5_24h);
} // end calc_united_kingdom_daqi

int calc_united_states_aqi(
             const float co[24],  const float nh3[24],  const float no[24],
             const float no2[24], const float o3[24],   const float pb[24],
             const float so2[24], const float pm10[24], const float pm2_5[24])
{
  float co_8h     = avg_conc(co,     8);
  float no2_1h    = avg_conc(no2,    1);
  float o3_1h     = avg_conc(o3,     1);
  float o3_8h     = avg_conc(o3,     8);
  float so2_1h    = avg_conc(so2,    1);
  float so2_24h   = avg_conc(so2,   24);
  float pm10_24h  = avg_conc(pm10,  24);
  float pm2_5_24h = avg_conc(pm2_5, 24);
  return united_states_aqi(co_8h, no2_1h, o3_1h, o3_8h, so2_1h, so2_24h,
                           pm10_24h, pm2_5_24h);
} // end calc_united_states_aqi

/* Fast lookup for calc_aqi functions. Organized alphabetically
 * (same order as aqi_scale_t enums).
 */
static int (*CALC_AQI_LOOKUP_TABLE[NUM_AQI_SCALES])(
                          const float[24], const float[24], const float[24],
                          const float[24], const float[24], const float[24],
                          const float[24], const float[24], const float[24]) = {
  calc_australia_aqi,
  calc_canada_aqhi,
  calc_china_aqi,
  calc_european_union_caqi,
  calc_hong_kong_aqhi,
  calc_india_aqi,
  calc_singapore_psi,
  calc_south_korea_cai,
  calc_united_kingdom_daqi,
  calc_united_states_aqi,
};

int calc_aqi(aqi_scale_t scale,
             const float co[24],  const float nh3[24],  const float no[24],
             const float no2[24], const float o3[24],   const float pb[24],
             const float so2[24], const float pm10[24], const float pm2_5[24])
{
  return CALC_AQI_LOOKUP_TABLE[scale](co, nh3, no, no2, o3, pb, so2, pm10,
                                      pm2_5);
} // end calc_aqi

/* Fast lookup for AQI scale max values. Organized alphabetically
 * (same order as aqi_scale_t enums).
 */
static const int AQI_MAX_LOOKUP_TABLE[NUM_AQI_SCALES] = {
  AUSTRALIA_AQI_MAX,
  CANADA_AQHI_MAX,
  CHINA_AQI_MAX,
  EUROPEAN_UNION_CAQI_MAX,
  HONG_KONG_AQHI_MAX,
  INDIA_AQI_MAX,
  SINGAPORE_PSI_MAX,
  SOUTH_KOREA_CAI_MAX,
  UNITED_KINGDOM_DAQI_MAX,
  UNITED_STATES_AQI_MAX,
};

int aqi_scale_max(aqi_scale_t scale) {
  return AQI_MAX_LOOKUP_TABLE[scale];
} // end aqi_scale_max

/* Fast lookup for AQI descriptor functions. Organized alphabetically
 * (same order as aqi_scale_t enums).
 */
static const char *(*AQI_DESC_LOOKUP_TABLE[NUM_AQI_SCALES])(int) = {
  australia_aqi_desc,
  canada_aqhi_desc,
  china_aqi_desc,
  european_union_caqi_desc,
  hong_kong_aqhi_desc,
  india_aqi_desc,
  singapore_psi_desc,
  south_korea_cai_desc,
  united_kingdom_daqi_desc,
  united_states_aqi_desc,
};

const char *aqi_desc(aqi_scale_t scale, int val)
{
  return AQI_DESC_LOOKUP_TABLE[scale](val);
} // end aqi_desc

/* Fast lookup for AQI scale descriptor types. Organized alphabetically
 * (same order as aqi_scale_t enums).
 */
static const aqi_desc_type_t AQI_DESC_TYPE_LOOKUP_TABLE[NUM_AQI_SCALES] = {
  AUSTRALIA_AQI_DESC_TYPE,
  CANADA_AQHI_DESC_TYPE,
  CHINA_AQI_DESC_TYPE,
  EUROPEAN_UNION_CAQI_DESC_TYPE,
  HONG_KONG_AQHI_DESC_TYPE,
  INDIA_AQI_DESC_TYPE,
  SINGAPORE_PSI_DESC_TYPE,
  SOUTH_KOREA_CAI_DESC_TYPE,
  UNITED_KINGDOM_DAQI_DESC_TYPE,
  UNITED_STATES_AQI_DESC_TYPE,
};

aqi_desc_type_t aqi_desc_type(aqi_scale_t scale)
{
  return AQI_DESC_TYPE_LOOKUP_TABLE[scale];
} // aqi_desc_type
