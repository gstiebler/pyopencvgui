#ifndef IMAGE_8BITS_H
#define IMAGE_8BITS_H

typedef unsigned char uchar;

class Image8Bits
{
public:
    Image8Bits(uchar *data, int width, int height);
    uchar getLum(int i, int j) const;
    int getWidth() const;
    int getHeight() const;
	void setLum( uchar lum );
	void setLum(int i, int j, uchar lum);
    
protected:
    uchar *_data;
    int _width;
    int _height;
};

#endif