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

CensorColorPicker::CensorColorPicker(unsigned int b_width, PixelPoint ctr, unsigned int rad, PNG &inputimage) {
    blockwidth = b_width;
    center = ctr;
    radius = rad;
    img = inputimage;
    blockyimg = CreateBlockImage();

}

PNG CensorColorPicker::CreateBlockImage() {
    PNG blocky;
    blocky.resize(img.width(), img.height());
    for (unsigned int y = 0; y < img.height(); y += blockwidth) {
        for (unsigned int x = 0; x < img.width(); x += blockwidth) {
            HSLAPixel *average_pixel = GetAveragePixel(x, y);
            ApplyToWholeBlock(blocky, average_pixel, x, y);

        }
    }
    return blocky;
}

// returns average HSLA pixel for block, given the upper left most pixel in the block
HSLAPixel *CensorColorPicker::GetAveragePixel(unsigned int x, unsigned int y) {
    double sum_h = 0;
    double sum_s = 0;
    double sum_l = 0;
    double sum_a = 0;
    double block_count = 0;

    for (unsigned int i = x; i < x + blockwidth; i++) {
        for (unsigned int j = y; j < y + blockwidth; j++) {
            if (i < img.width() && j < img.height()) {
                HSLAPixel *curr = img.getPixel(i, j);
                sum_h += curr->h;
                sum_s += curr->s;
                sum_l += curr->l;
                sum_a += curr->a;
                block_count++;
            }
        }
    }

    return AverageColor(sum_h, sum_s, sum_l, sum_a, block_count);
}

HSLAPixel *CensorColorPicker::AverageColor(double sum_h, double sum_s, double sum_l, double sum_a, double block_count) {
    double avg_h = sum_h / block_count;
    double avg_s = sum_s / block_count;
    double avg_l = sum_l / block_count;
    double avg_a = sum_a / block_count;

    HSLAPixel *averagePixel = new HSLAPixel(avg_h, avg_s, avg_l, avg_a);
    return averagePixel;
}

void CensorColorPicker::ApplyToWholeBlock(PNG &blocky, HSLAPixel *average_pixel, unsigned int x, unsigned int y) {
    for (unsigned int i = x; i < x + blockwidth; i++) {
        for (unsigned int j = y; j < y + blockwidth; j++) {
            if (i < img.width() && j < img.height()) {
                HSLAPixel *curr = blocky.getPixel(i, j);
                *curr = *average_pixel;
            }
        }
    }
}

HSLAPixel CensorColorPicker::operator()(PixelPoint p) {
    HSLAPixel * pixel;
    if (PointDistance(p, center) <= radius) {
        pixel = blockyimg.getPixel(p.x, p.y);
    } else {
        pixel = img.getPixel(p.x, p.y);
    }
    return *pixel;
}


