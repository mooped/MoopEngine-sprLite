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

#define TESTBUILD

#if defined( TESTBUILD )
#define TEXDIR "../../"
#else
#if defined( WIN32 )
#define TEXDIR "./"
#elif defined( MACOSX )
#define TEXDIR "../Resources/"
#endif
#endif

namespace AM
{
	MSImage* m_fontImage;
	MSImage* m_spritesImage;
	MSImage* m_bigspritesImage;

	Entity::SRenderAnimSprite m_player[Entity::eSS_Count];
	Entity::SRenderAnimSprite m_enemy[Entity::eEE_Count][Entity::eSS_Count];

	Entity::SRenderAnimSprite m_weapon[Entity::eWT_Count];

	Entity::SRenderAnimSprite m_shield;

	MSSprite::SheetID m_sprites;
	MSSprite::SheetID m_bigsprites;
};

void AM::LoadResources()
{
	m_fontImage = new MSImage( TEXDIR "charset.tga" );
	m_spritesImage = new MSImage( TEXDIR "sprites.tga" );
	m_bigspritesImage = new MSImage( TEXDIR "bigsprites.tga" );

	MSFont::Initialise( "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!?.:'c", m_fontImage, MSVec( 6, 8 ) );

	m_sprites = MSSprite::AddSheet( m_spritesImage, MSVec( 16, 8 ) );
	m_bigsprites = MSSprite::AddSheet( m_bigspritesImage, MSVec( 16, 16 ) );

	// Player render objects
	// Passive
	m_player[Entity::eSS_Passive].sprite.base.kind = Entity::eRC_Sprite;
	m_player[Entity::eSS_Passive].sprite.sheet = AM::Sprites();
	m_player[Entity::eSS_Passive].sprite.sprite = 0;
	// Moving
	m_player[Entity::eSS_Moving].sprite.base.kind = Entity::eRC_AnimSprite;
	m_player[Entity::eSS_Moving].sprite.sheet = AM::Sprites();
	m_player[Entity::eSS_Moving].sprite.sprite = 1;
	m_player[Entity::eSS_Moving].frequency = 4;
	m_player[Entity::eSS_Moving].start = 1;
	m_player[Entity::eSS_Moving].end = 4;

	// Enemy render objects
	// Fire
	// 	Passive
	m_enemy[Entity::eEE_Fire][Entity::eSS_Passive].sprite.base.kind = Entity::eRC_Sprite;
	m_enemy[Entity::eEE_Fire][Entity::eSS_Passive].sprite.sheet = AM::Sprites();
	m_enemy[Entity::eEE_Fire][Entity::eSS_Passive].sprite.sprite = 8;
	// 	Moving
	m_enemy[Entity::eEE_Fire][Entity::eSS_Moving].sprite.base.kind = Entity::eRC_AnimSprite;
	m_enemy[Entity::eEE_Fire][Entity::eSS_Moving].sprite.sheet = AM::Sprites();
	m_enemy[Entity::eEE_Fire][Entity::eSS_Moving].sprite.sprite = 9;
	m_enemy[Entity::eEE_Fire][Entity::eSS_Moving].frequency = 5;
	m_enemy[Entity::eEE_Fire][Entity::eSS_Moving].start = 9;
	m_enemy[Entity::eEE_Fire][Entity::eSS_Moving].end = 12;
	// Water
	// 	Passive
	m_enemy[Entity::eEE_Water][Entity::eSS_Passive].sprite.base.kind = Entity::eRC_Sprite;
	m_enemy[Entity::eEE_Water][Entity::eSS_Passive].sprite.sheet = AM::Sprites();
	m_enemy[Entity::eEE_Water][Entity::eSS_Passive].sprite.sprite = 16;
	// 	Moving
	m_enemy[Entity::eEE_Water][Entity::eSS_Moving].sprite.base.kind = Entity::eRC_AnimSprite;
	m_enemy[Entity::eEE_Water][Entity::eSS_Moving].sprite.sheet = AM::Sprites();
	m_enemy[Entity::eEE_Water][Entity::eSS_Moving].sprite.sprite = 17;
	m_enemy[Entity::eEE_Water][Entity::eSS_Moving].frequency = 6;
	m_enemy[Entity::eEE_Water][Entity::eSS_Moving].start = 17;
	m_enemy[Entity::eEE_Water][Entity::eSS_Moving].end = 20;
	// Earth
	// 	Passive
	m_enemy[Entity::eEE_Earth][Entity::eSS_Passive].sprite.base.kind = Entity::eRC_Sprite;
	m_enemy[Entity::eEE_Earth][Entity::eSS_Passive].sprite.sheet = AM::Sprites();
	m_enemy[Entity::eEE_Earth][Entity::eSS_Passive].sprite.sprite = 24;
	// 	Moving
	m_enemy[Entity::eEE_Earth][Entity::eSS_Moving].sprite.base.kind = Entity::eRC_AnimSprite;
	m_enemy[Entity::eEE_Earth][Entity::eSS_Moving].sprite.sheet = AM::Sprites();
	m_enemy[Entity::eEE_Earth][Entity::eSS_Moving].sprite.sprite = 25;
	m_enemy[Entity::eEE_Earth][Entity::eSS_Moving].frequency = 12;
	m_enemy[Entity::eEE_Earth][Entity::eSS_Moving].start = 25;
	m_enemy[Entity::eEE_Earth][Entity::eSS_Moving].end = 28;

	// Weapon render objects
	// Lazzor
	m_weapon[Entity::eWT_Lazzor].sprite.base.kind = Entity::eRC_AnimSprite;
	m_weapon[Entity::eWT_Lazzor].sprite.sheet = AM::Sprites();
	m_weapon[Entity::eWT_Lazzor].sprite.sprite = 40;
	m_weapon[Entity::eWT_Lazzor].frequency = 6;
	m_weapon[Entity::eWT_Lazzor].start = 40;
	m_weapon[Entity::eWT_Lazzor].end = 43;
	// Flamer
	m_weapon[Entity::eWT_Flamer].sprite.base.kind = Entity::eRC_AnimSprite;
	m_weapon[Entity::eWT_Flamer].sprite.sheet = AM::Sprites();
	m_weapon[Entity::eWT_Flamer].sprite.sprite = 48;
	m_weapon[Entity::eWT_Flamer].frequency = 2;
	m_weapon[Entity::eWT_Flamer].start = 48;
	m_weapon[Entity::eWT_Flamer].end = 52;
	// Water
	m_weapon[Entity::eWT_Water].sprite.base.kind = Entity::eRC_AnimSprite;
	m_weapon[Entity::eWT_Water].sprite.sheet = AM::Sprites();
	m_weapon[Entity::eWT_Water].sprite.sprite = 56;
	m_weapon[Entity::eWT_Water].frequency = 2;
	m_weapon[Entity::eWT_Water].start = 56;
	m_weapon[Entity::eWT_Water].end = 59;

	// Shield render object
	m_shield.sprite.base.kind = Entity::eRC_AnimSprite;
	m_shield.sprite.sheet = AM::BigSprites();
	m_shield.sprite.sprite = 0;
	m_shield.frequency = 8;
	m_shield.start = 0;
	m_shield.end = 1;
}

void AM::UnloadResources()
{
	delete m_fontImage;
	m_fontImage = NULL;
	delete m_spritesImage;
	m_spritesImage = NULL;
	delete m_bigspritesImage;
	m_bigspritesImage = NULL;
}

MSSprite::SheetID AM::Sprites()
{
	return m_sprites;
}

MSSprite::SheetID AM::BigSprites()
{
	return m_bigsprites;
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

Entity::SRenderBase* AM::Shield()
{
	return reinterpret_cast<Entity::SRenderBase*>( &m_shield );
}

MSSprite::SpriteID AM::TestTube()
{
	return 64;
}

