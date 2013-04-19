#include "Image.h"

Image::Image(uchar *data, int width, int height) :
    _data(data),
    _width(width),
    _height(height)
{
}

uchar& Image::pix(int i, int j)
{
    return _data[j * _width + i];
}

int Image::getWidth()
{
    return _width;
}

int Image::getHeight()
{
    return _height;
}

