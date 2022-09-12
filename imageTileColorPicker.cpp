/*
*  File: imageTileColorPicker.cpp
*  Implements the image tile color picker for CPSC 221 PA2.
*
*/

#include "imageTileColorPicker.h"

ImageTileColorPicker::ImageTileColorPicker(PNG& otherimage) {
  // complete your implementation below
  img_other = otherimage;
  
}

HSLAPixel ImageTileColorPicker::operator()(PixelPoint p) {
  int width = img_other.width();
  int height = img_other.height();

  unsigned int x = p.x % width;
  unsigned int y = p.y % height;

  HSLAPixel* target = img_other.getPixel(x,y);

  HSLAPixel ret;
  ret.h =  target->h;
  ret.s =  target->s;
  ret.l =  target->l;

  return ret; // REPLACE THIS STUB
}