#include "myextension.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <vector>

using namespace std;

#define DST_THRESH 128

//#define DEBUG_COLORS

extern "C" {
  
__declspec(dllexport) void __stdcall myfunc(uchar *srcImgData, uchar *dstImgData, int height, int width, int thresh)
{
    printf("Largura %d, Altura %d\n", width, height);
    Image8Bits src(srcImgData, width, height);
    Image8Bits dst(dstImgData, width, height);

    uchar uThresh = (uchar) thresh; 
    for(int i(0); i < src.getWidth(); ++i)
    {
        for(int j(0); j < src.getHeight(); ++j)
        {
            if(src.pix(i, j) < uThresh)
                dst.pix(i, j) = 0;
            else
                dst.pix(i, j) = 255;
        }
    }
}



__declspec(dllexport) void __stdcall seismicProcess(uchar *srcImgData, uchar *dstImgData, int height, int width, int numPixelsString, int xD, int yD) 
{
	SeismicProcess seismicProcess( srcImgData, dstImgData, height, width, numPixelsString, xD, yD );
	seismicProcess.executar();
}

} // end extern "C"



void SeismicProcess::initDirections()
{
    _vX[0] = 0;
    _vX[1] = 1;
    _vX[2] = 1;
    _vX[3] = 1;
    _vX[4] = 0;
    _vX[5] = -1;
    _vX[6] = -1;
    _vX[7] = -1;
    memcpy( &_vX[8], _vX, 8 * sizeof(char));

    _vY[0] = -1;
    _vY[1] = -1;
    _vY[2] = 0;
    _vY[3] = 1;
    _vY[4] = 1;
    _vY[5] = 1;
    _vY[6] = 0;
    _vY[7] = -1;
    memcpy( &_vY[8], _vY, 8 * sizeof(char));

    for(int i(0); i < 16; ++i)
    {
        _vXi[i] = _vX[i] * -1;
        _vYi[i] = _vY[i];
    }
}

int SeismicProcess::normalize( int value )
{
	return (value + 8) % 8;
}


int SeismicProcess::findFirstWhite( int startIndex, char *vX, char *vY, Point &ini, uchar selfValue )
{    
	int deltaX, deltaY;
    int xIndex, yIndex;
	for(int i(startIndex); i < ( startIndex + 8 ); ++i)
    {
		deltaX = vX[i];
		deltaY = vY[i];
		xIndex = ini._x + deltaX;
		yIndex = ini._y + deltaY;

        if( xIndex == 0 || yIndex == 0 || xIndex == src.getWidth() || yIndex == src.getHeight() )
            break;

        uchar currValue = src.pix( xIndex, yIndex );
		if(currValue > selfValue /* || dst.getLum( xIndex, yIndex ) > DST_THRESH */ )
            return normalize(i);
    }    
	return -1;
}



int SeismicProcess::findFirstBlack( int startIndex, char *vX, char *vY, Point &ini, Point &previousPoint, Point &specialPoint, uchar selfValue )
{
	int deltaX, deltaY;
    int xIndex, yIndex;
	for(int i(startIndex); i < ( startIndex + 8 ); ++i)
    {
		deltaX = vX[i];
		deltaY = vY[i];
		xIndex = ini._x + deltaX;
		yIndex = ini._y + deltaY;

		Point p(xIndex, yIndex);
		if( p == previousPoint )
			return E_PREVIOUS_POINT;
		if( p == specialPoint ) 
			return E_SPECIAL_POINT;

        if( xIndex == 0 || yIndex == 0 || xIndex == src.getWidth() || yIndex == src.getHeight() )
            break;

        uchar currValue = src.pix(xIndex, yIndex);
        if(currValue <= selfValue /* && dst.getLum( xIndex, yIndex ) < DST_THRESH */ )
            return normalize(i);
    }

    return -1; // não encontrou
}



int SeismicProcess::findWhiteBlackEdge( Point &pointIni, char *vX, char *vY, uchar startIndex, Point &previousPoint, Point &specialPoint, uchar selfValue)
{
	int firstWhiteIndex = findFirstWhite( startIndex, vX, vY, pointIni, selfValue );   
    if( firstWhiteIndex == -1 )
        return E_ALL_BLACK;

    // finds black pixel after the first white pixel
    startIndex = normalize(firstWhiteIndex + 1);
	int firstBlackIndex = findFirstBlack( startIndex, vX, vY, pointIni, previousPoint, specialPoint, selfValue ); 
    if( firstBlackIndex == -1 )
        return E_ALL_WHITE;
	// valores especiais
	else if ( firstBlackIndex < -2 ) 
		return firstBlackIndex;

	return firstBlackIndex;
}



