#ifndef IMAGE_8BITS_H
#define IMAGE_8BITS_H

typedef unsigned char uchar;

class Image8Bits
{
public:
    Image8Bits(uchar *data, int width, int height);
    uchar& pix(int i, int j);
    int getWidth();
    int getHeight();
    
protected:
    uchar *_data;
    int _width;
    int _height;
};

#endif