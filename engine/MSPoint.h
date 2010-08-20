/*
 *  MSPoint.h
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MSPOINT_DEF
#define MSPOINT_DEF

#include "MSEngine.h"

template <class T>
class MSPoint
{
private:
	T x;
	T y;

public:
	MSPoint() : x( (T)0 ), y( (T)0 ) {};
	MSPoint( T x, T y ) : x( (T)0 ), y( (T)0 ) {};
	MSPoint( const MSPoint& other ) : x( (T)other.x ), y( (T)other.y ) {};

	T& x() { return x; }
	T& y() { return y; }

	MSPoint<T>& operator=( const MSPoint<T>& other )
	{
		if ( &other != this )
		{
			x = other.x;
			y = other.y;
		}
		return *this;
	}
};

MSPoint<T> operator+( const MSPoint<T>& a, const MSPoint<T>& b )
{
	return MSPoint( a.x + b.x, a.y + b.y );
}

MSPoint<T> operator-( const MSPoint<T>& a, const MSPoint<T>& b )
{
	return MSPoint( a.x - b.x, a.y - b.y );
}

MSPoint<T> operator*( const MSPoint<T>& a, const MSPoint<T>& b )
{
	return MSPoint( a.x * b.x, a.y * b.y );
}

MSPoint<T> operator/( const MSPoint<T>& a, const MSPoint<T>& b )
{
	return MSPoint( a.x / b.x, a.y / b.y );
}

typedef MSPoint<int> MSVec;

#endif

