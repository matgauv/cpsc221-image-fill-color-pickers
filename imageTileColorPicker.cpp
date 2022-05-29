/*
*  File: imageTileColorPicker.cpp
*  Implements the image tile color picker for CPSC 221 PA2.
*
*/

#include "imageTileColorPicker.h"

ImageTileColorPicker::ImageTileColorPicker(PNG& otherimage) {
  img_other = otherimage;
}

HSLAPixel ImageTileColorPicker::operator()(PixelPoint p) {
    HSLAPixel * newPixel;

    unsigned int x_coord = p.x % img_other.width();
    unsigned int y_cord = p.y % img_other.height();

    newPixel = img_other.getPixel(x_coord, y_cord);

    return * newPixel;
}