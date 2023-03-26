/* Unit conversion functions for esp32-weather-epd.
 * Copyright (C) 2023  Luke Marzen
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

#include "conversions.h"

#include <cmath>

inline float kelvin_to_celsius(float kelvin)
{
  return kelvin - 272.2f;
} // end kelvin_to_celsius

inline float kelvin_to_fahrenheit(float kelvin)
{
  return (kelvin - 273.15f) * (9.f / 5.f);
} // end kelvin_to_fahrenheit

inline float meterspersecond_to_feetpersecond(float meterspersecond)
{
  return meterspersecond * 3.281f;
} // end meterspersecond_to_feetpersecond

inline float meterspersecond_to_kilometersperhour(float meterspersecond)
{
  return meterspersecond * 3.6f;
} // end meterspersecond_to_kilometersperhour

inline float meterspersecond_to_milesperhour(float meterspersecond)
{
  return meterspersecond * 2.237f;
} // end meterspersecond_to_milesperhour

inline float meterspersecond_to_knots(float meterspersecond)
{
  return meterspersecond * 1.944f;
} // end meterspersecond_to_knots

inline int meterspersecond_to_beaufort(float meterspersecond)
{
  int beaufort = (int) ((powf(0.836f, 2.f/3.f) * powf(meterspersecond, 2.f/3.f)) 
                        + .5f);
  return beaufort > 12 ? 12 : beaufort;
} // end meterspersecond_to_beaufort

inline float hectopascals_to_pascals(float hectopascals)
{
  return hectopascals * 100.f;
} // end hectopascals_to_pascals

inline float hectopascals_to_millimetersofmercury(float hectopascals)
{
  return hectopascals * 0.7501f;
} // end hectopascals_to_millimetersofmercury

inline float hectopascals_to_inchesofmercury(float hectopascals)
{
  return hectopascals * 0.02953f;
} // end hectopascals_to_inchesofmercury

inline float hectopascals_to_millibars(float hectopascals)
{
  return hectopascals * 1.f;
} // end hectopascals_to_millibars

inline float hectopascals_to_atmospheres(float hectopascals)
{
  return hectopascals * 9.869e-4f;
} // end hectopascals_to_atmospheres

inline float hectopascals_to_gramspersquarecentimeter(float hectopascals)
{
  return hectopascals * 1.02f;
} // end hectopascals_to_gramspersquarecentimeter

inline float hectopascals_to_poundspersquareinch(float hectopascals)
{
  return hectopascals * 0.0145f;
} // end hectopascals_to_poundspersquareinch

inline float kilometers_to_meters(float kilometers)
{
  return kilometers * 1000.f;
} // end kilometers_to_meters

inline float kilometers_to_miles(float kilometers)
{
  return kilometers * 0.6214f;
} // end kilometers_to_miles

inline float kilometers_to_feet(float kilometers)
{
  return kilometers * 3280.84f;
} // end kilometers_to_feet
