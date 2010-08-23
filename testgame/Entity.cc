/*
 *  Entity.cc
 *  testgame
 *
 *  Created by Steve Barnett on 21/08/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#include "Entity.h"

#include <vector>
#include <stdlib.h>
#include <string.h>

#include "MSTimer.h"
#include "MSInput.h"
#include "MSFont.h"

#include "AssetManager.h"

namespace Entity
{
	typedef std::vector<SBase*> EntityList;
	typedef std::vector<EntID> EntityIDList;

	EntityList s_entities;
	EntityList s_newEntities;
	EntityIDList s_deletedEntities;
	SPlayer* s_player;

	const EntID s_firstDynamic = 100; // Start of dymanically assigned ids
	EntID s_currentId = s_firstDynamic;

	bool s_midUpdate = false;

	int s_frame = -1;
	int s_lastFrameRendered = -1;

	int s_nextWave = 0;
	int s_specialWeaponTimeout = 0;

	int s_score = 0;
	bool s_gameOver = false;

	enum EMagicNumbers
	{
		MINX = 0,
		MAXX = 640,
		MINY = 40,
		MAXY = 480,
		SPAWN_MARGIN = 40,

		SHIP_SPEED = 3,

		LASER_RELOAD = 250,
		SPECIAL_WEAPON_TIME = 10000,

		STARTING_HULL_INTEGRITY = 12,
		STARTING_MEGAVAX_INTEGRITY = 60,

		SCORE_CRASH = -10,
		SCORE_JETTISON = -5,
		SCORE_STUN = 1,
		SCORE_SUCK = 5,
		SCORE_BARGE = 10,
		SCORE_ELEMENTAL = 50,
		SCORE_SPECIAL = 100,
		SCORE_UBER = 1000,
	};

	// Internal methods
	EntID AssignID();

	void UpdateEntity( SBase* ent );
	SBase* GetEntity( EntID id );
	void UpdateSucking();
	void RenderEntity( SBase* ent );
	void RenderObject( SBase* ent, SRenderBase* obj, MSVec scale = MSVec( 1, 1 ), int layer = -1 );
	void RenderHUD();

	bool NeedAnimUpdate();

	void SpawnWaves();

	EntID BulletTest( const MSVec& pos, const MSVec& size );
	bool PlayerTest( const MSVec& pos, const MSVec& size );

	void ShieldUp();
	void ShieldDown();

	void ConsumeEntity( EntID id );

	void FireSpecialWeapon();
	void FireSpecialWeapon( EEnemyElement element );

	void AwardPoints( int points );
};

// Internals
Entity::EntID Entity::AssignID()
{
	return s_currentId++;
}

// Interface implementation
Entity::EntID Entity::Add( SBase* ent )
{
	// Entities passed in with a non-dynamic non-zero id will be left as is
	// All others will be assigned an id which will be returned
	if ( ent->id == 0 || ent->id >= s_firstDynamic )
	{
		ent->id = AssignID();
	}
	
	if ( s_midUpdate )
	{
		// Can't update the main entity list partway through an update
		s_newEntities.push_back( ent );
	}
	else
	{
		s_entities.push_back( ent );
	}
	return ent->id;
}

void Entity::Delete( EntID id, bool immediate/* = true*/ )
{
	if ( immediate )
	{
		for ( EntityList::iterator it = s_entities.begin(), end = s_entities.end(); it != end; ++it )
		{
			if ( (*it)->id == id )
			{
				delete *it;
				s_entities.erase( it );
				break;
			}
		}
	}
	else
	{
		// Can't update the main entity list partway through an update
		s_deletedEntities.push_back( id );
	}
}

void Entity::BeginGame()
{
	Entity::SpawnPlayer( MSVec( 64, 64 ) );
}

int Entity::GetScore()
{
	return s_score;
}

void Entity::AwardPoints( int points )
{
	s_score += points;
	if ( s_score < 0 ) s_score = 0;
}

bool Entity::IsGameOver()
{
	return s_gameOver;
}

