#!/bin/bash

# Font file to Adafruit GFX format script for esp32-weather-epd.
# Copyright (C) 2022-2023  Luke Marzen
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

OUTPUT_PATH="./fonts/"
SIZES=(4 5 6 7 8 9 10 11 12 14 16 18 20 22 24 26)
TEMPERATURE_SIZES=(48)

# ascii hexidecimal value of the character to remap degree symbol to
REMAP_CH="0x60"

# clean fonts output 
echo "Cleaning $OUTPUT_PATH"
rm $OUTPUT_PATH**.h

REMAP_OUT="${1%%.*}_remap.${1##*.}"
SUBSET_OUT="${1%%.*}_temperature_set.${1##*.}"
# generate temperature display subset version of the font
pyftsubset ${1} \
  --text="0123456789.-" \
  --unicodes=U+00B0 \
  --output-file=$SUBSET_OUT \

# remap ` to degree symbol
echo "Remapping degree symbol..."

ttx ${1}
REMAP_TTX="${1%%.*}_remap.ttx"
mv "${1%%.*}.ttx" $REMAP_TTX
sed -i "s/0xb0/${REMAP_CH}/g" $REMAP_TTX
ttx -b $REMAP_TTX
rm $REMAP_TTX

ttx $SUBSET_OUT
rm $SUBSET_OUT
SUBSET_TTX="${SUBSET_OUT%%.*}.ttx"
sed -i "s/0xb0/${REMAP_CH}/g" $SUBSET_TTX
ttx -b $SUBSET_TTX
rm $SUBSET_TTX

# build fontconvert
cd fontconvert
make clean
make
cd ../

# convert .otf/.ttf files to c-style arrays
FONT=$(basename ${1%%.*})
for SI in ${SIZES[*]}
  do
  OUTFILE=$OUTPUT_PATH$FONT$SI"pt8b.h"
  echo "fontconvert $REMAP_OUT $SI > $OUTFILE"
  ./fontconvert/fontconvert $REMAP_OUT $SI > $OUTFILE
  sed -i "s/_remap${SI}pt8b/${SI}pt8b/g" $OUTFILE
done
for SI in ${TEMPERATURE_SIZES[*]}
  do
  OUTFILE=$OUTPUT_PATH$FONT$SI"pt_temperature.h"
  echo "fontconvert $SUBSET_OUT $SI > $OUTFILE"
  ./fontconvert/fontconvert $SUBSET_OUT $SI > $OUTFILE
  sed -i "s/_temperature_set${SI}pt8b/${SI}pt_temperature/g" $OUTFILE
done

# clean up
cd fontconvert
make clean
cd ../
echo "rm $REMAP_OUT"
rm $REMAP_OUT
echo "rm $SUBSET_OUT"
rm $SUBSET_OUT
