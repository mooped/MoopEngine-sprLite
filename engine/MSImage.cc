/*
 *  MSImage.cc
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#include "MSImage.h"

#include <stdio.h>
#include <string.h>

#pragma pack(push,1)
struct TGAHeader
{
    char  identsize;          // size of ID field that follows 18 byte header (0 usually)
    char  colourmaptype;      // type of colour map 0=none, 1=has palette
    char  imagetype;          // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

    short colourmapstart;     // first colour map entry in palette
    short colourmaplength;    // number of colours in palette
    char  colourmapbits;      // number of bits per palette entry 15,16,24,32

    short xstart;             // image x origin
    short ystart;             // image y origin
    short width;              // image width in pixels
    short height;             // image height in pixels
    char  bits;               // image bits per pixel 8,16,24,32
    char  descriptor;         // image descriptor bits (vh flip bits)
};
#pragma pack(pop)

void MSImage::LoadTGA( const char* const filename )
{
	FILE* f = fopen( filename, "rb" );
	if ( f )
	{
		TGAHeader header;
		int read = 0;

		// Read in the header
		read = fread( &header, sizeof( TGAHeader ), 1, f );
		MASSERT( read, "[LoadTGA] Unexpected EOF." );

#ifdef BIGENDIAN
		header.colourmapstart = swap_endian16( header.colourmapstart );
		header.colourmaplength = swap_endian16( header.colourmaplength );
		header.xstart = swap_endian16( header.xstart );
		header.ystart = swap_endian16( header.ystart );
		header.width = swap_endian16( header.width );
		header.height = swap_endian16( header.height );
#endif

		if ( header.bits == 32 )
		{
			// Read in the rest of the data
			u_char* data = new u_char[header.width * header.height * 4];
			read = fread( data, sizeof( u_char ), header.width * header.height * 4, f );
			MASSERT( read, "[LoadTGA] Unexpected EOF." );

			// Colour keying - BGRA
			for ( int u = 0; u < header.width * header.height * 4; u += 4 )
			{
				if ( data[u + 0] == 0xff && data[u + 1] == 0x00 && data[u + 2] == 0xff )
				{
					data[u + 0] = 0x00;	// Blue
					data[u + 1] = 0x00;	// Green
					data[u + 2] = 0x00;	// Red
					data[u + 3] = 0x00;	// Alpha
				}
			}

			m_width = header.width;
			m_height = header.height;
			m_data = data;
		}
		else
		{
			printf( "[LoadTGA] Unsupported colour depth" );
			MASSERT( 0, "[LoadTGA] Unsupported colour depth" );
		}
	}
}

MSImage::MSImage( const char* const filename )
: m_hash( MHash( filename ) )
, m_ref( ~0 )
, m_width( 0 )
, m_height( 0 )
, m_data( NULL )
{
	LoadTGA( filename );
}

MSImage::MSImage( const MHashID hash, const int width, const int height )
: m_hash( hash )
, m_ref( ~0 )
, m_width( width )
, m_height( height )
, m_data( new u_char[width * height * 4] )
{
	memset( m_data, 0x00, width * height * 4 );
}

MSImage::~MSImage()
{
	delete[] m_data;
}

