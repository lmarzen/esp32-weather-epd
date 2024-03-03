#!/bin/bash
# Font file to Adafruit GFX format script for esp32-weather-epd.
# Copyright (C) 2022-2024  Luke Marzen
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
TTF_FILES=ttf/*
OUTPUT_PATH="./fonts"
SIZES=(4 5 6 7 8 9 10 11 12 14 16 18 20 22 24 26)
TEMPERATURE_SIZES=(48)

# clean fonts output
echo "Cleaning $OUTPUT_PATH"
rm -r $OUTPUT_PATH
mkdir $OUTPUT_PATH

# build fontconvert
cd fontconvert
make clean
make
cd ../


for fontfile in $TTF_FILES
  do
  # ascii hexidecimal value of the character to remap degree symbol to
  # REMAP_CH="0x60"

  # REMAP_OUT="${fontfile%%.*}_remap.${fontfile##*.}"
  SUBSET_OUT="${fontfile%%.*}_temperature_set.${fontfile##*.}"
  # generate temperature display subset version of the font
  pyftsubset ${fontfile} \
    --text="0123456789.-" \
    --unicodes=U+00B0 \
    --output-file=$SUBSET_OUT \

  # remap ` to degree symbol
  # echo "Remapping degree symbol..."

  # ttx ${fontfile}
  # REMAP_TTX="${fontfile%%.*}_remap.ttx"
  # mv "${fontfile%%.*}.ttx" $REMAP_TTX
  # sed -i "s/0xb0/${REMAP_CH}/g" $REMAP_TTX
  # ttx -b $REMAP_TTX
  # rm $REMAP_TTX

  # ttx $SUBSET_OUT
  # rm $SUBSET_OUT
  # SUBSET_TTX="${SUBSET_OUT%%.*}.ttx"
  # sed -i "s/0xb0/${REMAP_CH}/g" $SUBSET_TTX
  # ttx -b $SUBSET_TTX
  # rm $SUBSET_TTX

  # convert .otf/.ttf files to c-style arrays
  FONT=`basename ${fontfile%%.*} | tr '-' '_'`
  echo $FONT
  mkdir $OUTPUT_PATH/$FONT
  for SI in ${SIZES[*]}
    do
    OUTFILE=$OUTPUT_PATH/$FONT/$FONT"_"$SI"pt8b.h"
    echo "fontconvert ${fontfile} $SI > $OUTFILE"
    ./fontconvert/fontconvert ${fontfile} $SI > $OUTFILE
    sed -i "s/${SI}pt8b/_${SI}pt8b/g" $OUTFILE
    # sed -i "s/_remap${SI}pt8b/${SI}pt8b/g" $OUTFILE
  done
  for SI in ${TEMPERATURE_SIZES[*]}
    do
    OUTFILE=$OUTPUT_PATH/$FONT/$FONT"_"$SI"pt8b_temperature.h"
    echo "fontconvert $SUBSET_OUT $SI > $OUTFILE"
    ./fontconvert/fontconvert $SUBSET_OUT $SI > $OUTFILE
    sed -i "s/_temperature_set${SI}pt8b/_${SI}pt8b_temperature/g" $OUTFILE
  done

  # clean up
  echo "rm $SUBSET_OUT"
  rm $SUBSET_OUT
  # echo "rm $REMAP_OUT"
  # rm $REMAP_OUT

  # create header file (this will make fonts way easier to include)
  HEADER_FILE=$OUTPUT_PATH/$FONT".h"
  echo "#ifndef __FONTS_"${FONT^^}"_H__" >> $HEADER_FILE
  echo "#define __FONTS_"${FONT^^}"_H__" >> $HEADER_FILE
  for FILE in $OUTPUT_PATH/$FONT/*
    do
    echo "#include \"$FONT/`basename $FILE`\"" >> $HEADER_FILE
  done
  echo "" >> $HEADER_FILE
  for FILE in $OUTPUT_PATH/$FONT/*
    do
    FONT_SUFFIX=$(echo "`basename $FILE .h`" | grep -oP '(?<=pt8b)\w+')
    FONT_SIZE=$(echo "`basename $FILE .h`" | grep -oP '\d+(?=pt8b)')

    echo "#define FONT_"$FONT_SIZE"pt8b"$FONT_SUFFIX" `basename $FILE .h`" >> $HEADER_FILE
  done
  echo "#endif" >> $HEADER_FILE

done

# clean up
cd fontconvert
make clean
cd ../

