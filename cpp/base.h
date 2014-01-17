#ifndef BASE_H
#define BASE_H
 
#if defined(_MSC_VER)
    //  Microsoft 
    #define EXPORT __declspec(dllexport)
	#define CALL_CONV __stdcall
#else
    //  GCC
	#define CALL_CONV 
    #define EXPORT __attribute__((visibility("default")))
#endif

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
