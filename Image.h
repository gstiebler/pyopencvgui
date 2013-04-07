 
#ifndef IMAGE_CPP
#define IMAGE_CPP

typedef unsigned char uchar;

class Image
{
public:
    Image(uchar *data, int width, int height);
    uchar& pix(int i, int j);
    int getWidth();
    int getHeight();
    
protected:
    uchar *_data;
    int _width;
    int _height;
};

#endif