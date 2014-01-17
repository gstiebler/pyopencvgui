#ifndef EDGES_H
#define EDGES_H

#include "base.h"
#include "Image8Bits.h"
#include "ImageRGB.h"
#include <vector>

extern "C" {
	
void hEdges2(Image8Bits &src, ImageRGB &dst, int thresh, int delta);
void getAB( std::vector<Point> points, double &a, double &b, double &stdDev );

}

#endif
	