void Entity::FinishGame()
{
	MASSERT( !s_midUpdate, "Cannot clean up mid update!" );

	// Delete the player's render proxy (the rest are static)
	delete s_player->base.render;
	s_player = NULL;

	// Delete all entities
	for ( EntityList::iterator it = s_entities.begin(), end = s_entities.end(); it != end; ++it )
	{
		if ( *it ) delete *it;
	}

	// Clear all lists
	s_entities.clear();
	s_newEntities.clear();
	s_deletedEntities.clear();
	
	// Reset all statics
	s_currentId = s_firstDynamic;
	s_midUpdate = false;
	s_frame = -1;
	s_lastFrameRendered = -1;
	s_nextWave = 0;
	s_specialWeaponTimeout = 0;
	s_score = 0;
	s_gameOver = false;
}

void Entity::Update()
{
	++s_frame;
	s_midUpdate = true;

	for ( EntityList::iterator it = s_entities.begin(), end = s_entities.end(); it != end; ++it )
	{
		UpdateEntity( *it );
	}

	UpdateSucking();

	SpawnWaves();

	s_midUpdate = false;

	// Add new entities
	for ( EntityList::iterator it = s_newEntities.begin(), end = s_newEntities.end(); it != end; ++it )
	{
		s_entities.push_back( *it );
	}
	s_newEntities.clear();

	// Remove deleted entities
	for ( EntityIDList::iterator it = s_deletedEntities.begin(), end = s_deletedEntities.end(); it != end; ++it )
	{
		Delete( (*it), true );
	}
	s_deletedEntities.clear();
}

void Entity::UpdateSucking()
{
	if ( s_player->sucking )
	{
		const MSVec& ppos = s_player->base.pos;
	
		// Get a list of enemies within range
		for ( EntityList::iterator it = s_entities.begin(), end = s_entities.end(); it != end; ++it )
		{
			SEnemy* ent = reinterpret_cast<SEnemy*>( *it );
			if ( ent->base.kind == eEC_Enemy && ent->state == eSS_Passive )
			{
				const MSVec dist = ent->base.pos - ppos;
				const MSVec conedist = MSVec( dist.x + 64, abs( dist.y ) );
				// Only consider enemies in a cone to the front
				if ( conedist.x >= 0 && conedist.x < 256 && conedist.y <= conedist.x )
				{
					const MSVec suck = min( dist, MSVec( 4, 4 ) );
					ent->base.pos = ent->base.pos - suck;
					if ( dist - suck == MSVec( 0, 0 ) )
					{
						ConsumeEntity( ent->base.id );
					}
				}
			}
		}
	}
}

void Entity::Render()
{
	RenderHUD();

	for ( EntityList::iterator it = s_entities.begin(), end = s_entities.end(); it != end; ++it )
	{
		RenderEntity( *it );
	}

	// Prevent multiple animation updates (which
	// occur in the render pass) per frame
	s_lastFrameRendered = s_frame;
}

