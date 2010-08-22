/*
 *  Entity.cc
 *  testgame
 *
 *  Created by Steve Barnett on 21/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Entity.h"

#include <vector>
#include <stdlib.h>

#include "MSTimer.h"
#include "MSInput.h"

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

	enum EMagicNumbers
	{
		MINX = 0,
		MAXX = 640,
		MINY = 40,
		MAXY = 480,
		SPAWN_MARGIN = 40,
		SHIP_SPEED = 3,
		LASER_RELOAD = 250,
	};

	// Internal methods
	EntID AssignID();

	void UpdateEntity( SBase* ent );
	void RenderEntity( SBase* ent );

	bool NeedAnimUpdate();

	void SpawnWaves();

	EntID BulletTest( const MSVec& pos, const MSVec& size );
	bool PlayerTest( const MSVec& pos, const MSVec& size );
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

void Entity::Update()
{
	++s_frame;

	s_midUpdate = true;
	for ( EntityList::iterator it = s_entities.begin(), end = s_entities.end(); it != end; ++it )
	{
		UpdateEntity( *it );
	}
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

	// Spawn a new wave if necessary
	SpawnWaves();
}

void Entity::Render()
{
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
				// Keep player on screen
				MSVec halfSize = MSVec( player->base.size.x / 2, player->base.size.y / 2 );
				player->base.pos = min( max( player->base.pos, MSVec( MINX, MINY ) + MSVec( halfSize ) ), MSVec( MAXX, MAXY ) - halfSize );
				if ( MSInput::Key( ' ' ) )
				{
					// Fire primary
					const int time = MSTimer::GetTime();
					if ( time > player->shotTimeout )
					{
						SpawnBullet( player->base.pos, MSVec( 3, 0 ) );
						player->shotTimeout = time + LASER_RELOAD;
					}
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
					if ( 	enemy->base.pos.x < MINX - halfSize.x ||
								//enemy->base.pos.x > MAXX + halfSize.x ||
								enemy->base.pos.y < MINY - halfSize.y ||
								enemy->base.pos.y > MAXY + halfSize.y )
					{
						Delete( enemy->base.id, false );
					}
				}
				// Collision tests
				if ( EntID bullet = BulletTest( enemy->base.pos, enemy->base.size ) )
				{
					enemy->state = eSS_Passive;
					Delete( bullet, false );
				}
				if ( enemy->state == eSS_Moving && PlayerTest( enemy->base.pos, enemy->base.size ) )
				{
					MASSERT( 0, "DEADDEADDEAD" );
				}
				// Update render data to match state
				enemy->base.render = AM::Enemy( enemy->element, enemy->state );
				// ToDo: Fire stuff!
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

void Entity::RenderEntity( SBase* ent )
{
	if ( ent && ent->render )
	{
		switch ( ent->render->kind )
		{
			case eRC_Sprite:
			{
				SRenderSprite* sprite = reinterpret_cast<SRenderSprite*>( ent->render );
				MSSprite::RenderSprite( sprite->sheet, sprite->sprite, ent->pos, ent->layer, ent->size );
			} break;
			case eRC_AnimSprite:
			{
				SRenderAnimSprite* sprite = reinterpret_cast<SRenderAnimSprite*>( ent->render );
				MSSprite::RenderSprite( sprite->sprite.sheet, sprite->sprite.sprite, ent->pos, ent->layer, ent->size );
				// Tick the anim
				if ( NeedAnimUpdate() && ( s_frame % sprite->frequency ) == 0 )
				{
					++sprite->sprite.sprite;
					if ( sprite->sprite.sprite > sprite->end ) sprite->sprite.sprite = sprite->start;
				}
			} break;
			default: break;
		};
	}
}

bool Entity::NeedAnimUpdate()
{
	return s_frame != s_lastFrameRendered;
}

// Spawn helpers
Entity::EntID Entity::SpawnPlayer( const MSVec& pos )
{
	Entity::SPlayer* player = new Entity::SPlayer;

	player->base.id = 0;
	player->base.kind = Entity::eEC_Player;
	player->base.pos = pos;
	player->base.layer = 4;
	player->base.size = MSVec( 32, 16 );
	player->base.render = AM::Player( eSS_Moving );

	s_player = player;

	return Entity::Add( reinterpret_cast<Entity::SBase*>( player ) );
}

Entity::EntID Entity::SpawnEnemy( const MSVec& pos, const MSVec& vel, Entity::EEnemyElement element )
{
	Entity::SEnemy* enemy = new Entity::SEnemy;

	enemy->base.id = 0;
	enemy->base.kind = Entity::eEC_Enemy;
	enemy->base.pos = pos;
	enemy->base.layer = 5;
	enemy->base.size = MSVec( 32, 16 );
	enemy->base.render = AM::Enemy( element, eSS_Moving );
	enemy->element = element;
	enemy->vel = vel;
	enemy->state = eSS_Moving;

	return Entity::Add( reinterpret_cast<Entity::SBase*>( enemy ) );
}

Entity::EntID Entity::SpawnBullet( const MSVec& pos, const MSVec& vel )
{
	Entity::SBullet* bullet = new Entity::SBullet;

	bullet->base.id = 0;
	bullet->base.kind = Entity::eEC_Bullet;
	bullet->base.pos = pos;
	bullet->base.layer = 6;
	bullet->base.size = MSVec( 32, 16 );
	bullet->base.render = AM::Weapon( eWT_Lazzor );
	bullet->vel = vel;

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

