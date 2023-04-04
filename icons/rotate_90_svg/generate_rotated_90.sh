#!/bin/bash

INPUT=$1

mkdir -p rotated

for DEG in {0..359..90}; 
do
OUTPUT="./rotated/`basename $INPUT .svg`_`expr \( $DEG \) % 360`deg.svg"

cp $INPUT $OUTPUT

sed -i -e "s/rotate(0/rotate($DEG/g" $OUTPUT
echo $DEG
done