void Entity::UpdateEntity( SBase* ent )
{
	if ( ent )
	{
		switch( ent->kind )
		{
			case eEC_Player:
			{
				SPlayer* player = reinterpret_cast<SPlayer*>( ent );
				// Check for game over
				if ( player->hullIntegrity <= 0 || player->megaVacIntegrity <= 0 )
				{
					s_gameOver = true;
				}
				// Player movement
				if ( MSInput::Key( 'w' ) )
				{
					player->base.pos.y -= SHIP_SPEED;
				}
				else if ( MSInput::Key( 's' ) )
				{
					player->base.pos.y += SHIP_SPEED;
				}
				if ( MSInput::Key( 'a' ) )
				{
					player->base.pos.x -= SHIP_SPEED;
				}
				else if ( MSInput::Key( 'd' ) )
				{
					player->base.pos.x += SHIP_SPEED;
				}
				if ( MSInput::Key( 'q' ) )
				{
					player->sucking = true;
				}
				else
				{
					player->sucking = false;
				}
				if ( MSInput::Key( 'e' ) )
				{
					MSInput::ResetKey( 'e' );
					FireSpecialWeapon();
				}
				// Keep player on screen
				MSVec halfSize = MSVec( player->base.size.x / 2, player->base.size.y / 2 );
				player->base.pos = min( max( player->base.pos, MSVec( MINX, MINY ) + MSVec( halfSize ) ), MSVec( MAXX, MAXY ) - halfSize );
				if ( MSInput::Key( ' ' ) )
				{
					// Fire primary
					const int time = MSTimer::GetTime();
					if ( time > player->shotTimeout )
					{
						EWeaponType weapon = eWT_Lazzor;
						switch ( player->special )
						{
							case eEE_Fire:
							{
								weapon = eWT_Flamer;
							} break;
							case eEE_Water:
							{
								weapon = eWT_Water;
							} break;
						}
						SpawnBullet( player->base.pos, MSVec( 3, 0 ), weapon );
						player->shotTimeout = time + LASER_RELOAD;
					}
				}
				// Reset special weapon?
				if ( MSTimer::GetTime() > s_specialWeaponTimeout )
				{
					player->special = eEE_Count;
					s_specialWeaponTimeout = 0;
					ShieldDown();
				}
			} break;
			case eEC_Enemy:
			{
				SEnemy* enemy = reinterpret_cast<SEnemy*>( ent );
				// Enemy movement
				if ( enemy->state == eSS_Moving )
				{
					enemy->base.pos = enemy->base.pos + enemy->vel;
					MSVec halfSize = MSVec( enemy->base.size.x / 2, enemy->base.size.y / 2 );
					if ( 	enemy->base.pos.x < MINX - halfSize.x )
					{
						--s_player->megaVacIntegrity;
						Delete( enemy->base.id, false );
					}
					if (	enemy->base.pos.y < MINY - halfSize.y ||
								enemy->base.pos.y > MAXY + halfSize.y )
					{
						Delete( enemy->base.id, false );
					}
				}
				// Collision tests
				if ( EntID bullet = BulletTest( enemy->base.pos, enemy->base.size ) )
				{
					SBullet* bulletent = reinterpret_cast<SBullet*>( GetEntity( bullet ) );
					enemy->state = eSS_Passive;
					switch ( bulletent->type )
					{
						case eWT_Lazzor:
						{
							AwardPoints( SCORE_STUN );
						} break;
						case eWT_Flamer:
						{
							if ( enemy->element == eEE_Water )
							{
								Delete( enemy->base.id, false );
							}
							AwardPoints( SCORE_ELEMENTAL );
						} break;
						case eWT_Water:
						{
							if ( enemy->element == eEE_Fire )
							{
								Delete( enemy->base.id, false );
							}
							AwardPoints( SCORE_ELEMENTAL );
						} break;
						default: break;
					}
					Delete( bullet, false );
					return;
				}
				if ( enemy->state == eSS_Moving && PlayerTest( enemy->base.pos, enemy->base.size ) )
				{
					if ( !s_player->shieldUp )
					{
						--s_player->hullIntegrity;
						AwardPoints( SCORE_CRASH );
					}
					else
					{
						AwardPoints( SCORE_BARGE );
					}
					Delete( enemy->base.id, false );
				}
				// Update render data to match state
				enemy->base.render = AM::Enemy( enemy->element, enemy->state );
			} break;
			case eEC_Bullet:
			{
				SBullet* bullet = reinterpret_cast<SBullet*>( ent );
				bullet->base.pos = bullet->base.pos + bullet->vel;
				if ( bullet->base.pos.x > 640 ) { Delete( bullet->base.id, false ); } 
			} break;
			default: break;
		}
	}
}

Entity::SBase* Entity::GetEntity( Entity::EntID id )
{
	for ( EntityList::iterator it = s_entities.begin(), end = s_entities.end(); it != end; ++it )
	{
		if ( *it && (*it)->id == id )
		{
			return *it;
		}
	}
	return NULL;
}

void Entity::RenderEntity( SBase* ent )
{
	if ( ent && ent->render )
	{
		RenderObject( ent, ent->render );
	}
}

