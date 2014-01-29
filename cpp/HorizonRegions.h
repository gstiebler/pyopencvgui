#ifndef _HORIZON_REGIONS_
#define _HORIZON_REGIONS_

#include <vector>

#include "base.h"
#include "Image8Bits.h"
#include "ImageRGB.h"

class HorizonRegions
{
public:
	HorizonRegions(Image8Bits &src, ImageRGB &dst);

private:

	void initLums(Image8Bits &src);

	void processPixels(Image8Bits &src, ImageRGB &dst);

	static void initDirections(char vX[8], char vY[8]);

	std::vector<Point> _lums[256];
};

class RegionsManager
{
public:

	Region* getRegion( const Point &point );

	void createRegion( const Point &point );

	void mergeRegions( Region *region1, Region *region2 );
};

class Region
{
public:

	void addPoint( const Point &point );
};

#endif