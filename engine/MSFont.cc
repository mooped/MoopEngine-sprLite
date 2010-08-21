/*
 *  MSFont.cc
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "MSFont.h"

#include <string.h>
#include <algorithm>

namespace MSFont
{
	// Data
	const char* s_pCharSet;
	MSVec s_charSize;
	MSImage* s_pImage;
	
	bool Valid() { return s_pCharSet != NULL && s_pImage != NULL; }
};

void MSFont::Initialise( const char* const pCharSet, MSImage* texture, MSVec& charSize )
{
	s_pCharSet = pCharSet;
	s_charSize = charSize;
	s_pImage = texture;
	
}

void MSFont::Shutdown()
{
	s_pCharSet = NULL;
	s_pImage = NULL;
}

void MSFont::RenderString( const char* const pszString, MSVec& pos, int layer, MSVec& size, const Colour rgba/* = 0xffffffff*/, const bool center/* = false*/ )
{

}

void MSFont::RenderCharacter( const u_char character, MSVec& pos, int layer, MSVec& size, const Colour rgba/* = 0xffffffff*/ )
{
	if ( !Valid() || character >= 128 ) return;

	// Find it in the character set and compute starting U coordinate
	int startU = -1;
	const int maxLen = std::min( 128, static_cast<int>( strlen( s_pCharSet ) ) );
	for ( int u = 0; u < maxLen; ++u )
	{
		if ( s_pCharSet[u] == character )
		{
			startU = u * s_charSize.x;
			break;
		}
	}
	if ( startU >= 0 )
	{
		// Compute the vertices
		const MSVec offsetX = MSVec( size.x >> 1, 0 );
		const MSVec offsetY = MSVec( 0, size.y >> 1 );
	
		MSRender::SetTexture( s_pImage );
		MSVec verts[4] =
		{
			pos - offsetX - offsetY,
			pos + offsetX - offsetY,
			pos + offsetX + offsetY,
			pos - offsetX + offsetY,
		};
		MSVec uvs[2] =
		{
			MSVec( startU, 0 ),
			MSVec( startU + s_charSize.x, s_charSize.y ),
		};
		MSRender::Quad(
			verts,
			layer,
			uvs,
			rgba
		);
	}
}