void Entity::RenderObject( SBase* ent, SRenderBase* obj, MSVec scale/* = MSVec( 1, 1 )*/, int layer/* = -1*/ )
{
	if ( obj )
	{
		switch ( obj->kind )
		{
			case eRC_Sprite:
			{
				SRenderSprite* sprite = reinterpret_cast<SRenderSprite*>( obj );
				MSSprite::RenderSprite( sprite->sheet, sprite->sprite, ent->pos, ( layer == -1 ) ? ent->layer : layer, ent->size * scale );
			} break;
			case eRC_AnimSprite:
			{
				SRenderAnimSprite* sprite = reinterpret_cast<SRenderAnimSprite*>( obj );
				MSSprite::RenderSprite( sprite->sprite.sheet, sprite->sprite.sprite, ent->pos, ( layer == -1 ) ? ent->layer : layer, ent->size * scale );
				// Tick the anim
				if ( NeedAnimUpdate() && ( s_frame % sprite->frequency ) == 0 )
				{
					++sprite->sprite.sprite;
					if ( sprite->sprite.sprite > sprite->end ) sprite->sprite.sprite = sprite->start;
				}
			} break;
			case eRC_Mux:
			{
				SRenderMux* mux = reinterpret_cast<SRenderMux*>( obj );
				RenderObject( ent, mux->obj[0], mux->scale[0], ent->layer + mux->layeroffset[0] );
				RenderObject( ent, mux->obj[1], mux->scale[1], ent->layer + mux->layeroffset[1] );
			} break;
			default: break;
		};
	}
}

void Entity::RenderHUD()
{
	// Title
	MSFont::RenderString( "ASTROVAX!", MSVec( 320, 36 ), 5, MSVec( 12, 24 ), 0xff0000ff, true );

	// Score
	char score[32];
	snprintf( score, 32, "%i", s_score );
	MSFont::RenderString( score, MSVec( 320, 64 ), 5, MSVec( 20, 32 ), 0xff0000ff, true );

	// Hull integrity
	char hull[16];
	snprintf( hull, 16, "%i", s_player->hullIntegrity );
	MSFont::RenderString( "HULL", MSVec( 8, 8 ), 5, MSVec( 12, 16 ) );
	MSFont::RenderString( hull, MSVec( 16, 26 ), 5, MSVec( 12, 16 ) );

	// MegaVac integrity
	char megavac[16];
	snprintf( megavac, 16, "%i", s_player->megaVacIntegrity );
	MSVec mvpos( MAXX - MSFont::CalculateSize( "MEGAVAC", MSVec( 12, 16 ) ).x - 8, 8 );
	MSFont::RenderString( "MEGAVAC", mvpos, 5, MSVec( 12, 16 ) );
	MSFont::RenderString( megavac, mvpos + MSVec( 8, 18 ), 5, MSVec( 12, 16 ) );

	// Resources
	MSVec start( 32, 42 );
	int first = MAX_RESOURCES - s_player->resources;
	for ( int i = first; i < MAX_RESOURCES; ++i )
	{
		int idx = i - first;
		// BEWARE VILE HARDCODING AHEAD
		MSSprite::RenderSprite( AM::Sprites(), /*Eeugh!*/8 + 8 * s_player->res[idx], start + MSVec( 32 * i, 0 ), 6, MSVec( 32, 16 ), 0xffffffff );
		// WHEW! WE GOT THROUGH THAT
	}
	// 'Test Tube'
	MSSprite::RenderSprite( AM::Sprites(), AM::TestTube(), start, 5, MSVec( 32, 16 ), 0x7f7fffff );
	for ( int i = 1; i < 5; ++i )
	{
		MSSprite::RenderSprite( AM::Sprites(), AM::TestTube() + 2, start + MSVec( 32 * i, 0 ), 5, MSVec( 32, 16 ), 0x7f7fffff );
	}
	MSSprite::RenderSprite( AM::Sprites(), AM::TestTube() + 1, start + MSVec( 32 * 5, 0 ), 5, MSVec( 32, 16 ), 0x7f7fffff );
	MSSprite::RenderSprite( AM::Sprites(), AM::TestTube() + 2, start + MSVec( 32 * 6, 0 ), 5, MSVec( 32, 16 ), 0x7f7fffff );
	MSSprite::RenderSprite( AM::Sprites(), AM::TestTube() + 2, start + MSVec( 32 * 7, 0 ), 5, MSVec( 32, 16 ), 0x7f7fffff );
	MSSprite::RenderSprite( AM::Sprites(), AM::TestTube() + 3, start + MSVec( 32 * 8, 0 ), 5, MSVec( 32, 16 ), 0x7f7fffff );
}

