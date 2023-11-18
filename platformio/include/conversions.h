/* Unit conversion declarations for esp32-weather-epd.
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

#ifndef __CONVERSIONS_H__
#define __CONVERSIONS_H__

float kelvin_to_celsius(float kelvin);
float kelvin_to_fahrenheit(float kelvin);
float celsius_to_kelvin(float celsius);
float celsius_to_fahrenheit(float celsius);

float meterspersecond_to_feetpersecond(float meterspersecond);
float meterspersecond_to_kilometersperhour(float meterspersecond);
float meterspersecond_to_milesperhour(float meterspersecond);
float meterspersecond_to_knots(float meterspersecond);
int meterspersecond_to_beaufort(float meterspersecond);

float hectopascals_to_pascals(float hectopascals);
float hectopascals_to_millimetersofmercury(float hectopascals);
float hectopascals_to_inchesofmercury(float hectopascals);
float hectopascals_to_millibars(float hectopascals);
float hectopascals_to_atmospheres(float hectopascals);
float hectopascals_to_gramspersquarecentimeter(float hectopascals);
float hectopascals_to_poundspersquareinch(float hectopascals);

float meters_to_kilometers(float meters);
float meters_to_miles(float meters);
float meters_to_feet(float meters);

float millimeters_to_inches(float meters);
float millimeters_to_centimeters(float meters);

#endif

