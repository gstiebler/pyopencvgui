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

static uchar vX[8], vY[8];

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

    vY[0] = -1;
    vY[1] = -1;
    vY[2] = 0;
    vY[3] = 1;
    vY[4] = 1;
    vY[5] = 1;
    vY[6] = 0;
    vY[7] = -1;
}


uchar findCWEdge(Image &src, int xIni, int yIni, uchar startIndex)
{
    uchar selfValue = src.pix(xIni, yIni);

    int index;
    // finds white pixel
    int whiteIndex;
    for(int i(0); i < 8; ++i)
    {
        index = (startIndex + i) % 8;
        uchar currValue = src.pix(xIni + vX[index], yIni + vY[index]);
        if(currValue > selfValue)
        {
            whiteIndex = index;
            break;
        }
    }

    // finds black pixel
    startIndex = whiteIndex + 1;
    int blackIndex;
    for(int i(0); i < 8; ++i)
    {
        index = (startIndex + i) % 8;
        uchar currValue = src.pix(xIni + vX[index], yIni + vY[index]);
        if(currValue <= selfValue)
            return index;
    }
}



void seismicProcess(uchar *srcImgData, uchar *dstImgData, int height, int width, int numPixelsString) 
{
    Image src(srcImgData, width, height);
    Image dst(dstImgData, width, height);

    initDirections();

    int blackIndex;
    for(int y(1); y < src.getHeight() - 1; ++y)
    {
        for(int x(1); x < src.getWidth() - 1; ++x)
        {      
            blackIndex = findCWEdge(src, x, y, 0); 
	    }
    }
  
}

} // end extern "C"
