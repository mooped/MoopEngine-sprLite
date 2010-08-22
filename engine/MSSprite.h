/*
 *  MSSprite.h
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MSSPRITE_DEF
#define MSSPRITE_DEF

#include "MSEngine.h"

#include "MSImage.h"
#include "MSPoint.h"

namespace MSSprite
{
	typedef int SheetID;
	typedef int SpriteID;

	SheetID AddSheet( MSImage* image, const MSVec& spriteSize );
	void Shutdown();

	void RenderSprite( SheetID sheet, SpriteID sprite, const MSVec& pos, int layer, const MSVec& size, const Colour rgba = 0xffffffff );
	MSVec GetSpriteSize( SheetID sheet );
};

#endif

