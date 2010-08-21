/*
 *  GameThread.cc
 *  testgame
 *
 *  Created by Steve Barnett on 21/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "GameThread.h"

#include "MSTimer.h"
#include "MSInput.h"
#include "MSRender.h"
#include "MSFont.h"
#include "MSSprite.h"

#include <stdio.h>

#include "AssetManager.h"
#include "Entity.h"

GameThread::GameThread()
: MSThread()
{
	AM::LoadResources();
}

GameThread::~GameThread()
{
	AM::UnloadResources();
}

void GameThread::RunThread()
{
	GameSetup();

	while ( !ExitRequested() )
	{
		MSTimer::FrameStart();
		MSInput::ResetFrameKeys();

		GameUpdate();
	}
	Exit();
}

void GameThread::GameSetup()
{
	// Create the player's entity
	Entity::SpawnPlayer( MSVec( 64, 64 ) );
}

void GameThread::GameUpdate()
{
	MSRender::ClearColour( 0x000000ff );;
	MSRender::BeginScene();

	Entity::Update();

	// Hard coded rendering
	MSFont::RenderString( "ENEMIES AS WEAPONS!", MSVec( 320, 36 ), 5, MSVec( 24, 32 ), 0xffff00ff, true );

	// Entity rendering 
	Entity::Render();

	MSRender::EndScene();
	MSRender::Sync();
}

