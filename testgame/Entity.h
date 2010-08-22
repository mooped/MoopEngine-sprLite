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
		eEC_Enemy,
		eEC_Bullet,
	};

	enum ERenderClass
	{
		eRC_Base = 0,
		eRC_Sprite,
		eRC_AnimSprite,
	};

	enum EShipState
	{
		eSS_Passive = 0,
		eSS_Moving,
		eSS_Count,
	};

	enum EEnemyElement
	{
		eEE_Fire = 0,
		eEE_Water,
		eEE_Earth,
		//eEE_Air,
		eEE_Count,
	};

	enum EWeaponType
	{
		eWT_Lazzor = 0,
		eWT_Flamer,
		eWT_Count,
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
		int frequency;
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

	struct SEnemy
	{
		SBase base;
		EEnemyElement element;
		MSVec vel;
		EShipState state;
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
	EntID SpawnEnemy( const MSVec& pos, const MSVec& vel, EEnemyElement element );
	EntID SpawnBullet( const MSVec& pos, const MSVec& vel );

	void SpawnWaves();
};

#endif

