
#include "HorizonRegions.h"

using namespace std;


HorizonRegions::HorizonRegions(Image8Bits &src, ImageRGB &dst)
{
	initLums(src);
	processPixels(src, dst);
}



void HorizonRegions::initLums(Image8Bits &src)
{
	int width = src.getWidth();
	int height = src.getHeight();
	uchar lum;

	for(int y(0); y < height; ++y)
	{
		for(int x(0); x < width; ++x)
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



void HorizonRegions::processPixels(Image8Bits &src, ImageRGB &dst)
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

						regionsManager.mergeRegions( nRegions[m], nRegions[n] );
					}
			}
		}
	}
}



RegionsManager::RegionsManager( int width, int height ) :
	_width( width ),
	_height( height )
{
	_regionOfPixel = new Region ** [_height];
	for(int y(0); y < _height; ++y)
	{
		_regionOfPixel[y] = new Region * [_width];
		memset( _regionOfPixel[y], 0, _width * sizeof(Region*) );
	}
}



RegionsManager::~RegionsManager()
{
	for(int y(0); y < _height; ++y)
	{
		for(int x(0); x < _width; ++x)
			if(_regionOfPixel[y][x] )
				delete [] _regionOfPixel[y][x];

		delete [] _regionOfPixel[y];
	}

	delete [] _regionOfPixel;
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
	(new Region( *this ))->addPoint( point );
}



void RegionsManager::mergeRegions( Region *region1, Region *region2 )
{
}



Region::Region( RegionsManager regionsManager ) : 
	_regionsManager( regionsManager )
{
}



void Region::addPoint( const Point &point )
{
	_points.push_back( point );
	_regionsManager.setRegion( point, this );
}



