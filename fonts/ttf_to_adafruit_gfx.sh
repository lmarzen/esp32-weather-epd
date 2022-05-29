#!/bin/bash
OUTPUT_PATH="./fonts/"
SIZES=(9 12 18 24 72)
TEMPERATURE_SIZES=(72)

# ascii hexidecimal value of the character to remap degree symbol to
REMAP_CH="0x60"

# clean fonts output 
echo "Cleaning $OUTPUT_PATH"
rm $OUTPUT_PATH**.h

REMAP_OUT="${1%%.*}_remap.${1##*.}"
SUBSET_OUT="${1%%.*}_temperature_set.${1##*.}"
# generate temperature display subset version
pyftsubset ${1} \
  --text="0123456789." \
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
  OUTFILE=$OUTPUT_PATH$FONT$SI"pt7b.h"
  echo "fontconvert $REMAP_OUT $SI > $OUTFILE"
  ./fontconvert/fontconvert $REMAP_OUT $SI > $OUTFILE
done
for SI in ${TEMPERATURE_SIZES[*]}
  do
  OUTFILE=$OUTPUT_PATH$FONT$SI"pttempdisp.h"
  echo "fontconvert $SUBSET_OUT $SI > $OUTFILE"
  ./fontconvert/fontconvert $SUBSET_OUT $SI > $OUTFILE
done

# clean up
echo "rm $REMAP_OUT"
rm $REMAP_OUT
echo "rm $SUBSET_OUT"
rm $SUBSET_OUT