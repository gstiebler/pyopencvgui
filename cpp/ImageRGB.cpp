#include "ImageRGB.h"

ImageRGB::ImageRGB(uchar *data, int width, int height) :
    _data(data),
    _width(width),
    _height(height)
{
}

void ImageRGB::setRGB( int i, int j, uchar r, uchar g, uchar b )
{
	int lineOffset = j * _width * 3;
	int totalOffset = lineOffset + i * 3;
	uchar *p = &_data[totalOffset];
	*p++ = r;
	*p++ = g;
	*p = b;
}

int ImageRGB::getWidth()
{
    return _width;
}

int ImageRGB::getHeight()
{
    return _height;
}

