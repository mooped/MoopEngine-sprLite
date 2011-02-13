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

	struct SSpriteData
	{
		SSpriteData() {}
		SSpriteData( MSSprite::SheetID _sheet, MSSprite::SpriteID _sprite )
		: sheet( _sheet), sprite( _sprite ) {}

		MSSprite::SheetID sheet;
		MSSprite::SpriteID sprite;
	};
};

#endif

