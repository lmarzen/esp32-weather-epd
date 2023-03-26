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

inline float kelvin_to_celsius(float kelvin);
inline float kelvin_to_fahrenheit(float kelvin);

inline float meterspersecond_to_feetpersecond(float meterspersecond);
inline float meterspersecond_to_kilometersperhour(float meterspersecond);
inline float meterspersecond_to_milesperhour(float meterspersecond);
inline float meterspersecond_to_knots(float meterspersecond);
inline int meterspersecond_to_beaufort(float meterspersecond);

inline float hectopascals_to_pascals(float hectopascals);
inline float hectopascals_to_millimetersofmercury(float hectopascals);
inline float hectopascals_to_inchesofmercury(float hectopascals);
inline float hectopascals_to_millibars(float hectopascals);
inline float hectopascals_to_atmospheres(float hectopascals);
inline float hectopascals_to_gramspersquarecentimeter(float hectopascals);
inline float hectopascals_to_poundspersquareinch(float hectopascals);

inline float kilometers_to_meters(float kilometers);
inline float kilometers_to_miles(float kilometers);
inline float kilometers_to_feet(float kilometers);

#endif
