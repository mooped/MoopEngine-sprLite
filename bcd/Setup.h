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

	MSSprite::SheetID Sprites();
	const char* const DataPath();
};

#endif

