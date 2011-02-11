/*
 *  MSBase64.h
 *  minirts
 *
 *  Created by Steve Barnett on 05/07/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#ifndef MBASE64_DEF
#define MBASE64_DEF

namespace MSBase64
{
	unsigned int Encode( char* out, const char* const __restrict in, const unsigned int size, bool terminate = true );
	unsigned int Decode( char* out, const char* const __restrict in, const unsigned int size, bool terminate = true );
	unsigned int EncodedSize( const unsigned int size );
	unsigned int DecodedSize( const unsigned int size );
};

#endif

