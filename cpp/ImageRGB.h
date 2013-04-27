 
#ifndef IMAGE_RGB_H
#define IMAGE_RGB_H

typedef unsigned char uchar;

class ImageRGB
{
public:
    ImageRGB(uchar *data, int width, int height);
    void setRGB( int i, int j, uchar r, uchar g, uchar b );
    int getWidth();
    int getHeight();
    
protected:
    uchar *_data;
    int _width;
    int _height;
};

#endif