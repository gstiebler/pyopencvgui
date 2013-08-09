#ifndef BASE_H
#define BASE_H
 
typedef unsigned char uchar;
typedef long int lint;

struct Point
{
	int _x, _y;
	Point( int x, int y ) : _x(x), _y(y) {}
	void set( int x, int y )
	{
		_x = x;
		_y = y;
	}
};


#endif
