 
#ifndef IMAGE_RGB_H
#define IMAGE_RGB_H

typedef unsigned char uchar;

struct Cor
{
	uchar r, g, b;

	Cor( uchar r_, uchar g_, uchar b_);
};

class ImageRGB
{
public:
    ImageRGB(uchar *data, int width, int height);
    int getWidth();
    int getHeight();

    void setRGB( int i, int j, uchar r, uchar g, uchar b );
	void setRGB( int i, int j, const Cor &cor );
    void setR( int i, int j, uchar r );
    void setG( int i, int j, uchar g );
    void setB( int i, int j, uchar b );
	void setLum( int i, int j, uchar lum );
	void setLum( uchar lum );
	void setRGB( const Cor &cor );

    uchar getR( int i, int j );
    uchar getG( int i, int j );
    uchar getB( int i, int j );
	uchar getLum( int i, int j );
    
protected:
    uchar *_data;
    int _width;
    int _height;

	int getOffset( int i, int j);
};

#endif