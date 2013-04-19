#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#include "Image.h"

extern "C" {
 
typedef unsigned char uchar;
typedef long int lint;
  
__declspec(dllexport) void __stdcall myfunc(uchar *srcImgData, uchar *dstImgData, int height, int width, int thresh)
{
    printf("Largura %d, Altura %d\n", width, height);
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


uchar findEdge(Image &src, int xIni, int yIni, char vX[16], char vY[16], uchar startIndex, uchar selfValue)
{
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

        if(currValue > selfValue)
        {
            whiteIndex = i % 8;
            break;
        }
    }    

    // finds black pixel
    startIndex = (whiteIndex + 1) % 8;
    for(int i(startIndex); i < ( startIndex + 8 ); ++i)
    {
        xIndex = xIni + vX[i];
        yIndex = yIni + vY[i];
        if( xIndex == 0 || yIndex == 0 || xIndex == src.getWidth() || yIndex == src.getHeight() )
            break;

        uchar currValue = src.pix(xIndex, yIndex);
        if(currValue <= selfValue)
            return i % 8;
    }

    return -1;
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
 

__declspec(dllexport) void __stdcall seismicProcess(uchar *srcImgData, uchar *dstImgData, int height, int width, int numPixelsString) 
{
    int xD = 180, yD = 156;
    printf("h %d, w %d, n %d, x %d, y %d\n", height, width, numPixelsString, xD, yD);
    Image src(srcImgData, width, height);
    Image dst(dstImgData, width, height);

    char vX[16], vY[16], vXi[16], vYi[16];
    initDirections(vX, vY, vXi, vYi);

    printf("Inicializou direcoes\n");

    int nextIndex, blackIndex;
    int currX, currY;
    int lastLeftX, lastLeftY, lastRightX, lastRightY;
    int difXright, difYright, difXleft, difYleft;
    
    double tan1, tan2, ang1, ang2;
    double conv = 180.0 / 3.1416;
    
    printf("Dims %d, %d\n", src.getHeight(), src.getWidth());

    for(int y(1); y < src.getHeight() - 1; ++y)
    {
        // printf("y%d ", y);
        for(int x(1); x < src.getWidth() - 1; ++x)
        {      
            
            uchar selfValue = src.pix(x, y);
            nextIndex = 0;
            currX = x;
            currY = y;
            for(int i(0); i < numPixelsString; ++i)
            {
                // clockwise
                blackIndex = findEdge(src, currX, currY, vX, vY, nextIndex, selfValue); 
                currX += vX[blackIndex];
                currY += vY[blackIndex];
                nextIndex = (blackIndex + 4) % 8;
                if( x == xD && y == yD )
                    printf("left i: %d, blackIndex: %d, currX: %d, currY:%d\n", i, blackIndex, currX, currY);
            }
            lastLeftX = currX;
            lastLeftY = currY;

            nextIndex = 0;
            currX = x;
            currY = y;
            for(int i(0); i < numPixelsString; ++i)
            {
                // counter-clockwise
                blackIndex = findEdge(src, currX, currY, vXi, vYi, nextIndex, selfValue); 
                currX += vXi[blackIndex];
                currY += vYi[blackIndex];
                nextIndex = (blackIndex + 4) % 8;
                if( x == xD && y == yD )
                    printf("right i: %d, blackIndex: %d, currX: %d, currY:%d \n", i, blackIndex, currX, currY);
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
                difAng = 0.0;

            dst.pix(x, y) = (int) difAng;
            //dst.pix(x, y) = x + y;

            if( x == xD && y == yD )
            {
                printf(" --------------- \n");
                printf("difXright: %d, difYright: %d\n", difXright, difYright);
                printf("difXleft: %d, difYleft: %d\n", difXleft, difYleft);
                printf("tan1: %f, tan2: %f, ang1: %f, ang2: %f, difAng: %f\n", tan1, tan2, ang1, ang2, difAng);
                printf("\n");
            }
	    }
    }
  
}

} // end extern "C"

