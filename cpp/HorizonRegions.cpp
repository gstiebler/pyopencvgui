
#include "HorizonRegions.h"

using namespace std;


extern "C" {

EXPORT void CALL_CONV horizonRegionsExtern(uchar *srcImgData, uchar *dstImgData, int height, int width, int maxHorizontalIntersection) 
{
	Image8Bits srcImg( srcImgData, width, height );
	ImageRGB dstImg( dstImgData, width, height );
	HorizonRegions horizonRegions( srcImg );
	horizonRegions.exec( srcImg, dstImg, maxHorizontalIntersection );
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



void HorizonRegions::exec(Image8Bits &src, ImageRGB &dst, int maxHorizontalIntersection)
{
	char vX[8], vY[8];
	initDirections(vX, vY);
	RegionsManager regionsManager( src.getWidth(), src.getHeight() );
	Cor red(0xFF, 0x0, 0x0);
	Cor black(0x0, 0x0, 0x0);
	dst = src;

	for(uchar lum(0); lum < 256; ++lum)
	{
		vector<Point> &currentLums = _lums[lum];
		int size = (int) currentLums.size();
		for(int n(0); n < size; ++n)
		{
			Point &point = currentLums[n];
			vector<Region*> nRegions;

			for(int k(0); k < 8; ++k)
			{
				Point nPoint(point._x + vX[k], point._y + vY[k]);
				Region* region = regionsManager.getRegion( nPoint );
				if( region != NULL )
					nRegions.push_back( region );
			}

			if( nRegions.size() == 0 )
				regionsManager.createRegion( point );
			else if ( nRegions.size() == 1 )
				nRegions[0]->addPoint( point );
			else
			{
				dst.setRGB( point, red );

				for(int m(0); m < nRegions.size(); ++m)
					for(int n(0); n < nRegions.size(); ++n)
					{
						if(m == n)
							continue;

						regionsManager.mergeRegions( nRegions[m], nRegions[n], maxHorizontalIntersection );
					}
			}
		}
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



void RegionsManager::mergeRegions( Region *region1, Region *region2, int maxHorizontalIntersection )
{
	region1 = region1->finalRegion();
	region2 = region2->finalRegion();

	if( region1 == region2 )
		return;

	if( region1->horizontalIntersection( region2 ) > maxHorizontalIntersection )
		return;

	region1->merge( region2 );
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


