/*
 *  Entity.h
 *  testgame
 *
 *  Created by Steve Barnett on 21/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ENTITY_DEF
#define ENTITY_DEF

#include "MSEngine.h"

#include "MSPoint.h"
#include "MSSprite.h"

namespace Entity
{
	typedef unsigned int EntID;

	// Magic numbers
	enum EClass
	{
		eEC_Base = 0,
		eEC_Player,
		eEC_Bullet,
	};

	enum ERenderClass
	{
		eRC_Base = 0,
		eRC_Sprite,
		eRC_AnimSprite,
	};

	// Entity data structures
	struct SRenderBase
	{
		ERenderClass kind;
	};

	struct SRenderSprite
	{
		SRenderBase base;
		MSSprite::SheetID sheet;
		MSSprite::SpriteID sprite;
	};

	struct SRenderAnimSprite
	{
		SRenderSprite sprite;
		MSSprite::SpriteID start;
		MSSprite::SpriteID end;
	};

	struct SBase
	{
		EntID id;
		EClass kind;
		MSVec pos;
		int layer;
		MSVec size;
		SRenderBase* render;
	};

	struct SPlayer
	{
		SBase base;
		int shotTimeout;
	};

	struct SBullet
	{
		SBase base;
		MSVec vel;
	};

	EntID Add( SBase* ent );
	void Delete( EntID id, bool immediate = true );

	void Update();
	void Render();

	// Spawn helpers
	EntID SpawnPlayer( const MSVec& pos );
	EntID SpawnBullet( const MSVec& pos, const MSVec& vel );
};

#endif

