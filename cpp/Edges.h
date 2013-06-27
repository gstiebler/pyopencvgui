#ifndef EDGES_H
#define EDGES_H

#include "base.h"
#include "Image8Bits.h"
#include "ImageRGB.h"
#include <vector>

struct Point
{
	Point( int x, int y );
	int _x, _y;
};

extern "C" {
	
__declspec(dllexport) void __stdcall hEdges(uchar *srcImgData, uchar *dstImgData, int height, int width, int thresh, int delta);
__declspec(dllexport) void __stdcall hEdgesMaxDif(uchar *srcImgData, uchar *dstImgData, int height, int width, int thresh, int delta);
__declspec(dllexport) void __stdcall hEdgesMiddle(uchar *srcImgData, uchar *dstImgData, int height, int width, int thresh, int delta, int minGreen, int minGray, int minRed);
void hEdges2(Image8Bits &src, ImageRGB &dst, int thresh, int delta);
void getAB( std::vector<Point> points, double &a, double &b );

}

#endif
	