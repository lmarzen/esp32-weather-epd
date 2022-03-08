#!/bin/bash
echo "Cleaning old files..."
rm ./png/large/*
rm ./png/small/*
rm ./headers/*

# arguments 1($1) and 2($2) determine the resolution of the output images
LDENSITY="$(($1*96))"
SDENSITY="$(($2*96))"
echo "Converting .svg files to 'large' .png files..."
mogrify -format png -path ./png/large -colorspace sRGB -density $LDENSITY ./svg/*.svg
echo "Converting .svg files to 'small' .png files..."
mogrify -format png -path ./png/small -colorspace sRGB -density $SDENSITY ./svg/*.svg

FILES="./png/large/*.png"
for f in $FILES
do
  echo "Processing $f..."
  out="./headers/$(basename $f .png | tr -s -c [:alnum:] _)large.h"
  python3 png_to_header.py -i $f -o $out
done

FILES="./png/small/*.png"
for f in $FILES
do
  echo "Processing $f..."
  out="./headers/$(basename $f .png | tr -s -c [:alnum:] _)small.h"
  python3 png_to_header.py -i $f -o $out
done

echo "Done."