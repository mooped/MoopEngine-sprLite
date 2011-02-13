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

#include <vector>

namespace Setup
{
	MSImage* s_fontImage;
	std::vector<MSImage*> s_spriteImages;

	SSpriteData s_sprites[eSp_Count];
};

void Setup::LoadResources()
{
	s_fontImage = new MSImage( TEXDIR "charset.tga" );
	MSFont::Initialise( "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890@-.:'c", s_fontImage, MSVec( 6, 8 ) );

	MSImage* image = NULL;
	MSSprite::SpriteID sprite = 0;

	image = new MSImage( TEXDIR "terrain.tga" );
	s_spriteImages.push_back( image );
	sprite = MSSprite::AddSheet( image, MSVec( 32, 96 ) );
	s_sprites[eSp_Kerb1] = SSpriteData( sprite, 0 );
	s_sprites[eSp_Kerb2] = SSpriteData( sprite, 1 );
	s_sprites[eSp_Kerb3] = SSpriteData( sprite, 2 );
	s_sprites[eSp_Low] = SSpriteData( sprite, 3 );
	s_sprites[eSp_Mid] = SSpriteData( sprite, 4 );
	s_sprites[eSp_High1] = SSpriteData( sprite, 5 );
	s_sprites[eSp_High2] = SSpriteData( sprite, 6 );
	s_sprites[eSp_High3] = SSpriteData( sprite, 7 );
	s_sprites[eSp_Large1] = SSpriteData( sprite, 9 );


	s_sprites[eSp_Large2] = SSpriteData( sprite, 10 );
	s_sprites[eSp_Large3] = SSpriteData( sprite, 11 );
	s_sprites[eSp_Small1] = SSpriteData( sprite, 12 );
	s_sprites[eSp_Small2] = SSpriteData( sprite, 13 );
	s_sprites[eSp_Small3] = SSpriteData( sprite, 14 );

	image = new MSImage( TEXDIR "title.tga" );
	s_spriteImages.push_back( image );
	sprite = MSSprite::AddSheet( image, MSVec( 256, 256 ) );
	s_sprites[eSp_Title] = SSpriteData( sprite, 0 );

	image = new MSImage( TEXDIR "pause.tga" );
	s_spriteImages.push_back( image );
	sprite = MSSprite::AddSheet( image, MSVec( 256, 256 ) );
	s_sprites[eSp_Pause] = SSpriteData( sprite, 0 );

	image = new MSImage( TEXDIR "victory.tga" );
	s_spriteImages.push_back( image );
	sprite = MSSprite::AddSheet( image, MSVec( 256, 256 ) );
	s_sprites[eSp_Victory] = SSpriteData( sprite, 0 );

	image = new MSImage( TEXDIR "defeat.tga" );
	s_spriteImages.push_back( image );
	sprite = MSSprite::AddSheet( image, MSVec( 256, 256 ) );
	s_sprites[eSp_Defeat] = SSpriteData( sprite, 0 );
}

void Setup::UnloadResources()
{
	MSFont::Shutdown();
	MSSprite::Shutdown();

	delete s_fontImage;

	for ( std::vector<MSImage*>::iterator it = s_spriteImages.begin(); it < s_spriteImages.end(); ++it )
	{
		delete *it;
	}
}

MSSprite::SheetID Setup::Sheet( ESprites id )
{
	fprintf( stderr, "Id: %i Sheet: %i\n", id, s_sprites[id].sheet );
	return s_sprites[id].sheet;
}

MSSprite::SpriteID Setup::Sprite( ESprites id )
{
	fprintf( stderr, "Id: %i Sprite: %i\n", id, s_sprites[id].sprite );
	return s_sprites[id].sprite;
}

const char* const Setup::DataPath()
{
	return TEXDIR;
}

