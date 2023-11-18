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

float kelvin_to_celsius(float kelvin)
{
  return kelvin - 273.15f;
} // end kelvin_to_celsius

float kelvin_to_fahrenheit(float kelvin)
{
  return kelvin * (9.f / 5.f) - 459.67f;
} // end kelvin_to_fahrenheit

float celsius_to_kelvin(float celsius)
{
  return celsius + 273.15f;
} // end celsius_to_kelvin

float celsius_to_fahrenheit(float celsius)
{
  return celsius * (9.f / 5.f) + 32.f;
} // end celsius_to_fahrenheit

float meterspersecond_to_feetpersecond(float meterspersecond)
{
  return meterspersecond * 3.281f;
} // end meterspersecond_to_feetpersecond

float meterspersecond_to_kilometersperhour(float meterspersecond)
{
  return meterspersecond * 3.6f;
} // end meterspersecond_to_kilometersperhour

float meterspersecond_to_milesperhour(float meterspersecond)
{
  return meterspersecond * 2.237f;
} // end meterspersecond_to_milesperhour

float meterspersecond_to_knots(float meterspersecond)
{
  return meterspersecond * 1.944f;
} // end meterspersecond_to_knots

int meterspersecond_to_beaufort(float meterspersecond)
{
  int beaufort = (int) ((powf( 1 / 0.836f, 2.f/3.f)
                         * powf(meterspersecond, 2.f/3.f))
                        + .5f);
  return beaufort > 12 ? 12 : beaufort;
} // end meterspersecond_to_beaufort

float hectopascals_to_pascals(float hectopascals)
{
  return hectopascals * 100.f;
} // end hectopascals_to_pascals

float hectopascals_to_millimetersofmercury(float hectopascals)
{
  return hectopascals * 0.7501f;
} // end hectopascals_to_millimetersofmercury

float hectopascals_to_inchesofmercury(float hectopascals)
{
  return hectopascals * 0.02953f;
} // end hectopascals_to_inchesofmercury

float hectopascals_to_millibars(float hectopascals)
{
  return hectopascals * 1.f;
} // end hectopascals_to_millibars

float hectopascals_to_atmospheres(float hectopascals)
{
  return hectopascals * 9.869e-4f;
} // end hectopascals_to_atmospheres

float hectopascals_to_gramspersquarecentimeter(float hectopascals)
{
  return hectopascals * 1.02f;
} // end hectopascals_to_gramspersquarecentimeter

float hectopascals_to_poundspersquareinch(float hectopascals)
{
  return hectopascals * 0.0145f;
} // end hectopascals_to_poundspersquareinch

float meters_to_kilometers(float meters)
{
  return meters * 0.001f;
} // end meters_to_kilometers

float meters_to_miles(float meters)
{
  return meters * 6.214e-4f;
} // end meters_to_miles

float meters_to_feet(float meters)
{
  return meters * 3.281f;
} // end meters_to_feet

float millimeters_to_inches(float millimeter)
{
  return millimeter / 25.4f;
} // end milimeters_to_inches

float millimeters_to_centimeters(float millimeter)
{
  return millimeter / 10.0f;
} // end milimeters_to_centimeter

