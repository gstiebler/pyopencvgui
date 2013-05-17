#include "ImageRGB.h"

ImageRGB::ImageRGB(uchar *data, int width, int height) :
    _data(data),
    _width(width),
    _height(height)
{
}

int ImageRGB::getWidth()
{
    return _width;
}

int ImageRGB::getHeight()
{
    return _height;
}

int ImageRGB::getOffset( int i, int j )
{
	int lineOffset = j * _width * 3;
	return lineOffset + i * 3;
}

void ImageRGB::setRGB( int i, int j, uchar r, uchar g, uchar b )
{
	uchar *p = &_data[getOffset( i, j )];
	*p++ = r;
	*p++ = g;
	*p = b;
}



void ImageRGB::setR( int i, int j, uchar r )
{
	_data[getOffset( i, j )] = r;
}

void ImageRGB::setG( int i, int j, uchar g )
{
	_data[getOffset( i, j ) + 1] = g;
}

void ImageRGB::setB( int i, int j, uchar b )
{
	_data[getOffset( i, j ) + 2] = b;
}


uchar ImageRGB::getR( int i, int j )
{
	return _data[getOffset( i, j )];
}

uchar ImageRGB::getG( int i, int j )
{
	return _data[getOffset( i, j ) + 1];
}

uchar ImageRGB::getB( int i, int j )
{
	return _data[getOffset( i, j ) + 2];
}
