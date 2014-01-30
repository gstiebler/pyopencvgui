#ifndef _HORIZON_REGIONS_
#define _HORIZON_REGIONS_

#include <vector>

#include "base.h"
#include "Image8Bits.h"
#include "ImageRGB.h"

class HorizonRegions
{
public:
	/**
	 * Constructor
	 * @param[in] src Source image
	 * @param[out] dst Destination image
	 */
	HorizonRegions(Image8Bits &src);

	/**
	 * The main function of the class
	 * @param[in] src Source image
	 * @param[out] dst Destination image
	 */
	void exec(Image8Bits &src, ImageRGB &dst, int maxHorizontalIntersection, int maxLum, int xD, int yD);

private:

	/**
	 * Initializes the lums vector
	 * @param[in] src Source image
	 */
	void initLums(Image8Bits &src);

	/**
	 * Initializes the direction vectors to the 8 neighbours of the pixels
	 * @param[out] vX X coords of the neighbour pixels
	 * @param[out] vY Y coords of the neighbour pixels
	 */
	static void initDirections(char vX[8], char vY[8]);

	/** Source pixels ordered by luminosity */
	std::vector<Point> _lums[256];
};


class Region;

class RegionsManager
{
public:

	/**
	 * Constructor
	 * @param[in] width Width of the source image
	 * @param[in] height Height of the source image
	 */
	RegionsManager( int width, int height );

	/**
	 * Destructor
	 */
	~RegionsManager();

	/**
	 * Returns the region of a point
	 * @param[in] point The point of the region
	 * @return The region of the point
	 */
	Region* getRegion( const Point &point );

	/**
	 * Sets the region of a point
	 * @param[in] point The point of the region
	 * @param[in] region The region of the point
	 */
	void setRegion( const Point &point, Region *region );

	/**
	 * Creates a region from a point
	 * @param[in] point First point of the region
	 */
	void createRegion( const Point &point );

	/**
	 * Merges 2 regions (if applicable)
	 * @param[in] region1 First region
	 * @param[in] region2 Second region
	 */
	static void mergeRegions( Region *region1, Region *region2, Point &point, int maxHorizontalIntersection );

private:

	/** All the regions */
	std::vector<Region*> _regions;

	/** Region of each pixel */
	Region ***_regionOfPixel;

	int _height;
};



class Region
{
public:

	/**
	 * Constructor
	 * @param[in,out] regionsManager The unique and only regions manager
	 */
	Region( RegionsManager *regionsManager );

	/**
	 * Adds a point to the region
	 * @param[in] point The point to be added
	 */
	void addPoint( const Point &point );

	void merge( Region *other );

	int horizontalIntersection( Region *other );

	Region* finalRegion();

	int length();

	int _xMin, _xMax;

private:

	/** The points of the region */
	std::vector<Point> _points;

	/** The unique and only regions manager */
	RegionsManager *_regionsManager;

	bool _active;

	Region *_mergedRegion;
};

#endif