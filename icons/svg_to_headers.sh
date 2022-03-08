#!/bin/bash
echo "Cleaning old files..."
rm ./png/large/*
rm ./png/small/*
rm ./headers/*

# arguments 1($1) and 2($2) determine the resolution of the output images
# IMAGES MUST HAVE A TOTAL NUMBER OF PIXELS THAT IS DIVISIBLE BY 8
# For sqaure images:
# x = original dimension of icon
# y = desired dimension of icon
# z = density
# In this case we are scaling by 0.25 for better image quality
# ImageMagick default density is 96
# z = 96 * y / (0.25 * x)
LDENSITY=$1 # 2508.8
SDENSITY=$2 # 819.2

echo "Converting .svg files to 'large' .png files..."
mogrify -format png -path ./png/large -colorspace sRGB -density $LDENSITY -resize 25% ./svg/*.svg
echo "Converting .svg files to 'small' .png files..."
mogrify -format png -path ./png/small -colorspace sRGB -density $SDENSITY -resize 25% ./svg/*.svg

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
