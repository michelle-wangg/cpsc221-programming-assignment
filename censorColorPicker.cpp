/*
*  File: censorColorPicker.cpp
*  Implements the censor color picker for CPSC 221 PA2.
*
*/

#include "censorColorPicker.h"

#include <math.h> // gives access to sqrt function

/*
*  Useful function for computing the Euclidean distance between two PixelPoints
*/
double PointDistance(PixelPoint a, PixelPoint b) {
  unsigned int diff_x, diff_y;
  if (a.x > b.x)
    diff_x = a.x - b.x;
  else
    diff_x = b.x - a.x;
  if (a.y > b.y)
    diff_y = a.y - b.y;
  else
    diff_y = b.y - a.y;
  return sqrt(diff_x * diff_x + diff_y * diff_y);
}

CensorColorPicker::CensorColorPicker(unsigned int b_width, PixelPoint ctr, unsigned int rad, PNG& inputimage)
{
  // complete your implementation below
  blockwidth = b_width; // width of a mosaic block in pixels.
  center = ctr;         // middle/center point of mosaic area
  radius = rad;         // maximum pixel distance away from center where the mosaic color will be applied
  img = inputimage;     // Original image used in flood fill algorithm
  blockyimg = inputimage; 
  createBlockyimg();
}

void CensorColorPicker::createBlockyimg()
{
  HSLAPixel avgPixel;
  int height = img.height();
  int width = img.width();

  // case 1: block size fits in img
  if (blockwidth < height && blockwidth < width) {
    for (int y = 0; y < (height/blockwidth)*blockwidth + blockwidth; y += blockwidth) {
      for (int x = 0; x < (width/blockwidth)*blockwidth + blockwidth; x += blockwidth) {
        if (x < width && y < height) {
          avgPixel = getAvgPixel(x, y);
          setBlockyColor(x, y, avgPixel);
          revertBlocks();
        }
      }
    }
    // case 2: block size is greater than img (both dimensions)
  } else if (blockwidth >= height && blockwidth >= width) {
      avgPixel = getAvgPixel(0, 0);
      setBlockyColor(0, 0, avgPixel);
      revertBlocks();
    // case 3: block size is greater than img (height)
  } else if (blockwidth < width) {
      for (int x = 0; x < (width/blockwidth)*blockwidth + blockwidth; x += blockwidth) {
        if (x < width) {
          avgPixel = getAvgPixel(x, 0);
          setBlockyColor(x, 0, avgPixel);
          revertBlocks();
        }
      }
    // case 4: block size is greater than img (width)
  } else {
    for (int y = 0; y < (height/blockwidth)*blockwidth + blockwidth; y += blockwidth) {
        if (y < height) {
          avgPixel = getAvgPixel(0, y);
          setBlockyColor(0, y, avgPixel);
          revertBlocks();
        }
      }
  }
}

HSLAPixel CensorColorPicker::getAvgPixel(int x, int y)
{
  double totalH = 0.0;
  double totalS = 0.0;
  double totalL = 0.0;
  double totalA = 0.0;
  double totalNumPix = 0.0;

  for (int currY = 0; currY < blockwidth; currY++) {
    for (int currX = 0; currX < blockwidth; currX++) {
      if (x + currX < img.width() && y + currY < img.height()) { 
        totalH += img.getPixel(currX+x, currY+y)->h;
        totalS += img.getPixel(currX+x, currY+y)->s;
        totalL += img.getPixel(currX+x, currY+y)->l;
        totalA += img.getPixel(currX+x, currY+y)->a;
        totalNumPix += 1.0;
      }
    }
  }

  // double totalNumPix = (double)(blockwidth * blockwidth);
  HSLAPixel ret = HSLAPixel(totalH/totalNumPix, totalS/totalNumPix, totalL/totalNumPix, totalA/totalNumPix);

  return ret;
}

void CensorColorPicker::setBlockyColor(int x, int y, HSLAPixel avgPixel)
{
  for (int currY = 0; currY < blockwidth; currY++) {
    for (int currX = 0; currX < blockwidth; currX++) {
      if (x + currX < img.width() && y + currY < img.height()) {
        HSLAPixel* target = blockyimg.getPixel(currX+x, currY+y);

        target->h = avgPixel.h;
        target->s = avgPixel.s;
        target->l = avgPixel.l;
        target->a = avgPixel.a;
      }
    }
  }
}

void CensorColorPicker::revertBlocks()
{
  for (int y = 0; y < img.height(); y++) {
    for (int x = 0; x < img.width(); x++) {
      if (PointDistance(center, PixelPoint(x, y, HSLAPixel(0, 0, 0))) > radius) {
        HSLAPixel *target = blockyimg.getPixel(x, y);
        HSLAPixel *ref = img.getPixel(x,y);
        target->h = ref->h;
        target->s = ref->s;
        target->l = ref->l;
        target->a = ref->a;
      }
    }
  }
}

HSLAPixel CensorColorPicker::operator()(PixelPoint p)
{
  // complete your implementation below
  HSLAPixel* target = blockyimg.getPixel(p.x,p.y);
  HSLAPixel ret;

  ret.s =  target -> s;
  ret.h =  target -> h;
  ret.l =  target -> l;
  ret.a =  target -> a;

  return ret; // REPLACE THIS STUB
}
 