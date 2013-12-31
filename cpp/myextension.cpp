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
	SeismicProcess seismicProcess( srcImgData, dstImgData, height, width);
	seismicProcess.executar( numPixelsString, xD, yD );
}



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
        _vXi[i] = _vX[15 - i];
        _vYi[i] = _vY[15 - i];
    }
}

int SeismicProcess::normalize( int value )
{
	return (value + 8) % 8;
}


int SeismicProcess::findFirstWhite( int startIndex, char *vX, char *vY, Point &ini, set<Point> &usedPoints, uchar selfValue )
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
		if( usedPoints.find( p ) != usedPoints.end() )
			continue;

        if( xIndex == 0 || yIndex == 0 || xIndex == src.getWidth() || yIndex == src.getHeight() )
            break;

        uchar currValue = src.pix( xIndex, yIndex );
		if(currValue > selfValue /* || dst.getLum( xIndex, yIndex ) > DST_THRESH */ )
            return normalize(i);
    }    
	return -1;
}



int SeismicProcess::findFirstBlack( int startIndex, char *vX, char *vY, Point &ini, set<Point> &usedPoints, uchar selfValue )
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
		if( usedPoints.find( p ) != usedPoints.end() )
			return E_CLOSED_LOOP; // loop fechado

        if( xIndex == 0 || yIndex == 0 || xIndex == src.getWidth() || yIndex == src.getHeight() )
            break;

        uchar currValue = src.pix(xIndex, yIndex);
        if(currValue <= selfValue /* && dst.getLum( xIndex, yIndex ) < DST_THRESH */ )
            return normalize(i);
    }

    return -1; // não encontrou
}



int SeismicProcess::findWhiteBlackEdge( Point &pointIni, char *vX, char *vY, uchar startIndex, set<Point> &usedPoints, uchar selfValue)
{
	int firstWhiteIndex = findFirstWhite( startIndex, vX, vY, pointIni, usedPoints, selfValue );   
    if( firstWhiteIndex == -1 )
        return E_ALL_BLACK;

    // finds black pixel after the first white pixel
    startIndex = normalize(firstWhiteIndex + 1);
	int firstBlackIndex = findFirstBlack( startIndex, vX, vY, pointIni, usedPoints, selfValue ); 
    if( firstBlackIndex == -1 )
        return E_ALL_WHITE;
	else if ( firstBlackIndex == E_CLOSED_LOOP ) 
		return E_CLOSED_LOOP;

	return firstBlackIndex;
}



