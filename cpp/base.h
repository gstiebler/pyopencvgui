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

	bool operator<(Point other) const
    {
		if(_x < other._x)
			return true;
		else if (_x > other._x)
			return false;

        return _y < other._y;
    }

	bool operator==(Point other) const
	{
		return _x == other._x && _y == other._y;
	}
};


#endif
