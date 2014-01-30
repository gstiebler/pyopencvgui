
#include "HorizonRegions.h"

using namespace std;


extern "C" {

EXPORT void CALL_CONV horizonRegionsExtern(uchar *srcImgData, uchar *dstImgData, int height, int width, int maxHorizontalIntersection, int maxLum, int xD, int yD) 
{
	Image8Bits srcImg( srcImgData, width, height );
	ImageRGB dstImg( dstImgData, width, height );
	HorizonRegions horizonRegions( srcImg );
	horizonRegions.exec( srcImg, dstImg, maxHorizontalIntersection, maxLum,  xD, yD );
}

}



HorizonRegions::HorizonRegions(Image8Bits &src)
{
	initLums(src);
}



void HorizonRegions::initLums(Image8Bits &src)
{
	int width = src.getWidth();
	int height = src.getHeight();
	uchar lum;

	for(int y(1); y < height - 1; ++y)
	{
		for(int x(1); x < width - 1; ++x)
		{
			lum = src.getLum(x, y);
			_lums[lum].push_back(Point(x, y));
		}
	}
}



void HorizonRegions::initDirections(char vX[8], char vY[8])
{
	vX[0] = 0;
    vX[1] = 1;
    vX[2] = 1;
    vX[3] = 1;
    vX[4] = 0;
    vX[5] = -1;
    vX[6] = -1;
    vX[7] = -1;

    vY[0] = -1;
    vY[1] = -1;
    vY[2] = 0;
    vY[3] = 1;
    vY[4] = 1;
    vY[5] = 1;
    vY[6] = 0;
    vY[7] = -1;
}



void HorizonRegions::exec(Image8Bits &src, ImageRGB &dst, int maxHorizontalIntersection, int maxLum, int xD, int yD)
{
	char vX[8], vY[8];
	initDirections(vX, vY);
	RegionsManager regionsManager( src.getWidth(), src.getHeight() );
	Cor black(0x0, 0x0, 0x0);
	dst = src;

	for(uchar lum(0); lum < maxLum; ++lum)
	{
		vector<Point> &currentLums = _lums[lum];
		int size = (int) currentLums.size();
		for(int n(0); n < size; ++n)
		{
			Point &point = currentLums[n];
			vector<Region*> nRegions;

			if( point._x == xD && point._y == yD )
				int x = 5;

			bool hadRegionOnLast = false;
			for(int k(0); k < 8; ++k)
			{
				Point nPoint(point._x + vX[k], point._y + vY[k]);
				Region* region = regionsManager.getRegion( nPoint );
				if( region != NULL )
				{
					if( !hadRegionOnLast )
					{
						nRegions.push_back( region );
						
						if( point._x == xD && point._y == yD )
							printf( "Has neighbour region on: (%d, %d)\n", vX[k], vY[k] );
					}
					hadRegionOnLast = true;
				}
				else
					hadRegionOnLast = false;
			}

			if( nRegions.size() == 0 )
				regionsManager.createRegion( point );
			else if ( nRegions.size() == 1 )
				nRegions[0]->addPoint( point );
			else
			{
				for(int p(0); p < nRegions.size(); ++p)
					for(int q(0); q < nRegions.size(); ++q)
					{
						if(p == q)
							continue;

						regionsManager.mergeRegions( nRegions[p], nRegions[q], point, maxHorizontalIntersection );
					}
			}
		}
	}

	Cor red(0xFF, 0x0, 0x0);
	for(int y(0); y < src.getHeight(); ++y)
		for(int x(0); x < src.getWidth(); ++x)
		{
			Point point(x, y);
			if( !regionsManager.getRegion( point ) )
				dst.setRGB( point, red );
		}
}



RegionsManager::RegionsManager( int width, int height ) :
	_height( height )
{
	_regionOfPixel = new Region ** [_height];
	for(int y(0); y < _height; ++y)
	{
		_regionOfPixel[y] = new Region * [width];
		memset( _regionOfPixel[y], 0, width * sizeof(Region*) );
	}
}



RegionsManager::~RegionsManager()
{
	for(int y(0); y < _height; ++y)
		delete [] _regionOfPixel[y];

	delete [] _regionOfPixel;

	for(int n(0); n < _regions.size(); ++n)
		delete _regions[n];
}



Region* RegionsManager::getRegion( const Point &point )
{
	return _regionOfPixel[point._y][point._x];
}



void RegionsManager::setRegion( const Point &point, Region *region )
{
	_regionOfPixel[point._y][point._x] = region;
}



void RegionsManager::createRegion( const Point &point )
{
	Region *region = new Region( this );
	_regions.push_back( region );
	region->addPoint( point );
}



void RegionsManager::mergeRegions( Region *region1, Region *region2, Point &point, int maxHorizontalIntersection )
{
	region1 = region1->finalRegion();
	region2 = region2->finalRegion();

	if( region1 == region2 )
	{
		region2->addPoint( point );
		return;
	}

	if( region1->horizontalIntersection( region2 ) > maxHorizontalIntersection )
		return;

	region1->merge( region2 );
	region2->addPoint( point );
}



Region::Region( RegionsManager *regionsManager ) : 
	_regionsManager( regionsManager ),
	_active( true ),
	_xMin( 10000000 ),
	_xMax( -1 ),
	_mergedRegion( NULL )
{
}



void Region::addPoint( const Point &point )
{
	_points.push_back( point );
	if( point._x < _xMin )
		_xMin = point._x;

	if( point._x > _xMax )
		_xMax = point._x;

	_regionsManager->setRegion( point, this );
}



void Region::merge( Region *other )
{
	_active = false;

	for(int n(0); n < _points.size(); ++n)
		_regionsManager->setRegion( _points[n], other );

	other->_points.assign( _points.begin(), _points.end() );

	_mergedRegion = other;
}



int Region::length()
{
	return _xMax - _xMin + 1;
}



int Region::horizontalIntersection( Region *other )
{
	int totalXMin = _xMin;
	if( other->_xMin < totalXMin )
		totalXMin = other->_xMin;

	int totalXMax = _xMax;
	if( other->_xMax > totalXMax )
		totalXMax = other->_xMax;

	int totalLength = totalXMax - totalXMin + 1;

	return length() + other->length() - totalLength;
}



Region* Region::finalRegion()
{
	_ASSERT(this != _mergedRegion);

	if( _active )
		return this;
	else
		return _mergedRegion->finalRegion();
}


