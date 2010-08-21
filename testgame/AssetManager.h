/*
 *  AssetManager.h
 *  testgame
 *
 *  Created by Steve Barnett on 21/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ASSETMANAGER_DEF
#define ASSETMANAGER_DEF

#include "MSSprite.h"

namespace AM
{
	void LoadResources();
	void UnloadResources();

	MSSprite::SheetID Ships();
};

#endif

