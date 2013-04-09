#include <iostream>
#include <stdio.h>

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

static uchar vX[16], vY[16];

void initDirections()
{
    vX[0] = 0;
    vX[1] = 1;
    vX[2] = 1;
    vX[3] = 1;
    vX[4] = 0;
    vX[5] = -1;
    vX[6] = -1;
    vX[7] = -1;

    vX[8] = 0;
    vX[9] = 1;
    vX[10] = 1;
    vX[11] = 1;
    vX[12] = 0;
    vX[13] = -1;
    vX[14] = -1;
    vX[15] = -1;

    vY[0] = -1;
    vY[1] = -1;
    vY[2] = 0;
    vY[3] = 1;
    vY[4] = 1;
    vY[5] = 1;
    vY[6] = 0;
    vY[7] = -1;

    vY[8] = -1;
    vY[9] = -1;
    vY[10] = 0;
    vY[11] = 1;
    vY[12] = 1;
    vY[13] = 1;
    vY[14] = 0;
    vY[15] = -1;
}


uchar findCWEdge(Image &src, int xIni, int yIni, uchar startIndex)
{
    uchar selfValue = src.pix(xIni, yIni);

    // finds white pixel
    int whiteIndex;
    for(int i(startIndex); i < ( startIndex + 8 ); ++i)
    {
        uchar currValue = src.pix(xIni + vX[i], yIni + vY[i]);
        if(currValue > selfValue)
        {
            whiteIndex = index;
            break;
        }
    }

    // finds black pixel
    startIndex = (whiteIndex + 1) % 8;
    int blackIndex;
    for(int i(startIndex); i < ( startIndex + 8 ); ++i)
    {
        uchar currValue = src.pix(xIni + vX[i], yIni + vY[i]);
        if(currValue <= selfValue)
            return i % 8;
    }
}



void seismicProcess(uchar *srcImgData, uchar *dstImgData, int height, int width, int numPixelsString) 
{
    Image src(srcImgData, width, height);
    Image dst(dstImgData, width, height);

    initDirections();

    int nextIndex;
    for(int y(1); y < src.getHeight() - 1; ++y)
    {
        for(int x(1); x < src.getWidth() - 1; ++x)
        {      
            nextIndex = 0;
            for(int i(0)
            blackIndex = findCWEdge(src, x, y, 0); 
	    }
    }
  
}

} // end extern "C"