bool Entity::NeedAnimUpdate()
{
	return s_frame != s_lastFrameRendered;
}

// Spawn helpers
Entity::EntID Entity::SpawnPlayer( const MSVec& pos )
{
	SPlayer* player = new SPlayer;
	SRenderMux* renderMux = new SRenderMux;
	renderMux->base.kind = eRC_Mux;
	renderMux->obj[0] = AM::Player( eSS_Moving );
	renderMux->obj[1] = NULL;
	renderMux->scale[0] = MSVec( 1, 1 );
	renderMux->scale[1] = MSVec( 1, 2 );
	renderMux->layeroffset[0] = 0;
	renderMux->layeroffset[1] = -1;

	player->base.id = 0;
	player->base.kind = eEC_Player;
	player->base.pos = pos;
	player->base.layer = 4;
	player->base.size = MSVec( 32, 16 );
	player->base.render = reinterpret_cast<SRenderBase*>( renderMux );
	player->shotTimeout = 0;
	player->hullIntegrity = STARTING_HULL_INTEGRITY;
	player->megaVacIntegrity = STARTING_MEGAVAX_INTEGRITY;
	player->shieldUp = false;
	player->resources = 0;
	player->special = eEE_Count;	// No special weapon

	s_player = player;

	return Entity::Add( reinterpret_cast<Entity::SBase*>( player ) );
}

Entity::EntID Entity::SpawnEnemy( const MSVec& pos, const MSVec& vel, Entity::EEnemyElement element )
{
	SEnemy* enemy = new SEnemy;

	enemy->base.id = 0;
	enemy->base.kind = eEC_Enemy;
	enemy->base.pos = pos;
	enemy->base.layer = 5;
	enemy->base.size = MSVec( 32, 16 );
	enemy->base.render = AM::Enemy( element, eSS_Moving );
	enemy->element = element;
	enemy->vel = vel;
	enemy->state = eSS_Moving;

	return Entity::Add( reinterpret_cast<Entity::SBase*>( enemy ) );
}

Entity::EntID Entity::SpawnBullet( const MSVec& pos, const MSVec& vel, EWeaponType weapon )
{
	SBullet* bullet = new SBullet;

	bullet->base.id = 0;
	bullet->base.kind = eEC_Bullet;
	bullet->base.pos = pos;
	bullet->base.layer = 6;
	bullet->base.size = MSVec( 32, 16 );
	bullet->base.render = AM::Weapon( weapon );
	bullet->vel = vel;
	bullet->type = weapon;

	return Entity::Add( reinterpret_cast<Entity::SBase*>( bullet ) );
}

