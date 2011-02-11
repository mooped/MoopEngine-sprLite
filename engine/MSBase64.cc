/*
 *  MSBase64.cc
 *  minirts
 *
 *  Created by Steve Barnett on 05/07/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#include "MSBase64.h"

#include "MSEngine.h"

namespace MSBase64
{
	unsigned char indexTable[65] = \
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
		"abcdefghijklmnopqrstuvwxyz" \
		"0123456789+/";

	unsigned char invertTable[256];
	bool invertTableBuilt = false;;

	void BuildInvertTable( void );
};

// Bytes
// |     8     |     8     |     8     |
// |-----------------------------------|
// |   6   | 2 |  4  |  4  | 2 |   6   |
// Base64

unsigned int MSBase64::Encode( char* out, const char* __restrict const in, const unsigned int size, bool terminate /*= true*/ )
{
	unsigned char buf[3];
	unsigned int u;
	const char* const start = out;

	// initial aligned bytes

	const int remainder = size % 3;
	const unsigned int initial = size - remainder;
	for ( u = 0; u < initial; u += 3 )
	{
		for ( unsigned int v = 0; v < 3; ++v ) buf[v] = in[u + v];

		*(out++) = indexTable[(buf[0] & 0xfe) >> 2];
		*(out++) = indexTable[((buf[0] & 0x03) << 4) | ((buf[1] & 0xf0) >> 4)];
		*(out++) = indexTable[((buf[1] & 0x0f) << 2) | ((buf[2] & 0xc0) >> 6)];
		*(out++) = indexTable[buf[2] & 0x3f];
	}

	// remaining unaligned bytes

	for ( int v = 0; v < remainder; ++v ) buf[v] = in[u + v];

	if ( remainder == 2 )
	{
		*(out++) = indexTable[(buf[0] & 0xfe) >> 2];
		*(out++) = indexTable[((buf[0] & 0x03) << 4 ) | ((buf[1] & 0xf0) >> 4)];
		*(out++) = indexTable[((buf[1] & 0x0f) << 2 )];
		*(out++) = '=';
	}
	else if ( remainder == 1 )
	{
		*(out++) = indexTable[(buf[0] & 0xfe) >> 2];
		*(out++) = indexTable[(buf[0] & 0x03) << 4];
		*(out++) = '=';
		*(out++) = '=';
	}

	if ( terminate ) *(out++) = '\0';

	return out - start;
}

// Base64
// |   6   | 2 |  4  |  4  | 2 |   6   |
// |-----------------------------------|
// |     8     |     8     |     8     |
// Bytes

unsigned int MSBase64::Decode( char* out, const char* __restrict const in, const unsigned int size, bool terminate /*= true*/ )
{
	unsigned char buf[4];
	unsigned int u;
	const char* const start = out;

	if ( !invertTableBuilt )
	{
		invertTableBuilt = true;
		BuildInvertTable();
	}

	MASSERT( size % 4 == 0, "Expected padding on Base64 encoded string." );

	for ( u = 0; u < size; u += 4 )
	{
		for ( unsigned int v = 0; v < 4; ++v ) buf[v] = in[u + v];

		*(out++) = ( invertTable[buf[0]] << 2 ) | ( ( invertTable[buf[1]] & 0x30 ) >> 4 );
		if ( buf[1] == '=' || buf[2] == '=' )
		{
			break;
		}
		*(out++) = ( ( invertTable[buf[1]] & 0x0f ) << 4 ) | ( ( invertTable[buf[2]] & 0x3c ) >> 2 );
		if ( buf[2] == '=' || buf[3] == '=' )
		{
			break;
		}
		*(out++) = ( ( invertTable[buf[2]] & 0x03 ) << 6 ) | ( invertTable[buf[3]] & 0x3f );
	}

	if ( terminate ) *(out++) = '\0';

	return out - start;
}

unsigned int MSBase64::EncodedSize( const unsigned int size )
{
	return (size + 2 - ((size + 2) % 3)) * 4 / 3;
}

unsigned int MSBase64::DecodedSize( const unsigned int size )
{
	// size will include up to 3 bytes of padding
	return size * 3 / 4 + PADDINGP2( size * 3 / 4, 4 );
}

void MSBase64::BuildInvertTable( void )
{
	unsigned int u, v;
	for ( u = 0; u < 256; ++u )
	{
		invertTable[u] = '\0';
		for ( v = 0; v < 64; ++v )
		{
			if ( indexTable[v] == u )
			{
				invertTable[u] = v;
				break;
			}
		}
	}
}

// Simple test case
#if 0
namespace MSBase64
{
	#include <stdio.h>

	void Test( void )
	{
		fprintf( stderr, "Example:dGVzdGluZyB0ZXN0aW5nIHRlc3RpbmcuLi4i\n" );
		const char* const test = "testing testing testing...";
		unsigned int len = strlen( test );
		char buffer[255];
		char dbuffer[255];
		while ( len )
		{
			MSBase64::Encode( buffer, test, len );
			fprintf( stderr, "len: %d encoded: %d\nbase64: %s\n", len, MSBase64::EncodedSize( len ), buffer );
			MSBase64::Decode( dbuffer, buffer, MSBase64::EncodedSize( strlen( buffer ) ) );
			fprintf( stderr, "decoded: %d ascii: %s\n\n", MSBase64::DecodedSize( strlen( buffer ) ), dbuffer );
			--len;
		}
	}
};
#endif

