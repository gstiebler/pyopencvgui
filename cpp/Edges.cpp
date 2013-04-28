#include "Edges.h"

#include "ImageRGB.h"
#include "Image8Bits.h"

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
