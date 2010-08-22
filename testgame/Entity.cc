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

#include "MSTimer.h"
#include "MSInput.h"

#include "AssetManager.h"

#define MINX 0
#define MAXX 640
#define MINY 40	// Reserve 40 pixels for the header
#define MAXY 480

namespace Entity
{
	typedef std::vector<SBase*> EntityList;
	typedef std::vector<EntID> EntityIDList;

	EntityList s_entities;
	EntityList s_newEntities;
	EntityIDList s_deletedEntities;

	const EntID s_firstDynamic = 100; // Start of dymanically assigned ids
	EntID s_currentId = s_firstDynamic;

	bool s_midUpdate = false;

	// Internal methods
	EntID AssignID();

	void UpdateEntity( SBase* ent );
	void RenderEntity( SBase* ent );
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
}

void Entity::Render()
{
	for ( EntityList::iterator it = s_entities.begin(), end = s_entities.end(); it != end; ++it )
	{
		RenderEntity( *it );
	}
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
					player->base.pos.y -= 1;
				}
				else if ( MSInput::Key( 's' ) )
				{
					player->base.pos.y += 1;
				}
				if ( MSInput::Key( 'a' ) )
				{
					player->base.pos.x -= 1;
				}
				else if ( MSInput::Key( 'd' ) )
				{
					player->base.pos.x += 1;
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
						SpawnBullet( player->base.pos, MSVec( 1, 0 ) );
						player->shotTimeout = time + 100;
					}
				}
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
				++sprite->sprite.sprite;
				if ( sprite->sprite.sprite > sprite->end ) sprite->sprite.sprite = sprite->start;
			} break;
			default: break;
		};
	}
}

// Spawn helpers
Entity::EntID Entity::SpawnPlayer( const MSVec& pos )
{
	Entity::SPlayer* player = new Entity::SPlayer;
	Entity::SRenderAnimSprite* playerSprite = new Entity::SRenderAnimSprite;

	player->base.id = 0;
	player->base.kind = Entity::eEC_Player;
	player->base.pos = pos;
	player->base.layer = 4;
	player->base.size = MSVec( 32, 16 );
	player->base.render = reinterpret_cast<Entity::SRenderBase*>( playerSprite );

	playerSprite->sprite.base.kind = Entity::eRC_AnimSprite;
	playerSprite->sprite.sheet = AM::Ships();
	playerSprite->sprite.sprite = 0;
	playerSprite->start = 1;
	playerSprite->end = 4;

	return Entity::Add( reinterpret_cast<Entity::SBase*>( player ) );
}

Entity::EntID Entity::SpawnBullet( const MSVec& pos, const MSVec& vel )
{
	Entity::SBullet* bullet = new Entity::SBullet;
	Entity::SRenderSprite* bulletSprite = new Entity::SRenderSprite;

	bullet->base.id = 0;
	bullet->base.kind = Entity::eEC_Bullet;
	bullet->base.pos = pos;
	bullet->base.layer = 5;
	bullet->base.size = MSVec( 32, 16 );
	bullet->base.render = reinterpret_cast<Entity::SRenderBase*>( bulletSprite );
	bullet->vel = vel;

	bulletSprite->base.kind = Entity::eRC_Sprite;
	bulletSprite->sheet = AM::Ships();
	bulletSprite->sprite = 40;

	return Entity::Add( reinterpret_cast<Entity::SBase*>( bullet ) );
}