int SeismicProcess::findBlackWhiteEdge( Point &pointIni, char *vX, char *vY, uchar startIndex, Point &previousPoint, Point &specialPoint, uchar selfValue)
{
	int firstBlackIndex = findFirstBlack( startIndex, vX, vY, pointIni, previousPoint, specialPoint, selfValue ); 
    if( firstBlackIndex == -1 )
        return E_ALL_WHITE;
	else if ( firstBlackIndex < -2 ) 
		return firstBlackIndex;
	
    // finds white pixel after the first black pixel
    startIndex = normalize(firstBlackIndex + 1);
	int firstWhiteIndex = findFirstWhite( startIndex, vX, vY, pointIni, selfValue );   
	// valores especiais
    if( firstWhiteIndex == -1 )
        return E_ALL_BLACK;

	return firstWhiteIndex;
}



double SeismicProcess::quadrantCorrection( double ang, int x, int y)
{
    if(x == 0)
    {
        if(y > 0)
            return -90.0;
        else
            return -90.0;
    }
    else if (x > 0)
        return ang;
    else
    {
        ang += 180.0;
        if(ang > 360.0)
            ang -= 360.0;
        return ang;
    }
} 


Cor SeismicProcess::paintSumTurn( int sumTurns, int x, int y )
{
	if( sumTurns < 0 )
	{
		#ifdef DEBUG_COLORS
			return verdeEscuro;
		#else
			return branco;
		#endif
	}
	else
	{
		#ifdef DEBUG_COLORS
			return verdeClaro;
		#else
			return preto;
		#endif
	}
}


void SeismicProcess::calcSumTurn( int &sumTurns, int blackIndex, int lastBlackIndex )
{
	int stTemp = normalize( blackIndex - lastBlackIndex );
	if( stTemp <= 4 )
		sumTurns += stTemp;
	else
		sumTurns -= 8 - stTemp;
}


SeismicProcess::SeismicProcess(uchar *srcImgData, uchar *dstImgData, int height, int width, int numPixelsString, int xD, int yD  ) :
		src(srcImgData, width, height),
		dst(dstImgData, width, height),
		_numPixelsString( numPixelsString ),
		_xD( xD ),
		_yD( yD ),
		violeta(0x70, 0x0, 0x70),
		vermelho(0x70, 0x0, 0x0),
		azul( 0x0, 0x0, 0x70 ),
		azulClaro( 0xA0, 0xA0, 0xFF),
		verdeClaro( 0xA0, 0xFF, 0xA0 ),
		verdeEscuro( 0x0, 0x70, 0x0 ),
		azulMax( 0x0, 0x0, 0xFF ),
		verdeMax( 0x0, 0xFF, 0x0 ),
		vermelhoMax( 0xFF, 0x0, 0x0 ),
		amarelo( 0xFF, 0xFF, 0 ),
		branco( 0xFF, 0xFF, 0xFF ),
		preto( 0x0, 0x0, 0x0 ),
		rosa( 0xFF, 0xD0, 0xD0 )
{
	printf("h %d, w %d\n", height, width );
	 
    initDirections();
}



SeismicProcess::~SeismicProcess()
{
}


