#include "Image8Bits.h"

Image8Bits::Image8Bits(uchar *data, int width, int height) :
    _data(data),
    _width(width),
    _height(height)
{
}

uchar& Image8Bits::pix(int i, int j)
{
    return _data[j * _width + i];
}

int Image8Bits::getWidth()
{
    return _width;
}

int Image8Bits::getHeight()
{
    return _height;
}

