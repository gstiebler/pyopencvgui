#include <iostream>
#include <stdio.h>

#include "Image.h"

extern "C" {
 
typedef unsigned char uchar;
typedef long int lint;
  
int myfunc(uchar *srcImgData, uchar *dstImgData, int height, int width, int thresh)
{
    printf("entrou na funcão\n");
    printf("tamanho %d, %d, thresh %d\n", width, height, thresh);
    
    Image src(srcImgData, width, height);
    Image dst(dstImgData, width, height);
    
    printf("tamanho2 %d, %d, thresh %d\n", src.getWidth(), src.getHeight(), thresh);

    uchar uThresh = (uchar) thresh;
    for(int i(0); i < src.getWidth(); ++i)
    {
        for(int j(0); j < src.getHeight(); ++j)
        {
            if(src.pix(i, j) < thresh)
                dst.pix(i, j) = 0;
            else
                dst.pix(i, j) = 255;
        }
    }
  
    return 0;  
}
} // end extern "C"
