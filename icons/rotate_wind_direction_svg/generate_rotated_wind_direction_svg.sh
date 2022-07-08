#!/bin/bash

INPUT=$1

for ((DEG=0;DEG<360;++DEG)); 
do
# meteorological wind is the direction wind is coming from so arrow must be rotated by 180 to indicate correct wind direction
OUTPUT="./rotated/`basename $INPUT .svg``expr \( $DEG + 180 \) % 360`deg.svg"

cp $INPUT $OUTPUT

sed -i -e "s/rotate(0/rotate($DEG/g" $OUTPUT
echo $DEG
done

