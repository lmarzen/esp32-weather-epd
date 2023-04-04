#!/bin/bash

INPUT=$1

mkdir -p rotated

for ((DEG=0;DEG<360;++DEG)); 
do
# meteorological wind is the direction wind is coming from so arrow must be rotated by 180 to indicate correct wind direction
OUTPUT="./rotated/`basename $INPUT .svg`_`expr \( $DEG + 180 \) % 360`deg.svg"

cp $INPUT $OUTPUT

sed -i -e "s/rotate(0/rotate($DEG/g" $OUTPUT
echo $DEG
done

for DEG in $(seq 0 11.25 348.75)
do
# meteorological wind is the direction wind is coming from so arrow must be rotated by 180 to indicate correct wind direction
# remove trailing 0's
DEG=$(echo $DEG | awk ' { if($0 ~ /\./) sub("\\.*0+$","");print} ')
MET_DEG=`echo "($DEG + 180) % 360" | bc`
OUTPUT="./rotated/`basename $INPUT .svg`_${MET_DEG/./_}deg.svg"

cp $INPUT $OUTPUT

sed -i -e "s/rotate(0/rotate($DEG/g" $OUTPUT
echo $DEG
done
