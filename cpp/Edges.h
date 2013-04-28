#ifndef EDGES_H
#define EDGES_H

#include "base.h"

extern "C" {
	
__declspec(dllexport) void __stdcall hEdges(uchar *srcImgData, uchar *dstImgData, int height, int width, int thresh, int delta);

}

#endif
	