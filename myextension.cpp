#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#include "Image.h"

extern "C" {
 
typedef unsigned char uchar;
typedef long int lint;
  
int myfunc(uchar *srcImgData, uchar *dstImgData, int height, int width, int thresh)
{
    Image src(srcImgData, width, height);
    Image dst(dstImgData, width, height);

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
  
    return 0;  
}

void initDirections(char vX[16], char vY[16], char vXi[16], char vYi[16])
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


uchar findEdge(Image &src, int xIni, int yIni, char vX[16], char vY[16], uchar startIndex, uchar selfValue)
{
    printf("findEdge xIni: %d, yIni: %d, selfValue: %d\n", xIni, yIni, selfValue);

    // finds white pixel
    int whiteIndex = 0;
    int xIndex, yIndex;
    for(int i(startIndex); i < ( startIndex + 8 ); ++i)
    {
        xIndex = xIni + vX[i];
        yIndex = yIni + vY[i];
        if( xIndex == 0 || yIndex == 0 || xIndex == src.getWidth() || yIndex == src.getHeight() )
            break;

        uchar currValue = src.pix( xIndex, yIndex );

        printf("i: %d, xIndex: %d, yIndex: %d, currValue: %d\n", i, xIndex, yIndex, currValue);
        if(currValue > selfValue)
        {
            whiteIndex = i % 8;
            break;
        }
    }    
    printf("whiteIndex: %d\n", whiteIndex);

    // finds black pixel
    startIndex = (whiteIndex + 1) % 8;
    for(int i(startIndex); i < ( startIndex + 8 ); ++i)
    {
        xIndex = xIni + vX[i];
        yIndex = yIni + vY[i];
        if( xIndex == 0 || yIndex == 0 || xIndex == src.getWidth() || yIndex == src.getHeight() )
            break;

        uchar currValue = src.pix(xIndex, yIndex);
        printf("i: %d, xIndex: %d, yIndex: %d, currValue: %d\n", i, xIndex, yIndex, currValue);
        if(currValue <= selfValue)
            return i % 8;
    }
}



void seismicProcess(uchar *srcImgData, uchar *dstImgData, int height, int width, int numPixelsString, int xD, int yD) 
{
    Image src(srcImgData, width, height);
    Image dst(dstImgData, width, height);

    printf("Inicio -------\n\n\n\n ------------- \n"); 
    char vX[16], vY[16], vXi[16], vYi[16];
    initDirections(vX, vY, vXi, vYi);

    int nextIndex, blackIndex;
    char currX, currY;
    int lastLeftX, lastLeftY, lastRightX, lastRightY;
    int difX, difY;
    
    double tan1, tan2;
    for(int y(1); y < src.getHeight() - 1; ++y)
    {
        for(int x(1); x < src.getWidth() - 1; ++x)
        {      
            
            uchar selfValue = src.pix(x, y);
            if( x != xD || y != yD )
            {
                dst.pix(x, y) = 0xFF;   
                continue;
            }
            nextIndex = 0;
            currX = x;
            currY = y;
            for(int i(0); i < numPixelsString; ++i)
            {
                // clockwise
                blackIndex = findEdge(src, currX, currY, vX, vY, nextIndex, selfValue);
                printf("blackIndex: %d\n", blackIndex); 
                currX += vX[blackIndex];
                currY += vY[blackIndex];
                dst.pix(currX, currY) = 0;
                nextIndex = (blackIndex + 4) % 8;
            }
            lastLeftX = currX;
            lastLeftY = currY;

            printf("segunda rodada ---\n ------------- \n"); 

            nextIndex = 0;
            for(int i(0); i < numPixelsString; ++i)
            {
                // counter-clockwise
                blackIndex = findEdge(src, currX, currY, vXi, vYi, nextIndex, selfValue); 
                printf("blackIndex: %d\n", blackIndex); 
                currX += vXi[blackIndex];
                currY += vYi[blackIndex];
                dst.pix(currX, currY) = 0;
                nextIndex = (blackIndex + 4) % 8;
            }
            lastRightX = currX;
            lastRightY = currY;

            
            difX = abs(lastRightX - x);
            difY = abs(lastRightY - y);
            tan1 = 0.0;
            if(difY != 0)
                tan1 = difX * 1.0 / difY;


            
            difX = lastLeftX - x;
            difY = lastLeftY - y;
            tan2 = 0.0;
            if(difY != 0)
                tan2 = difX * 1.0 / difY;

            double difTans = fabs(tan1 - tan2);
            dst.pix(x, y) = (int) difTans;
	    }
    }
  
}

} // end extern "C"
