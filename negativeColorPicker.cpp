/*
*  File: negativeColorPicker.cpp
*  Implements the negative color picker for CPSC 221 PA2.
*
*/

#include "negativeColorPicker.h"
#include <math.h>

NegativeColorPicker::NegativeColorPicker(PNG& inputimg)
{
  // complete your implementation below
  img = inputimg;
  
}

HSLAPixel NegativeColorPicker::operator()(PixelPoint p)
{
  // complete your implementation below

  HSLAPixel* target = img.getPixel(p.x,p.y);

  HSLAPixel ret;
  ret.s =  target->s;
  if (target->h + 180.0 <= 360.0) {
    ret.h = (target->h) + 180.0;
  } else {
    ret.h = 180.0 - (360.0 - (target->h));
  }
  
  ret.l =  1.0 - (target->l);

  return ret; // REPLACE THIS STUB
}
