/*
 *  Setup.cc
 *  bunny chicken dinosaur
 *
 *  Created by Steve Barnett on 13/02/2011.
 *  Copyright 2011 Stephen Barnett. All rights reserved.
 *
 */

#include "Setup.h"

#include "MSFont.h"

#if defined( TESTBUILD )
#define TEXDIR "./"
#else
#if defined( WIN32 )
#define TEXDIR "./"
#elif defined( MACOSX )
#define TEXDIR "../Resources/"
#endif
#endif

namespace Setup
{
	MSImage* s_fontImage;
	MSImage* s_spritesImage;

	MSSprite::SheetID s_sprites;
};

void Setup::LoadResources()
{
	s_fontImage = new MSImage( TEXDIR "charset.tga" );
	MSFont::Initialise( "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890@-.:'c", s_fontImage, MSVec( 6, 8 ) );

	//s_spritesImage = new MSImage( TEXDIR "space.tga" );
	//s_sprites = MSSprite::AddSheet( s_spritesImage, MSVec( 16, 16 ) );
}

void Setup::UnloadResources()
{
	MSFont::Shutdown();
	MSSprite::Shutdown();

	delete s_fontImage;
	delete s_spritesImage;
}

MSSprite::SheetID Setup::Sprites()
{
	return s_sprites;
}

const char* const Setup::DataPath()
{
	return TEXDIR;
}