int SeismicProcess::findBlackWhiteEdge( Point &pointIni, char *vX, char *vY, uchar startIndex, set<Point> &usedPoints, uchar selfValue)
{
	int firstBlackIndex = findFirstBlack( startIndex, vX, vY, pointIni, usedPoints, selfValue ); 
    if( firstBlackIndex == -1 )
        return E_ALL_WHITE;
	else if ( firstBlackIndex == E_CLOSED_LOOP ) 
		return E_CLOSED_LOOP;
	
    // finds white pixel after the first black pixel
    startIndex = normalize(firstBlackIndex + 1);
	int firstWhiteIndex = findFirstWhite( startIndex, vX, vY, pointIni, usedPoints, selfValue );   
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


void SeismicProcess::paintSumTurn( int sumTurns, int x, int y )
{
	if( sumTurns < 0 )
	{
		#ifdef DEBUG_COLORS
			dst.setRGB(x, y, verdeEscuro);
		#else
			dst.setRGB(x, y, branco);
		#endif
	}
	else
	{
		#ifdef DEBUG_COLORS
			dst.setRGB(x, y, verdeClaro);
		#else
			dst.setRGB(x, y, preto);
		#endif
	}
}


SeismicProcess::SeismicProcess(uchar *srcImgData, uchar *dstImgData, int height, int width ) :
		src(srcImgData, width, height),
		dst(dstImgData, width, height),
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


void SeismicProcess::executar( int numPixelsString, int xD, int yD )
{
	printf("n %d, x %d, y %d\n", numPixelsString, xD, yD);
	dst.setRGB( rosa );

	vector<Point> firstString, secondString;
	Point debugFirstBlackPixel(0, 0);

    printf("Inicializou direcoes\n");

    int nextStartingIndex, blackIndex;
    Point curr(0, 0), lastLeft(0, 0), lastRight(0, 0);
    int difXright, difYright, difXleft, difYleft;
    
    double tan1, tan2, ang1, ang2;
    double conv = 180.0 / 3.1416;
    
    printf("Dims %d, %d\n", src.getHeight(), src.getWidth());

    for(int y(1); y < src.getHeight() - 1; ++y)
    {
        for(int x(1); x < src.getWidth() - 1; ++x)
        {    
            uchar selfValue = src.pix(x, y);
            if( x == xD && y == yD )
                printf("Debug (%d, %d), lum: %d\n", x, y, selfValue);

            nextStartingIndex = 0;
			curr.set(x, y);
			
			//nextStartingIndex = firstBlackIndex;
			bool shouldContinue = false;
			int sumTurns = 0;
			int firstBlackIndex, lastBlackIndex;
			set<Point> usedPoints;
			usedPoints.insert( Point(x, y) );
			// Primeira trilha
			{ 
				// first pixel
				{
					blackIndex = findWhiteBlackEdge( curr, _vX, _vY, nextStartingIndex, usedPoints, selfValue);
					if( blackIndex == E_ALL_BLACK )
					{
						#ifdef DEBUG_COLORS
							dst.setRGB(x, y, vermelho);
						#else
							dst.setRGB(x, y, branco);
						#endif
							
						if( x == xD && y == yD )
							printf("E_ALL_BLACK\n", x, y, selfValue);
						continue;
					}
					else if( blackIndex == E_ALL_WHITE )
					{
						#ifdef DEBUG_COLORS
							dst.setRGB(x, y, violeta);
						#else
							dst.setRGB(x, y, preto);
						#endif
						if( x == xD && y == yD )
							printf("E_ALL_WHITE\n", x, y, selfValue);
						continue;
					}
					firstBlackIndex = blackIndex;
					
					usedPoints.insert( curr );
				}
				lastBlackIndex = firstBlackIndex;
			
				if( x == xD && y == yD )
					debugFirstBlackPixel.set( curr._x + _vX[firstBlackIndex], curr._y + _vY[firstBlackIndex] );

				for(int i(0); i < numPixelsString; ++i)
				{
					// clockwise
					blackIndex = findWhiteBlackEdge( curr, _vX, _vY, nextStartingIndex, usedPoints, selfValue);
					
					if( blackIndex == E_ALL_WHITE )
					{
						#ifdef DEBUG_COLORS
							dst.setRGB(x, y, violeta);
						#else
							dst.setRGB(x, y, preto);
						#endif
						if( x == xD && y == yD )
							printf("E_ALL_WHITE\n", x, y, selfValue);
						shouldContinue = true;
						break;
					}
					else if( blackIndex == E_ALL_BLACK )
					{
						#ifdef DEBUG_COLORS
							dst.setRGB(x, y, vermelho);
						#else
							dst.setRGB(x, y, branco);
						#endif
						if( x == xD && y == yD )
							printf("E_ALL_BLACK\n", x, y, selfValue);
						shouldContinue = true;
						break;
					}
					else if ( blackIndex == E_CLOSED_LOOP )
					{
						paintSumTurn(sumTurns, x, y );
						if( x == xD && y == yD )
							printf("E_CLOSED_LOOP\n", x, y, selfValue);
						shouldContinue = true;
						break;
					}

					curr._x += _vX[blackIndex];
					curr._y += _vY[blackIndex];
					nextStartingIndex = normalize(blackIndex + 4);
					if( x == xD && y == yD )
					{
						firstString.push_back(curr );
						printf("left i: %d, blackIndex: %d, currX: %d, currY:%d, difX: %d, difY: %d\n", i, blackIndex, curr, _vX[blackIndex], _vY[blackIndex]);
					}

					sumTurns += blackIndex - lastBlackIndex;
						
					usedPoints.insert( curr );

					// verifies if the pixel turned back to the first pixel
					if( curr._x == x && curr._y == y && i > 0 )
					{
						if( sumTurns > 0 )
						{
							#ifdef DEBUG_COLORS
								dst.setRGB(x, y, azulClaro);
							#else
								dst.setRGB(x, y, preto);
							#endif
						}
						else
						{
							#ifdef DEBUG_COLORS
								dst.setRGB(x, y, azul);
							#else
								dst.setRGB(x, y, branco);
							#endif
						}
						shouldContinue = true;
						break;
					}

					lastBlackIndex = blackIndex;
				}
			}


            lastLeft = curr;
            if( x == xD && y == yD )
				printf("Sum turn %d, lasts (%d, %d)\n", sumTurns, lastLeft._x, lastLeft._y);

            if( shouldContinue )
                continue;

            curr._x = x;
            curr._y = y;
            bool closed = false;
            sumTurns = 0;

			nextStartingIndex = 8 - firstBlackIndex;
			nextStartingIndex -= 1;
			nextStartingIndex = normalize(nextStartingIndex);
			{
				for(int i(0); i < numPixelsString; ++i)
				{
					// counter-clockwise
					blackIndex = findWhiteBlackEdge( curr, _vXi, _vYi, nextStartingIndex, usedPoints, selfValue);

					curr._x += _vXi[blackIndex];
					curr._y += _vYi[blackIndex];
					nextStartingIndex = normalize(blackIndex + 4);
                
					sumTurns += blackIndex - lastBlackIndex;

					if( blackIndex == E_ALL_WHITE )
					{
						paintSumTurn(sumTurns, x, y );
						if( x == xD && y == yD )
							printf("E_ALL_WHITE\n", x, y, selfValue);
						shouldContinue = true;
						break;
					}
					else if( blackIndex == E_ALL_BLACK )
					{
						#ifdef DEBUG_COLORS
							dst.setRGB(x, y, vermelho);
						#else
							dst.setRGB(x, y, branco);
						#endif
						if( x == xD && y == yD )
							printf("E_ALL_BLACK\n", x, y, selfValue);
						shouldContinue = true;
						break;
					}
					else if ( blackIndex == E_CLOSED_LOOP )
					{
						paintSumTurn(sumTurns, x, y );
						if( x == xD && y == yD )
							printf("E_CLOSED_LOOP\n", x, y, selfValue);
						shouldContinue = true;
						break;
					}

					usedPoints.insert( curr );

					// verifies if the pixel turned back to the first pixel
					if( curr == lastLeft )
					{
						paintSumTurn(sumTurns,  x, y );

						closed = true;
						if( x == xD && y == yD )
						{
							printf("Right met lastLeft (%d, %d) %d\n", lastLeft._x, lastLeft._y, sumTurns);
						}
						break;
					}
					lastBlackIndex = blackIndex;

					if( x == xD && y == yD )
					{
						secondString.push_back( curr );
						printf("right i: %d, blackIndex: %d, currX: %d, currY:%d, difX: %d, difY: %d\n", i, blackIndex, curr, _vXi[blackIndex], _vYi[blackIndex]);
					}
				}
				if( closed )
					continue;

				if( shouldContinue )
					continue;
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

            dst.setLum(x, y, (uchar) val);

            if( x == xD && y == yD )
            {
                printf(" --------------- \n");
                printf("difXright: %d, difYright: %d\n", difXright, difYright);
                printf("difXleft: %d, difYleft: %d\n", difXleft, difYleft);
                printf("tan1: %f, tan2: %f, ang1: %f, ang2: %f, difAng: %f, dist180: %f, val: %f\n", tan1, tan2, ang1, ang2, difAng, dist180, val);
                printf("\n");
            }
	    }
    }

	for( int i(0); i < (int) firstString.size(); ++i)
		dst.setRGB( firstString[i]._x, firstString[i]._y, vermelhoMax );

	for( int i(0); i < (int) secondString.size(); ++i)
		dst.setRGB( secondString[i]._x, secondString[i]._y, verdeMax );

	dst.setRGB( debugFirstBlackPixel._x, debugFirstBlackPixel._y, azulMax );
	dst.setRGB( xD, yD, amarelo );
}

} // end extern "C"