void SeismicProcess::executar()
{
	printf("n %d, x %d, y %d\n", _numPixelsString, _xD, _yD);
	dst.setRGB( rosa );

	const int THRESH = 128;

	const int MIN_VALUE = -128;
	const int MAX_VALUE = 127;

	vector<Point> firstString, secondString;
	Point debugFirstBlackPixel(0, 0);
    
    printf("Dims %d, %d\n", src.getHeight(), src.getWidth());

    for(int y(1); y < src.getHeight() - 1; ++y)
    {
        for(int x(1); x < src.getWidth() - 1; ++x)
        {    
            uchar selfValue = src.pix(x, y);

            if( x == _xD && y == _yD )
                printf("Debug (%d, %d), lum: %d\n", x, y, selfValue);

			int minValue = selfValue + MIN_VALUE;
			int maxValue = selfValue + MAX_VALUE;

			if( minValue < 0 )
				minValue = 0;

			if( maxValue > 255 )
				maxValue = 255;

			int currValue;

			while( (maxValue - minValue) > 1 )
			{
				currValue = (maxValue + minValue) / 2;
				Cor color = processPixel( x, y, currValue );

				if( color.r > 128 )
					maxValue = currValue;
				else
					minValue = currValue;
			}

			uchar finalValue = (uchar) ( minValue - selfValue - MIN_VALUE );

			dst.setRGB( x, y, Cor( finalValue, finalValue, finalValue ) );

			//if( x == _xD && y == _yD )
			//{
			//	printf(" --------------- \n");
			//	printf("difXright: %d, difYright: %d\n", difXright, difYright);
			//	printf("difXleft: %d, difYleft: %d\n", difXleft, difYleft);
			//	printf("tan1: %f, tan2: %f, ang1: %f, ang2: %f, difAng: %f, dist180: %f, val: %f\n", tan1, tan2, ang1, ang2, difAng, dist180, val);
			//	printf("\n");
			//}

	    }
    }

	for( int i(0); i < (int) firstString.size(); ++i)
		dst.setRGB( firstString[i]._x, firstString[i]._y, vermelhoMax );

	for( int i(0); i < (int) secondString.size(); ++i)
		dst.setRGB( secondString[i]._x, secondString[i]._y, verdeMax );

	dst.setRGB( debugFirstBlackPixel._x, debugFirstBlackPixel._y, azulMax );
	dst.setRGB( _xD, _yD, amarelo );
}



