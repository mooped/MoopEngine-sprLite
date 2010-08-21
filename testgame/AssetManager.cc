/*
 *  AssetManager.cc
 *  testgame
 *
 *  Created by Steve Barnett on 21/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "AssetManager.h"

#include "MSFont.h"

#define TEXDIR "../../"

namespace AM
{
	MSImage* m_fontImage;
	MSImage* m_shipsImage;

	MSSprite::SheetID m_ships;
};

void AM::LoadResources()
{
	m_fontImage = new MSImage( TEXDIR "charset.tga" );
	m_shipsImage = new MSImage( TEXDIR "sprites.tga" );
	MSFont::Initialise( "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!?.:'c", m_fontImage, MSVec( 6, 8 ) );
	m_ships = MSSprite::AddSheet( m_shipsImage, MSVec( 16, 8 ) );
}

void AM::UnloadResources()
{
	delete m_fontImage;
	m_fontImage = NULL;
	delete m_shipsImage;
	m_shipsImage = NULL;
}

MSSprite::SheetID AM::Ships()
{
	return m_ships;
}

