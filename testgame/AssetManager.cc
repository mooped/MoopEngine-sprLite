/*
 *  AssetManager.cc
 *  testgame
 *
 *  Created by Steve Barnett on 21/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "AssetManager.h"

#include "MSFont.h"

#define TEXDIR "../../"

namespace AM
{
	MSImage* m_fontImage;
	MSImage* m_shipsImage;

	Entity::SRenderAnimSprite m_player[Entity::eSS_Count];
	Entity::SRenderAnimSprite m_enemy[Entity::eEE_Count][Entity::eSS_Count];

	Entity::SRenderAnimSprite m_weapon[Entity::eWT_Count];

	MSSprite::SheetID m_ships;
};

void AM::LoadResources()
{
	m_fontImage = new MSImage( TEXDIR "charset.tga" );
	m_shipsImage = new MSImage( TEXDIR "sprites.tga" );

	MSFont::Initialise( "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!?.:'c", m_fontImage, MSVec( 6, 8 ) );

	m_ships = MSSprite::AddSheet( m_shipsImage, MSVec( 16, 8 ) );

	// Player render objects
	// Passive
	m_player[Entity::eSS_Passive].sprite.base.kind = Entity::eRC_Sprite;
	m_player[Entity::eSS_Passive].sprite.sheet = AM::Ships();
	m_player[Entity::eSS_Passive].sprite.sprite = 0;
	// Moving
	m_player[Entity::eSS_Moving].sprite.base.kind = Entity::eRC_AnimSprite;
	m_player[Entity::eSS_Moving].sprite.sheet = AM::Ships();
	m_player[Entity::eSS_Moving].sprite.sprite = 1;
	m_player[Entity::eSS_Moving].start = 1;
	m_player[Entity::eSS_Moving].end = 4;

	// Weapon render objects
	// Lazzor
	m_weapon[Entity::eWT_Lazzor].sprite.base.kind = Entity::eRC_Sprite;
	m_weapon[Entity::eWT_Lazzor].sprite.sheet = AM::Ships();
	m_weapon[Entity::eWT_Lazzor].sprite.sprite = 40;
	// Flamer
	m_weapon[Entity::eWT_Lazzor].sprite.base.kind = Entity::eRC_AnimSprite;
	m_weapon[Entity::eWT_Lazzor].sprite.sheet = AM::Ships();
	m_weapon[Entity::eWT_Lazzor].sprite.sprite = 48;
	m_weapon[Entity::eWT_Lazzor].start = 48;
	m_weapon[Entity::eWT_Lazzor].end = 52;
}

void AM::UnloadResources()
{
	delete m_fontImage;
	m_fontImage = NULL;
	delete m_shipsImage;
	m_shipsImage = NULL;
}

MSSprite::SheetID AM::Ships()
{
	return m_ships;
}

Entity::SRenderBase* AM::Player( Entity::EShipState state )
{
	return reinterpret_cast<Entity::SRenderBase*>( &m_player[state] );
}

Entity::SRenderBase* AM::Enemy( Entity::EEnemyElement element, Entity::EShipState state )
{
	return reinterpret_cast<Entity::SRenderBase*>( &m_enemy[element][state] );
}

Entity::SRenderBase* AM::Weapon( Entity::EWeaponType type )
{
	return reinterpret_cast<Entity::SRenderBase*>( &m_weapon[type] );
}

