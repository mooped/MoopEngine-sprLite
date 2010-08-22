/*
 *  MSSprite.cc
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "MSSprite.h"

#include "MSRender.h"

#include <vector>
#include <math.h>

namespace MSSprite
{
	struct SSheet
	{
		MSImage* m_image;
		MSVec m_size;
		int m_perLine;
		int m_lines;
	};

	typedef std::vector<SSheet> SheetList;
	SheetList s_sheets;
};

MSSprite::SheetID MSSprite::AddSheet( MSImage* image, const MSVec& spriteSize )
{
	SSheet sheet;
	sheet.m_image = image;
	sheet.m_size = spriteSize;
	sheet.m_perLine = image->GetWidth() / spriteSize.x;
	sheet.m_lines = image->GetHeight() / spriteSize.y;
	const int id = s_sheets.size();
	s_sheets.push_back( sheet );
	return id;
}

void MSSprite::Shutdown()
{
	s_sheets.clear();
}

void MSSprite::RenderSprite( SheetID sheet, SpriteID sprite, const MSVec& pos, int layer, const MSVec& size, const Colour rgba/* = 0xffffffff*/ )
{
	if ( sheet < static_cast<int>( s_sheets.size() ) )
	{
		// Get the sprite sheet
		const SSheet sprites = s_sheets[sheet];
		if ( sprite < ( sprites.m_perLine * sprites.m_lines ) )
		{
			// Generate texture coordinates for the sprite
			int startu = ( sprite % sprites.m_perLine ) * sprites.m_size.x;
			int startv = ( sprite / sprites.m_perLine ) * sprites.m_size.y;
	
			// Draw the sprite
			MSRender::SetTexture( sprites.m_image );
			const MSVec offsetX = MSVec( size.x >> 1, 0 );
			const MSVec offsetY = MSVec( 0, size.y >> 1 );
			MSVec verts[4] =
			{
				pos - offsetX - offsetY,
				pos + offsetX - offsetY,
				pos + offsetX + offsetY,
				pos - offsetX + offsetY,
			};
			MSVec uvs[2] =
			{
				MSVec( startu, startv ),
				MSVec( startu + sprites.m_size.x, startv + sprites.m_size.y ),
			};
			MSRender::Quad(
				verts,
				layer,
				uvs,
				rgba
			);
		}
	}
}

MSVec MSSprite::GetSpriteSize( SheetID sheet )
{
	if ( sheet < static_cast<int>( s_sheets.size() ) )
	{
		const SSheet sprites = s_sheets[sheet];
		return sprites.m_size;
	}
	else 
	{
		return MSVec( 0, 0 );
	}
}

