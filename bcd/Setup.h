/*
 *  Setup.h
 *  bunny chicken dinosaur
 *
 *  Created by Steve Barnett on 13/02/2011.
 *  Copyright 2011 Stephen Barnett. All rights reserved.
 *
 */

#ifndef SETUP_DEF
#define SETUP_DEF

#include "MSImage.h"
#include "MSSprite.h"

#include <vector>

namespace Setup
{
	void LoadResources();
	void UnloadResources();

	enum ESprites
	{
		eSp_Kerb1 = 0,
		eSp_Kerb2,
		eSp_Kerb3,
		eSp_Low,
		eSp_Mid,
		eSp_High1,
		eSp_High2,
		eSp_High3,
		eSp_Large1,
		eSp_Large2,
		eSp_Large3,
		eSp_Small1,
		eSp_Small2,
		eSp_Small3,
		eSp_Title,
		eSp_Pause,
		eSp_Victory,
		eSp_Defeat,
		eSp_Count,
	};

	MSSprite::SheetID Sheet( ESprites id );
	MSSprite::SpriteID Sprite( ESprites id );
	const char* const DataPath();

	struct STweaks
	{
		unsigned int stricttwocolour;
		unsigned int bunnycolour;
		unsigned int bunnyreserved1;
		unsigned int bunnyreserved2;
		unsigned int chickencolour;
		unsigned int chickenreserved1;
		unsigned int chickenreserved2;
		unsigned int dinosaurcolour;
		unsigned int dinosaurreserved1;
		unsigned int dinosaurreserved2;
	};

	struct SStage
	{
		char texture[8];
		unsigned int bg;
		unsigned int fg;
		unsigned int sky;
	};

	STweaks GetTweaks();
	int GetNumStages();
	SStage GetStage( int i );

	struct SSpriteData
	{
		SSpriteData() {}
		SSpriteData( MSSprite::SheetID _sheet, MSSprite::SpriteID _sprite )
		: sheet( _sheet), sprite( _sprite ) {}

		MSSprite::SheetID sheet;
		MSSprite::SpriteID sprite;
	};

	struct SCharData
	{
		MSSprite::SheetID sheet;
		MSSprite::SpriteID run[3];
		MSSprite::SpriteID jump[3];
		MSSprite::SpriteID fall[3];
	};

	enum EChar
	{
		eCh_Bunny = 0,
		eCh_Chicken,
		eCh_Dinosaur,
	};

	SCharData GetChar( EChar id );
};

#endif

