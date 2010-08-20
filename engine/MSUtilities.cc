/*
 *  MSUtilities.cc
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "MSEngine.h"

MHashID djb2( const char* pszString )
{
	MHashID hash = 5381;
	int c;
	const char* str = pszString;

	while (c = *str++) hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

