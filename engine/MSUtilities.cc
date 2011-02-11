/*
 *  MSUtilities.cc
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#include "MSEngine.h"

// Hashing
MHashID djb2( const char* pszString )
{
	MHashID hash = 5381;
	int c;
	const char* str = pszString;

	while (c = *str++) hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

// Endian swapping
/*
u64 swap_endian64( u64 n )
{
#ifdef BIGENDIAN
	return
		((n & 0x00000000000000ffLL) << 56) |
		((n & 0x000000000000ff00LL) << 40) |
		((n & 0x0000000000ff0000LL) << 32) |
		((n & 0x00000000ff000000LL) << 8) |
		((n & 0x000000ff00000000LL) >> 8) |
		((n & 0x0000ff0000000000LL) >> 32) |
		((n & 0x00ff000000000000LL) >> 40) |
		((n & 0xff00000000000000LL) >> 56);
#else 
	return n;
#endif
}
*/

u32 swap_endian32( u32 n )
{
#ifdef BIGENDIAN
	return
		((n & 0x000000ff) << 24) |
		((n & 0x0000ff00) << 8) |
		((n & 0x00ff0000) >> 8) |
		((n & 0xff000000) >> 24);
#else
	return n;
#endif
}

u16 swap_endian16( u16 n )
{
#ifdef BIGENDIAN
	return
		((n & 0x00ff) << 8) |
		((n & 0xff00) >> 8);
#else
	return n;
#endif
}