Cor SeismicProcess::processPixel( int x, int y, int selfValue )
{
    int nextStartingIndex, blackIndex;
    Point curr(0, 0), lastLeft(0, 0), lastRight(0, 0);
    int difXright, difYright, difXleft, difYleft;
	int sumTurns = 0;
	int firstBlackIndex, lastBlackIndex;
    
    double tan1, tan2, ang1, ang2;
    double conv = 180.0 / 3.1416;

    nextStartingIndex = 0;
			
	Point previousPoint(x, y);
	Point specialPoint(x, y);
	//nextStartingIndex = firstBlackIndex;
	// Primeira trilha
	{ 
		curr.set(x, y);
		// first pixel
		{
			blackIndex = findWhiteBlackEdge( curr, _vX, _vY, nextStartingIndex, previousPoint, specialPoint, selfValue);
			if( blackIndex == E_ALL_BLACK )
			{
				#ifdef DEBUG_COLORS
					return vermelho;
				#else
					return branco;
				#endif
							
				//if( x == _xD && y == _yD )
				//	printf("E_ALL_BLACK\n", x, y, selfValue);
				return branco;
			}
			else if( blackIndex == E_ALL_WHITE )
			{
				#ifdef DEBUG_COLORS
					return violeta;
				#else
					return preto;
				#endif
				//if( x == _xD && y == _yD )
				//	printf("E_ALL_WHITE\n", x, y, selfValue);
				return preto;
			}
			firstBlackIndex = blackIndex;
		}
		lastBlackIndex = firstBlackIndex;
			
		//if( x == _xD && y == _yD )
		//	debugFirstBlackPixel.set( curr._x + _vX[firstBlackIndex], curr._y + _vY[firstBlackIndex] );

		for(int i(0); i < _numPixelsString; ++i)
		{
			// clockwise
			blackIndex = findWhiteBlackEdge( curr, _vX, _vY, nextStartingIndex, previousPoint, specialPoint, selfValue);
			previousPoint = curr;

			curr._x += _vX[blackIndex];
			curr._y += _vY[blackIndex];
			nextStartingIndex = normalize(blackIndex + 4);

			calcSumTurn( sumTurns, blackIndex, lastBlackIndex );
					
			if( blackIndex == E_SPECIAL_POINT )
			{
				Cor ctemp = paintSumTurn(sumTurns, x, y );
				//if( x == _xD && y == _yD )
				//	printf("E_SPECIAL_POINT, sumTurns: %d\n", sumTurns);
				return ctemp;
			}
			else if( blackIndex == E_PREVIOUS_POINT )
			{
				//if( x == _xD && y == _yD )
				//	printf("E_PREVIOUS_POINT\n", x, y, selfValue);

				#ifdef DEBUG_COLORS
					return violeta;
				#else
					return preto;
				#endif
			}
			//if( x == _xD && y == _yD )
			//{
			//	firstString.push_back(curr );
			//	printf("left i: %d, blackIndex: %d, currX: %d, currY:%d, difX: %d, difY: %d, sumTurns: %d\n", i, blackIndex, curr, _vX[blackIndex], _vY[blackIndex], sumTurns);
			//}

			// verifies if the pixel turned back to the first pixel
			if( curr._x == x && curr._y == y && i > 0 )
			{
				if( sumTurns > 0 )
				{
					#ifdef DEBUG_COLORS
						return azulClaro;
					#else
						return preto;
					#endif
				}
				else
				{
					#ifdef DEBUG_COLORS
						return azul;
					#else
						return branco;
					#endif
				}
			}

			lastBlackIndex = blackIndex;
		}
	}


    lastLeft = curr;
	specialPoint = lastLeft;
	previousPoint.set(x, y);
  //  if( x == _xD && y == _yD )
		//printf("Sum turn %d, lasts (%d, %d)\n", sumTurns, lastLeft._x, lastLeft._y);

    curr.set(x, y);
    //sumTurns = 0;

	nextStartingIndex = normalize( 8 - firstBlackIndex );
	lastBlackIndex = normalize( 4 - nextStartingIndex );
	{
		for(int i(0); i < _numPixelsString; ++i)
		{
			// counter-clockwise
			blackIndex = findWhiteBlackEdge( curr, _vXi, _vYi, nextStartingIndex, previousPoint, specialPoint, selfValue);
			previousPoint = curr;

			curr._x += _vXi[blackIndex];
			curr._y += _vYi[blackIndex];
			nextStartingIndex = normalize(blackIndex + 4);
                
			calcSumTurn( sumTurns, blackIndex, lastBlackIndex );

			if( blackIndex == E_SPECIAL_POINT )
			{
				Cor ctemp = paintSumTurn(sumTurns, x, y );
				//if( x == _xD && y == _yD )
				//	printf("E_SPECIAL_POINT, sumTurns: %d\n", sumTurns);

				return ctemp;
			}
			else if( blackIndex == E_PREVIOUS_POINT )
			{
				//if( x == _xD && y == _yD )
				//	printf("E_PREVIOUS_POINT\n", x, y, selfValue);

				#ifdef DEBUG_COLORS
					return violeta;
				#else
					return preto;
				#endif
			}

			// verifies if the pixel turned back to the first pixel
			if( curr == lastLeft )
			{
				Cor ctemp = paintSumTurn(sumTurns,  x, y );

				//if( x == _xD && y == _yD )
				//	printf("Right met lastLeft (%d, %d) %d\n", lastLeft._x, lastLeft._y, sumTurns);

				return ctemp;
			}
			lastBlackIndex = blackIndex;

			//if( x == _xD && y == _yD )
			//{
			//	secondString.push_back( curr );
			//	printf("right i: %d, blackIndex: %d, currX: %d, currY:%d, difX: %d, difY: %d, sumTurns: %d\n", i, blackIndex, curr, _vXi[blackIndex], _vYi[blackIndex], sumTurns);
			//}
		}
	}

    lastRight = curr;
            
    difXright = lastRight._x - x;
    difYright = lastRight._y - y;
    tan1 = 0.0;
    if(difXright != 0)
        tan1 = - difYright * 1.0 / difXright;
    ang1 = atan( tan1 ) * conv;
    ang1 = quadrantCorrection( ang1, difXright, difYright );
            
    difXleft = lastLeft._x- x;
    difYleft = lastLeft._y - y;
    tan2 = 0.0;
    if(difXleft != 0)
        tan2 = - difYleft * 1.0 / difXleft;
    ang2 = atan( tan2 ) * conv;
    ang2 = quadrantCorrection( ang2, difXleft, difYleft );

    double difAng = ang1 - ang2;
	if(difAng < 0.0)
		difAng += 360.0;

	double dist180 = 180.0 - difAng;
	if(dist180 < 0.0)
		dist180 = 0.0;

    double val = (255.0 / 180.0) * dist180;

	uchar ucharVal = (uchar) val;
    return Cor( ucharVal, ucharVal, ucharVal );
}

