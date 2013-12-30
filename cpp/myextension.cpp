#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <vector>
#include <set>

#include "Image8Bits.h"
#include "ImageRGB.h"
#include "base.h"

using namespace std;

#define DST_THRESH 128

#define DEBUG_COLORS

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

void initDirections(char *vX, char *vY, char *vXi, char *vYi)
{
    vX[0] = 0;
    vX[1] = 1;
    vX[2] = 1;
    vX[3] = 1;
    vX[4] = 0;
    vX[5] = -1;
    vX[6] = -1;
    vX[7] = -1;
    memcpy( &vX[8], vX, 8 * sizeof(char));

    vY[0] = -1;
    vY[1] = -1;
    vY[2] = 0;
    vY[3] = 1;
    vY[4] = 1;
    vY[5] = 1;
    vY[6] = 0;
    vY[7] = -1;
    memcpy( &vY[8], vY, 8 * sizeof(char));

    for(int i(0); i < 16; ++i)
    {
        vXi[i] = vX[15 - i];
        vYi[i] = vY[15 - i];
    }
}

int normalize( int value )
{
	return (value + 8) % 8;
}

enum FE_RESPONSE { E_ALL_BLACK = -1, E_ALL_WHITE = -2 };

int findFirstWhite( Image8Bits &src, ImageRGB &dst, int startIndex, int xIni, int yIni, char vX[16], char vY[16], set<Point> &usedPoints, uchar selfValue )
{    
	int deltaX, deltaY;
    int xIndex, yIndex;
	for(int i(startIndex); i < ( startIndex + 8 ); ++i)
    {
		deltaX = vX[i];
		deltaY = vY[i];
        xIndex = xIni + deltaX;
        yIndex = yIni + deltaY;

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



int findFirstBlack( Image8Bits &src, ImageRGB &dst, int startIndex, int xIni, int yIni, char vX[16], char vY[16], set<Point> &usedPoints, uchar selfValue )
{
	int deltaX, deltaY;
    int xIndex, yIndex;
	for(int i(startIndex); i < ( startIndex + 8 ); ++i)
    {
		deltaX = vX[i];
		deltaY = vY[i];
        xIndex = xIni + deltaX;
        yIndex = yIni + deltaY;

		Point p(xIndex, yIndex);
		if( usedPoints.find( p ) != usedPoints.end() )
			continue;

        if( xIndex == 0 || yIndex == 0 || xIndex == src.getWidth() || yIndex == src.getHeight() )
            break;

        uchar currValue = src.pix(xIndex, yIndex);
        if(currValue <= selfValue /* && dst.getLum( xIndex, yIndex ) < DST_THRESH */ )
            return normalize(i);
    }

    return -1;
}



int findWhiteBlackEdge(Image8Bits &src, ImageRGB &dst, int xIni, int yIni, char vX[16], char vY[16], uchar startIndex, set<Point> &usedPoints, uchar selfValue)
{
	int firstWhiteIndex = findFirstWhite( src, dst, startIndex, xIni, yIni, vX, vY, usedPoints, selfValue );   
    if( firstWhiteIndex == -1 )
        return E_ALL_BLACK;

    // finds black pixel after the first white pixel
    startIndex = normalize(firstWhiteIndex + 1);
	int firstBlackIndex = findFirstBlack( src, dst, startIndex, xIni, yIni, vX, vY, usedPoints, selfValue ); 
    if( firstBlackIndex == -1 )
        return E_ALL_WHITE;

	return firstBlackIndex;
}



int findBlackWhiteEdge(Image8Bits &src, ImageRGB &dst, int xIni, int yIni, char vX[16], char vY[16], uchar startIndex, set<Point> &usedPoints, uchar selfValue)
{
	int firstBlackIndex = findFirstBlack( src, dst, startIndex, xIni, yIni, vX, vY, usedPoints, selfValue ); 
    if( firstBlackIndex == -1 )
        return E_ALL_WHITE;
	
    // finds white pixel after the first black pixel
    startIndex = normalize(firstBlackIndex + 1);
	int firstWhiteIndex = findFirstWhite( src, dst, startIndex, xIni, yIni, vX, vY, usedPoints, selfValue );   
    if( firstWhiteIndex == -1 )
        return E_ALL_BLACK;

	return firstWhiteIndex;
}



double quadrantCorrection( double ang, int x, int y)
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
 

__declspec(dllexport) void __stdcall seismicProcess(uchar *srcImgData, uchar *dstImgData, int height, int width, int numPixelsString, int xD, int yD) 
{
    printf("h %d, w %d, n %d, x %d, y %d\n", height, width, numPixelsString, xD, yD);
    Image8Bits src(srcImgData, width, height);
    ImageRGB dst(dstImgData, width, height);
	dst.setLum( 0 );

    char vX[16], vY[16], vXi[16], vYi[16];
    initDirections(vX, vY, vXi, vYi);
	vector<Point> firstString, secondString;
	Point debugFirstBlackPixel(0, 0);

    printf("Inicializou direcoes\n");

    int nextStartingIndex, blackIndex;
    int currX, currY;
    int lastLeftX, lastLeftY, lastRightX, lastRightY;
    int difXright, difYright, difXleft, difYleft;
    
    double tan1, tan2, ang1, ang2;
    double conv = 180.0 / 3.1416;

	Cor violeta(0x70, 0x0, 0x70); // somente mais claros em volta. 
	Cor vermelho(0x70, 0x0, 0x0); // somente pretos em volta
	Cor azul( 0x0, 0x0, 0x70 );
	Cor azulClaro( 0xA0, 0xA0, 0xFF); // Primeira trilha voltou para o primeiro pixel. sumTurns > 0
	Cor verdeClaro( 0xA0, 0xFF, 0xA0 ); // sumTurns >= 0
	Cor verdeEscuro( 0x0, 0x70, 0x0 );
	Cor azulMax( 0x0, 0x0, 0xFF ); // first black pixel. Primeiro pixel escuro partindo do primeiro pixel de debug
	Cor verdeMax( 0x0, 0xFF, 0x0 ); // segunda trilha
	Cor vermelhoMax( 0xFF, 0x0, 0x0 ); // primeira trilha
	Cor amarelo( 0xFF, 0xFF, 0 ); // X e Y selecionado de debug
    
    printf("Dims %d, %d\n", src.getHeight(), src.getWidth());

    for(int y(1); y < src.getHeight() - 1; ++y)
    {
        for(int x(1); x < src.getWidth() - 1; ++x)
        {    
            if( x == xD && y == yD )
                printf("Debug (%d, %d) \n", x, y);
            uchar selfValue = src.pix(x, y);
            nextStartingIndex = 0;
            currX = x;
            currY = y;
			
			//nextStartingIndex = firstBlackIndex;
			bool shouldContinue = false;
			int sumTurns = 0;
			int firstBlackIndex, lastBlackIndex;
			// Primeira trilha
			{ 
				set<Point> usedPoints;
				// first pixel
				{
					blackIndex = findWhiteBlackEdge(src, dst, currX, currY, vX, vY, nextStartingIndex, usedPoints, selfValue);
					if( blackIndex == E_ALL_BLACK )
					{
						#ifdef DEBUG_COLORS
							dst.setRGB(x, y, vermelho);
						#endif
						continue;
					}
					if( blackIndex == E_ALL_WHITE )
					{
						#ifdef DEBUG_COLORS
							dst.setRGB(x, y, violeta);
						#endif
						continue;
					}
					firstBlackIndex = blackIndex;

					usedPoints.insert( Point(x, y) );
				}
				lastBlackIndex = firstBlackIndex;
			
				if( x == xD && y == yD )
					debugFirstBlackPixel.set( currX + vX[firstBlackIndex], currY + vY[firstBlackIndex] );

				for(int i(0); i < numPixelsString; ++i)
				{
					// clockwise
					blackIndex = findWhiteBlackEdge(src, dst, currX, currY, vX, vY, nextStartingIndex, usedPoints, selfValue);
					currX += vX[blackIndex];
					currY += vY[blackIndex];
					nextStartingIndex = normalize(blackIndex + 4);
					if( x == xD && y == yD )
					{
						firstString.push_back(Point( currX, currY ));
						printf("left i: %d, blackIndex: %d, currX: %d, currY:%d, difX: %d, difY: %d\n", i, blackIndex, currX, currY, vX[blackIndex], vY[blackIndex]);
					}

					sumTurns += normalize(lastBlackIndex - blackIndex - 4);
						
					usedPoints.insert( Point(currX, currY) );

					// verifies if the pixel turned back to the first pixel
					if( currX == x && currY == y && i > 0 )
					{
						#ifdef DEBUG_COLORS
							if( sumTurns > 0 )
								dst.setRGB(x, y, azulClaro);
							else
								dst.setRGB(x, y, azul);
						#endif
						shouldContinue = true;
						break;
					}

					lastBlackIndex = blackIndex;
				}
			}


            lastLeftX = currX;
            lastLeftY = currY;
            if( x == xD && y == yD )
                printf("Sum turn %d, lasts (%d, %d)\n", sumTurns, lastLeftX, lastLeftY);

            if( shouldContinue )
                continue;

            currX = x;
            currY = y;
            bool closed = false;
            sumTurns = 0;

			nextStartingIndex = 8 - firstBlackIndex;
			nextStartingIndex -= 1;
			nextStartingIndex = normalize(nextStartingIndex);
			{
				set<Point> usedPoints;
				for(int i(0); i < numPixelsString; ++i)
				{
					// counter-clockwise
					blackIndex = findWhiteBlackEdge(src, dst, currX, currY, vXi, vYi, nextStartingIndex, usedPoints, selfValue); 
					currX += vXi[blackIndex];
					currY += vYi[blackIndex];
					nextStartingIndex = normalize(blackIndex + 4);
                
					sumTurns += normalize(lastBlackIndex - blackIndex - 4);

					usedPoints.insert( Point(currX, currY) );

					// verifies if the pixel turned back to the first pixel
					if( currX == lastLeftX && currY == lastLeftY )
					{
						#ifdef DEBUG_COLORS
							if( sumTurns < 0 )
								dst.setRGB(x, y, verdeEscuro);
							else
								dst.setRGB(x, y, verdeClaro);
						#endif

						closed = true;
						if( x == xD && y == yD )
						{
							printf("Right met lastLeft (%d, %d) %d\n", lastLeftX, lastLeftY, sumTurns);
						}
						break;
					}
					lastBlackIndex = blackIndex;

					if( x == xD && y == yD )
					{
						secondString.push_back(Point( currX, currY ));
						printf("right i: %d, blackIndex: %d, currX: %d, currY:%d, difX: %d, difY: %d\n", i, blackIndex, currX, currY, vXi[blackIndex], vYi[blackIndex]);
					}
				}
				if( closed )
					continue;
			}

            lastRightX = currX;
            lastRightY = currY;

            
            difXright = lastRightX - x;
            difYright = lastRightY - y;
            tan1 = 0.0;
            if(difXright != 0)
                tan1 = - difYright * 1.0 / difXright;
            ang1 = atan( tan1 ) * conv;
            ang1 = quadrantCorrection( ang1, difXright, difYright );
            
            difXleft = lastLeftX - x;
            difYleft = lastLeftY - y;
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

