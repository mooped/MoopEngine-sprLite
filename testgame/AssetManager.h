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
#include "Entity.h"

namespace AM
{
	void LoadResources();
	void UnloadResources();

	MSSprite::SheetID Sprites();
	MSSprite::SheetID BigSprites();

	Entity::SRenderBase* Player( Entity::EShipState state );
	Entity::SRenderBase* Enemy( Entity::EEnemyElement element, Entity::EShipState state );

	Entity::SRenderBase* Weapon( Entity::EWeaponType type );

	Entity::SRenderBase* Shield();

	MSSprite::SpriteID TestTube();
};

#endif

