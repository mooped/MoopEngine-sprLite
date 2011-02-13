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
#include <stdio.h>

namespace Setup
{
	MSImage* s_fontImage;
	std::vector<MSImage*> s_spriteImages;

	SSpriteData s_sprites[eSp_Count];

	STweaks s_tweaks;
	std::vector<SStage> s_stages;

	SCharData s_chars[3];
};

void Setup::LoadResources()
{
	s_fontImage = new MSImage( TEXDIR "charset.tga" );
	MSFont::Initialise( "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890@-.:'c", s_fontImage, MSVec( 6, 8 ) );

	MSImage* image = NULL;
	MSSprite::SpriteID sprite = 0;

	// Foreground and background
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
	s_sprites[eSp_Large1] = SSpriteData( sprite, 8 );
	s_sprites[eSp_Large2] = SSpriteData( sprite, 9 );
	s_sprites[eSp_Large3] = SSpriteData( sprite, 10 );
	s_sprites[eSp_Small1] = SSpriteData( sprite, 11 );
	s_sprites[eSp_Small2] = SSpriteData( sprite, 12 );
	s_sprites[eSp_Small3] = SSpriteData( sprite, 13 );

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

	// Characters
	image = new MSImage( TEXDIR "bunny.tga" );
	s_spriteImages.push_back( image );
	sprite = MSSprite::AddSheet( image, MSVec( 40, 40 ) );
	s_chars[0].sheet = sprite;
	s_chars[0].run[0] = 0;
	s_chars[0].run[1] = 1;
	s_chars[0].run[2] = 2;
	s_chars[0].jump[0] = 3;
	s_chars[0].jump[1] = 4;
	s_chars[0].jump[2] = 5;
	s_chars[0].fall[0] = 6;
	s_chars[0].fall[1] = 7;
	s_chars[0].fall[2] = 8;

	image = new MSImage( TEXDIR "chicken.tga" );
	s_spriteImages.push_back( image );
	sprite = MSSprite::AddSheet( image, MSVec( 32, 40 ) );
	s_chars[1].sheet = sprite;
	s_chars[1].run[0] = 0;
	s_chars[1].run[1] = 1;
	s_chars[1].run[2] = 2;
	s_chars[1].jump[0] = 4;
	s_chars[1].jump[1] = 5;
	s_chars[1].jump[2] = 6;
	s_chars[1].fall[0] = 8;
	s_chars[1].fall[1] = 9;
	s_chars[1].fall[2] = 10;

	image = new MSImage( TEXDIR "dinosaur.tga" );
	s_spriteImages.push_back( image );
	sprite = MSSprite::AddSheet( image, MSVec( 72, 48 ) );
	s_chars[2].sheet = sprite;
	s_chars[2].run[0] = 0;
	s_chars[2].run[1] = 1;
	s_chars[2].run[2] = 2;
	s_chars[2].jump[0] = 3;
	s_chars[2].jump[1] = 4;
	s_chars[2].jump[2] = 5;
	s_chars[2].fall[0] = 6;
	s_chars[2].fall[1] = 7;
	s_chars[2].fall[2] = 8;

	// Load tweaks
	{
		FILE* f = fopen( TEXDIR "tweaks.dat", "rb" );
		fread( &s_tweaks, sizeof( s_tweaks), 1, f );
		fclose( f );
		fprintf( stderr, "Tweaks:\n" );
		fprintf( stderr, "\tstricttwocolour: 0x%x\n", s_tweaks.stricttwocolour );
		fprintf( stderr, "\tbunnycolour: 0x%x\n", s_tweaks.bunnycolour );
		fprintf( stderr, "\tchickencolour: 0x%x\n", s_tweaks.chickencolour );
		fprintf( stderr, "\tdinosaurcolour: 0x%x\n", s_tweaks.dinosaurcolour );
		fprintf( stderr, "\n" );
	}

	// Load stage data
	{
		FILE* f = fopen( TEXDIR "stages.dat", "rb" );
		int nstages = 0;
		fread( &nstages, sizeof( nstages ), 1, f );
		s_stages.reserve( nstages );
		fprintf( stderr, "Stages: %i\n", nstages );
		for ( int i = 0; i < nstages; ++i )
		{
			fprintf( stderr, "Stage %i:\n", i );
			fread( &s_stages[i], sizeof( SStage ), 1, f );
			fprintf( stderr, "\tbg: 0x%x\n", s_stages[i].bg );
			fprintf( stderr, "\tfg: 0x%x\n", s_stages[i].fg );
			fprintf( stderr, "\tsky: 0x%x\n", s_stages[i].sky );
		}
		fclose( f );
	}
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

Setup::STweaks Setup::GetTweaks()
{
	return s_tweaks;
}

int	Setup::GetNumStages()
{
	return s_stages.size();
}

Setup::SStage Setup::GetStage( int i )
{
	return s_stages[i];
}

Setup::SCharData Setup::GetChar( EChar id )
{
	return s_chars[id];
}

