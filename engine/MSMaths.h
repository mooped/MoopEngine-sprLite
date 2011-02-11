/*
 *  MSMaths.h
 *  sokoban
 *
 *  Created by Steve Barnett on 10/10/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#ifndef MSMATHS_DEF
#define MSMATHS_DEF

#include <algorithm>

namespace MSMaths
{
	const float pi = 3.14159265;

	template <typename T>
	void clamp_inplace( T& a, const T& min, const T& max )
	{
		a = std::max( std::min( a, max ), min );
	}

	template <typename T>
	T clamp( T a, const T& min, const T& max )
	{
		clamp_inplace( a, min, max );
		return a;
	}

	template <typename T>
	T radians( T degrees )
	{
		return degrees * ( pi / 180.f );
	}
};

#endif

