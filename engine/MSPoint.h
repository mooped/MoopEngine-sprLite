/*
 *  MSPoint.h
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#ifndef MSPOINT_DEF
#define MSPOINT_DEF

#include "MSEngine.h"

#include <algorithm>

template <typename T>
class MSPoint
{
public:
	T x;
	T y;

	MSPoint() : x( (T)0 ), y( (T)0 ) {};
	MSPoint( T x, T y ) : x( (T)x ), y( (T)y ) {};
	MSPoint( const MSPoint& other ) : x( (T)other.x ), y( (T)other.y ) {};

	MSPoint& operator=( const MSPoint& other )
	{
		if ( &other != this )
		{
			x = other.x;
			y = other.y;
		}
		return *this;
	}
};

typedef MSPoint<int> MSVec;

template <typename T>
MSPoint<T> operator+( const MSPoint<T>& a, const MSPoint<T>& b )
{
	return MSPoint<T>( a.x + b.x, a.y + b.y );
}

template <typename T>
MSPoint<T> operator-( const MSPoint<T>& a, const MSPoint<T>& b )
{
	return MSPoint<T>( a.x - b.x, a.y - b.y );
}

template <typename T>
MSPoint<T> operator*( const MSPoint<T>& a, const MSPoint<T>& b )
{
	return MSPoint<T>( a.x * b.x, a.y * b.y );
}

template <typename T>
MSPoint<T> operator/( const MSPoint<T>& a, const MSPoint<T>& b )
{
	return MSPoint<T>( a.x / b.x, a.y / b.y );
}

template <typename T>
bool operator==( const MSPoint<T>& a, const MSPoint<T>& b )
{
	return ( a.x == b.x ) && ( a.y == b.y );
}

template <typename T>
bool operator!=( const MSPoint<T>& a, const MSPoint<T>& b )
{
	return !( a == b );
}

template <typename T>
MSPoint<T> min( const MSPoint<T>& a, const MSPoint<T>& b )
{
	MSPoint<T> res(
		std::min( a.x, b.x ),
		std::min( a.y, b.y )
	);
	return res;
}

template <typename T>
MSPoint<T> max( const MSPoint<T>& a, const MSPoint<T>& b )
{
	MSPoint<T> res(
		std::max( a.x, b.x ),
		std::max( a.y, b.y )
	);
	return res;
}

#endif

