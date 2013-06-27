#include "Edges.h"

#include <algorithm>  

using namespace std;

Point::Point( int x, int y )
{
	_x = x;
	_y = y;
}

__declspec(dllexport) void __stdcall hEdges(uchar *srcImgData, uchar *dstImgData, int height, int width, int thresh, int delta)
{
	Image8Bits src(srcImgData, width, height);
	ImageRGB dst(dstImgData, width, height);

	int dif;
	for( int j(0); j < src.getHeight(); ++j)
	{
		for( int i(0); i < src.getWidth() - delta; ++i)
		{
			dif = src.pix( i, j ) - src.pix( i + delta, j );
			if( dif > thresh )
				dst.setRGB( i, j, 0x0, 0xFF, 0x0 );
			else if( dif < -thresh )
				dst.setRGB( i + delta, j, 0xFF, 0x0, 0x0 );
		}
	}
}

__declspec(dllexport) void __stdcall hEdgesMaxDif(uchar *srcImgData, uchar *dstImgData, int height, int width, int thresh, int delta)
{
	Image8Bits src(srcImgData, width, height);
	ImageRGB dst(dstImgData, width, height);

	int dif;
	bool lastDifP = false, lastDifM = false;
	int maxDifP = 0, maxDifM = 0;
	int indexMaxDifP, indexMaxDifM;
	for( int j(0); j < src.getHeight(); ++j)
	{
		for( int i(0); i < src.getWidth() - delta; ++i)
		{
			dif = src.pix( i, j ) - src.pix( i + delta, j );
			if( dif > thresh )
			{
				dst.setR( i, j, 0x0 );
				dst.setB( i, j, 0x0 );
				lastDifP = true;

				if(dif > maxDifP)
				{
					maxDifP = dif;
					indexMaxDifP = i;
				}
			}
			else
			{
				if(lastDifP)
					dst.setRGB( indexMaxDifP, j, 0x0, 0xFF, 0x0 );

				lastDifP = false;
				maxDifP = 0;
			}

			if( -dif > thresh )
			{
				dst.setG( i + delta, j, 0x0 );
				dst.setB( i + delta, j, 0x0 );
				lastDifM = true;

				if(-dif > maxDifM)
				{
					maxDifM = -dif;
					indexMaxDifM = i + delta;
				}
			}
			else
			{
				if(lastDifM)
					dst.setRGB( indexMaxDifM, j, 0xFF, 0x0, 0x0 );

				lastDifM = false;
				maxDifM = 0;
			}
		}
	}
}


void hEdges2(Image8Bits &src, ImageRGB &dst, int thresh, int delta)
{
	int dif;
	for( int j(0); j < src.getHeight(); ++j)
	{
		for( int i(0); i < src.getWidth() - delta; ++i)
		{
			dif = src.pix( i, j ) - src.pix( i + delta, j );
			if( dif > thresh )
			{
				dst.setG( i, j, 0xFF );
				dst.setB( i, j, 0x0 );
			}
			else if( dif < -thresh )
			{
				dst.setR( i + delta, j, 0xFF );
				dst.setB( i + delta, j, 0x0 );
			}
		}
	}
}



void getAB( std::vector<Point> points, double &a, double &b )
{
	double factor = 0.1;
	int margin = (int)(factor * points.size());
	int halfSize = points.size() / 2;
	vector<double> as;
	double currA;
	int difX, difY;

	for(int p(margin); p < halfSize; ++p)
	{
		difX = points[p]._x - points[p + halfSize]._x;
		difY = points[p]._y - points[p + halfSize]._y;
		if( difY != 0)
			currA = difX * 1.0 / difY;
		as.push_back( currA );
	}

	sort( as.begin(), as.end() );
	a = as[ as.size() / 2 ];

	vector<double> bs;
	double currB;
	for(int p(margin); p < (int)points.size() - margin; ++p)
	{
		currB = points[p]._x - a * points[p]._y;
		bs.push_back( currB );
	}
	
	sort( bs.begin(), bs.end() );
	b = bs[ bs.size() / 2 ];
}



__declspec(dllexport) void __stdcall hEdgesMiddle(uchar *srcImgData, uchar *dstImgData, int height, int width, int *intStatsOutput, double *doubleStatsOutput,  
												  int thresh, int delta, int minGreen, int minGray, int minRed)
{
	Image8Bits src(srcImgData, width, height);
	ImageRGB dst(dstImgData, width, height);
	hEdges2( src, dst, thresh, delta );

	int countGreen, countGray, countRed;
	int firstGray, middleGray;
	enum E_COLOR { E_NONE, E_GREEN, E_GRAY, E_RED };
	E_COLOR state, lastPixel;
	uchar r, g, b;
	vector<Point> leftPoints; 
	vector<Point> rightPoints;

	for( int j(0); j < src.getHeight(); ++j)
	{
		countGreen = countGray = countRed = 0;
		state = E_NONE;
		lastPixel = E_NONE;
		firstGray = -1;
		for( int i(0); i < src.getWidth() - delta; ++i)
		{
			if( i == 164 && j == 142 )
				int k = 3;

			r = dst.getR( i, j );
			g = dst.getG( i, j );
			b = dst.getB( i, j );
            //if red
			if( b == 0x0 && r == 0xFF )
			{
				countGreen = countGray = 0;
				countRed++;

				if( lastPixel == E_GRAY )
					middleGray = (firstGray + i) / 2;

				lastPixel = E_RED;

				if( state == E_GREEN )
				{
					state = E_NONE;
					firstGray = -1;
				}
				else if( state == E_GRAY && countRed >= minRed )
				{
					state = E_NONE;
					firstGray = -1;

					if( middleGray < src.getWidth() / 2)
						leftPoints.push_back( Point( middleGray, j ) );
					else
						rightPoints.push_back( Point( middleGray, j ) );


					dst.setRGB( middleGray, j, 0x0, 0xFF, 0xFF ); // set cyan
				}
			}
			else if( b == 0x0 && g == 0xFF ) // if green
			{
				countGray = countRed = 0;
				countGreen++;

				state = E_NONE;
				lastPixel = E_GREEN;

				if( countGreen >= minGreen )
					state = E_GREEN;

				firstGray = -1;
			}
			else 
			{
				countGreen = countRed = 0;
				countGray++;
				lastPixel = E_GRAY;

				if( firstGray < 0 )
					firstGray = i;
				
				if( state == E_RED )
					state = E_NONE;
				else if( state == E_GREEN && countGray >= minGray )
					state = E_GRAY;
			}
		}
	}

	double A1, B1, A2, B2, squareWidth;
	getAB( leftPoints, A1, B1 );
	printf("A and B: %lf %lf\n", A1, B1 );
	getAB( rightPoints, A2, B2 );
	printf("A and B: %lf %lf\n", A2, B2 );
	squareWidth = B2 - B1;
	printf("Width: %lf\n", squareWidth);

	doubleStatsOutput[0] = A1;
	doubleStatsOutput[1] = B1;
	doubleStatsOutput[2] = A2;
	doubleStatsOutput[3] = B2;
	doubleStatsOutput[4] = squareWidth;
}
