/*
*  File: negativeColorPicker.cpp
*  Implements the negative color picker for CPSC 221 PA2.
*
*/

#include "negativeColorPicker.h"
#include <cmath>
#include <math.h>

NegativeColorPicker::NegativeColorPicker(PNG& inputimg)
{
    img = inputimg;
}

HSLAPixel NegativeColorPicker::operator()(PixelPoint p)
{
    HSLAPixel * negative_pixel = img.getPixel(p.x, p.y);
    HSLAPixel new_pixel;
    new_pixel.h = fmod((negative_pixel->h + 180.0), 360.0);
    new_pixel.s = negative_pixel->s;
    new_pixel.l = 1 - negative_pixel->l;
    new_pixel.a = negative_pixel->a;

    return new_pixel;
}