void Entity::SpawnWaves()
{
	// TESTCODE: Spawn new enemies if necessary
	const int time = MSTimer::GetTime();
	if ( time > s_nextWave )
	{
		// Decide on a base position
		// Waves spawning too close to the top or bottom of the screen will be
		// pushed inward. This should result in more enemies spawning at the
		// edges which may or may not be good for gameplay.
		MSVec pos = MSVec( MAXX + 8, MINY + ( rand() % ( MAXY - MINY ) ) );
		if ( pos.y < MINY + SPAWN_MARGIN ) { pos.y = MINY + SPAWN_MARGIN; }
		if ( pos.y > MAXY - SPAWN_MARGIN ) { pos.y = MAXY - SPAWN_MARGIN; }
		EEnemyElement elem = static_cast<EEnemyElement>( rand() %  Entity::eEE_Count );

		// Pick a formation
		const int formations = 2;
		const int formation = rand() % formations;
		switch ( formation )
		{
			case 0:
			{
				Entity::SpawnEnemy( pos, MSVec( -2, 0 ), elem );
			} break;
			case 1:
			{
				Entity::SpawnEnemy( pos, MSVec( -2, 0 ), elem );
				Entity::SpawnEnemy( pos + MSVec( 24, -16 ), MSVec( -2, 0 ), elem );
				Entity::SpawnEnemy( pos + MSVec( 24, 16 ), MSVec( -2, 0 ), elem );
			} break;
		}

		s_nextWave = time + 1750;
	}
}

Entity::EntID Entity::BulletTest( const MSVec& pos, const MSVec& size )
{
	MASSERT( s_midUpdate, "Calling BulletTest outside of Update will cause bad problems. Don't do it." );
	
	for ( EntityList::iterator it = s_entities.begin(), end = s_entities.end(); it != end; ++it )
	{
		SBullet* ent = reinterpret_cast<SBullet*>( *it );
		if ( ent && ent->base.kind == eEC_Bullet )
		{
			// Stupid cludge to find the bullet's hit pixel
			const MSVec halfSize = MSVec( size.x / 2, size.y / 2 );
			const MSVec bpos = ent->base.pos - MSVec( 4, 0 );
			const MSVec rpos = min( max( bpos, pos - halfSize ), pos + halfSize );
			if ( bpos == rpos )
			{
				return ent->base.id;
			}
		}
	}
	return 0;
}

bool Entity::PlayerTest( const MSVec& pos, const MSVec& size )
{
	const SPlayer* const ent = s_player;

	const MSVec ppos = ent->base.pos;
	const MSVec psize = ent->base.size;

	const MSVec grazeMargin( 4 , 4 );
	const MSVec dthresh = ( psize + size ) / MSVec( 2, 2 ) - grazeMargin;

	const MSVec dist = ppos - pos;
	const MSVec absdist = MSVec( abs( dist.x ), abs( dist.y ) );

	return ( absdist.x < dthresh.x && absdist.y < dthresh.y );
}

void Entity::ShieldUp()
{
	s_player->shieldUp = true;
	reinterpret_cast<SRenderMux*>( s_player->base.render)->obj[1] = AM::Shield();
}

void Entity::ShieldDown()
{
	s_player->shieldUp = false;
	reinterpret_cast<SRenderMux*>( s_player->base.render)->obj[1] = NULL;
}

void Entity::ConsumeEntity( Entity::EntID id )
{
	SEnemy* ent = reinterpret_cast<SEnemy*>( GetEntity( id ) );
	if ( ent )
	{
		if ( s_player->resources < MAX_RESOURCES )
		{
			s_player->res[s_player->resources++] = ent->element;
		}
		AwardPoints( SCORE_SUCK );
		Delete( id, false );
	}
}

void Entity::FireSpecialWeapon()
{
	// Got enough resources?
	if ( s_player->resources > 0 )
	{
		if ( s_player->resources >= 3 )
		{
			// Do they match?
			const int start = s_player->resources - 3;
			EEnemyElement type = s_player->res[start];
			if ( s_player->res[start + 1] == type && s_player->res[start + 2] == type )
			{
				// We can fire
				FireSpecialWeapon( type );
				s_player->resources -= 3;
				AwardPoints( SCORE_SPECIAL );
				return;
			}
		}

		// Otherwise we jettison a resource
		--s_player->resources;
		AwardPoints( SCORE_JETTISON );
	}
}

void Entity::FireSpecialWeapon( EEnemyElement element )
{
	// Set the timeout to reset the special weapon
	s_specialWeaponTimeout = MSTimer::GetTime() + SPECIAL_WEAPON_TIME;
	// Set the special weapon type
	s_player->special = element;
	if ( element == eEE_Earth )
	{
		ShieldUp();
	}
}

